import importlib.util
import sys
import types
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]

arduino = types.ModuleType("arduino")
app_utils = types.ModuleType("arduino.app_utils")
app_bricks = types.ModuleType("arduino.app_bricks")
llm_module = types.ModuleType("arduino.app_bricks.llm")


class DummyLLM:
    def __init__(self, system_prompt=None):
        self.system_prompt = system_prompt

    def chat(self, prompt):
        return "A" * 1000


app_utils.Bridge = object()
llm_module.LargeLanguageModel = DummyLLM
app_bricks.llm = llm_module
arduino.app_utils = app_utils
arduino.app_bricks = app_bricks

sys.modules.setdefault("arduino", arduino)
sys.modules.setdefault("arduino.app_utils", app_utils)
sys.modules.setdefault("arduino.app_bricks", app_bricks)
sys.modules.setdefault("arduino.app_bricks.llm", llm_module)

spec = importlib.util.spec_from_file_location("ai_recommendation", ROOT / "python" / "ai_recommendation.py")
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


class AIHardeningTests(unittest.TestCase):
    def test_run_inference_limits_response_length(self):
        provider = module.AIRecommendationProvider()
        provider._available = True
        provider._llm = DummyLLM()
        provider._status = "ready"

        provider._run_inference(
            {
                "session_state": "focus",
                "motion": True,
                "light_percent": 80,
                "session_time_seconds": 120,
                "idle_time_seconds": 0,
            },
            "Keep the desk bright and stay focused.",
        )

        self.assertIsNotNone(provider._cached_text)
        self.assertLessEqual(len(provider._cached_text), module.MAX_RECOMMENDATION_CHARS)


if __name__ == "__main__":
    unittest.main()
