"""
Linux-side AI recommendation layer.

    Sensor data (MCU)
        |
        v
    Session context
        |
        +-----------------------------+
        |                             |
        v                             v
    Rule-based (recommendation.cpp,   Local LLM Brick
    arrives via telemetry.recommendation)   |
        |                             |
        +--------------+--------------+
                       |
                       v
             Final recommendation -> WebUI

The rule-based recommendation always comes straight through with
telemetry -- it is never removed and is used immediately and as the
fallback. The local LLM Brick only *refines* it, opportunistically,
in a background thread, and can never block the ~1 Hz telemetry loop
or crash the app: any import/init/inference failure just leaves the
provider "unavailable"/"error" and callers keep getting a usable
recommendation (AI-cached, or rule-based) either way.
"""

import threading
import time

try:
    from arduino.app_bricks.llm import LargeLanguageModel
    _IMPORT_OK = True
    _IMPORT_ERROR = None
except Exception as error:  # Brick not installed / not enabled on this board
    LargeLanguageModel = None
    _IMPORT_OK = False
    _IMPORT_ERROR = str(error)

SYSTEM_PROMPT = (
    "You are a concise study-environment assistant embedded in a student's "
    "desk device. You are given the student's current session state and "
    "sensor readings and must reply with exactly ONE short, actionable "
    "recommendation (1-2 sentences) about lighting, movement or inactivity, "
    "focus vs. break state, and study duration. Only use the values you are "
    "given -- never invent measurements. Do not give medical advice, do not "
    "write a motivational speech, do not use markdown, and do not mention "
    "that you are an AI or a language model. Reply with the recommendation "
    "text only."
)

# Re-request the AI only when context has meaningfully changed, and no more
# often than this -- never on every ~1 Hz telemetry tick.
MIN_SECONDS_BETWEEN_REQUESTS = 20
SESSION_TIME_BUCKET_SECONDS = 60  # roughly once per study minute at most


class AIRecommendationProvider:
    """Wraps the local LLM Brick with caching, throttling, and a
    background-thread inference call so it never blocks the caller.
    """

    def __init__(self):
        self._lock = threading.Lock()
        self._llm = None
        self._available = False
        self._init_error = _IMPORT_ERROR

        self._cached_text = None
        self._status = "unavailable"  # unavailable | ready | generating | error
        self._last_context_key = None
        self._last_request_at = 0.0

        self._init_llm()

    # ---- setup ---------------------------------------------------------

    def _init_llm(self):
        if not _IMPORT_OK:
            return

        try:
            self._llm = LargeLanguageModel(system_prompt=SYSTEM_PROMPT)
            self._available = True
            self._status = "ready"
        except Exception as error:
            self._llm = None
            self._available = False
            self._init_error = str(error)
            self._status = "unavailable"

    # ---- public API ------------------------------------------------------

    def is_available(self):
        return self._available

    def get_recommendation(self, context, rule_based_text):
        """Returns (text, is_ai, status). Never blocks on inference.

        Triggers a background LLM request when the context has changed
        meaningfully and enough time has passed since the last request,
        then immediately returns the best text available right now: the
        cached AI recommendation if one exists, otherwise the rule-based
        text supplied by the caller (which always comes from the MCU's
        deterministic provider).
        """
        if not self._available:
            return rule_based_text, False, "unavailable"

        context_key = self._context_key(context)
        now = time.monotonic()

        with self._lock:
            should_request = (
                self._status != "generating"
                and context_key != self._last_context_key
                and (now - self._last_request_at) >= MIN_SECONDS_BETWEEN_REQUESTS
            )

            if should_request:
                self._status = "generating"
                self._last_context_key = context_key
                self._last_request_at = now
                threading.Thread(
                    target=self._run_inference,
                    args=(context, rule_based_text),
                    daemon=True,
                ).start()

            status = self._status
            cached = self._cached_text

        if cached:
            return cached, True, status

        return rule_based_text, False, status

    # ---- internal --------------------------------------------------------

    def _context_key(self, context):
        # Coarser than raw telemetry so we don't re-request on every
        # single-second tick of idleTime/sessionTime.
        return (
            context.get("session_state"),
            bool(context.get("motion")),
            round(context.get("light_percent", 0) / 10) * 10,
            int(context.get("session_time_seconds", 0)) // SESSION_TIME_BUCKET_SECONDS,
            context.get("idle_time_seconds", 0) >= 30,
        )

    def _run_inference(self, context, rule_based_text):
        prompt = self._build_prompt(context, rule_based_text)

        try:
            response = self._llm.chat(prompt)
            text = (response or "").strip()

            if not text:
                raise ValueError("empty AI response")

            with self._lock:
                self._cached_text = text
                self._status = "ready"
        except Exception:
            # Keep whatever was cached before (if anything); the caller
            # falls back to the rule-based text automatically otherwise.
            with self._lock:
                self._status = "error"

    def _build_prompt(self, context, rule_based_text):
        return (
            "Study environment context:\n"
            "- session_state: {session_state}\n"
            "- motion_detected: {motion}\n"
            "- light_percent: {light_percent}\n"
            "- session_time_seconds: {session_time_seconds}\n"
            "- idle_time_seconds: {idle_time_seconds}\n"
            "- rule_based_recommendation: {rule_based_text}\n"
            "Give one short recommendation for the student now."
        ).format(
            session_state=context.get("session_state"),
            motion=context.get("motion"),
            light_percent=context.get("light_percent"),
            session_time_seconds=context.get("session_time_seconds"),
            idle_time_seconds=context.get("idle_time_seconds"),
            rule_based_text=rule_based_text,
        )
