"""
DeviceBridge -- Linux-side aggregation layer over the Arduino Bridge RPC
calls exposed by communication.cpp.

The MCU remains authoritative for all sensor/session/buzzer state, and
its rule-based recommendation (recommendation.cpp) remains the primary
and fallback source of truth. AIRecommendationProvider only refines it
opportunistically -- see ai_recommendation.py for the layering and the
non-blocking/background design.
"""

from arduino.app_utils import Bridge

import protocol
from ai_recommendation import AIRecommendationProvider

# Commands the WebUI may issue. Each name matches a Bridge.provide_safe
# RPC already registered on the MCU (see communication.cpp) 1:1, so no
# translation table is needed -- just a whitelist.
VALID_COMMANDS = frozenset(
    {
        "start_session",
        "pause_session",
        "resume_session",
        "stop_session",
        "start_break",
    }
)

IDLE_WARNING_THRESHOLD_SECONDS = 60  # mirrors SESSION_IDLE_TIMEOUT in config.h


class DeviceBridge:
    def __init__(self):
        # Last known-good telemetry. Served (flagged stale) if a poll
        # fails, so a transient Bridge hiccup doesn't blank the dashboard.
        self._last_data = None
        self._ai = AIRecommendationProvider()

    def get_telemetry(self):
        """Polls the MCU over Bridge RPC and returns a telemetry envelope.

        On failure, returns an "error" envelope, or the last-good
        telemetry marked stale if one is available. On success, layers
        the (non-blocking, possibly cached) AI recommendation on top of
        the MCU's rule-based one before returning.
        """
        try:
            data = {
                "motion": bool(Bridge.call("get_motion")),
                "light": int(Bridge.call("get_light")),
                "focus": bool(Bridge.call("get_focus")),
                "sessionTime": int(Bridge.call("get_session_time")),
                "idleTime": int(Bridge.call("get_idle_time")),
                "recommendation": str(Bridge.call("get_recommendation")),
            }
        except Exception as error:
            if self._last_data is not None:
                stale = dict(self._last_data)
                stale["stale"] = True
                return protocol.build_telemetry(stale)

            return protocol.build_error(
                "BRIDGE_UNAVAILABLE",
                "Could not reach the UNO Q microcontroller: {}".format(error),
            )

        data = self._apply_ai_recommendation(data)
        self._last_data = data
        return protocol.build_telemetry(data)

    def send_command(self, command):
        """Validates and forwards a WebUI command to the MCU via Bridge.

        Returns an "event" envelope on success, an "error" envelope
        otherwise. The MCU's existing session functions (see
        session.cpp) do the actual work; this only relays the call.
        """
        if command not in VALID_COMMANDS:
            return protocol.build_error(
                "INVALID_COMMAND",
                "Unsupported command: {}".format(command),
            )

        try:
            Bridge.call(command)
        except Exception as error:
            return protocol.build_error(
                "BRIDGE_UNAVAILABLE",
                "Could not send '{}' to the UNO Q: {}".format(command, error),
            )

        return protocol.build_event("{}_ack".format(command))

    # ---- AI layering -----------------------------------------------------

    def _session_state(self, data):
        if not data["motion"] and data["idleTime"] >= IDLE_WARNING_THRESHOLD_SECONDS:
            return "idle"
        return "focus" if data["focus"] else "break"

    def _apply_ai_recommendation(self, data):
        rule_based_text = data["recommendation"]
        session_state = self._session_state(data)

        context = {
            "session_state": session_state,
            "motion": data["motion"],
            "light_percent": data["light"],
            "session_time_seconds": data["sessionTime"],
            "idle_time_seconds": data["idleTime"],
        }

        text, is_ai, ai_status = self._ai.get_recommendation(context, rule_based_text)

        data["recommendation"] = text
        data["recommendationSource"] = "ai" if is_ai else "rule"
        data["aiStatus"] = ai_status  # unavailable | ready | generating | error
        return data
