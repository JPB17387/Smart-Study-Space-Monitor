"""
Smart Study AI Platform -- Linux-side application entry point.

Runs under Arduino App Lab. Hosting is provided by the WebUI - HTML
Brick declared in app.yaml (arduino:web_ui) -- there is no separate
Flask/FastAPI process to stand up by hand; the Brick already runs an
HTTP server on the Linux side and exposes both static assets/ files
and the API routes registered below.

Do not reintroduce the old Serial-JSON polling loop or manual uvicorn
setup -- both are superseded by the Bridge RPC + WebUI Brick stack.
"""

import time

from arduino.app_utils import App
from arduino.app_bricks.web_ui import WebUI

from device import DeviceBridge

print("Smart Study AI Platform")

device = DeviceBridge()
web_ui = WebUI()


def api_get_state():
    """GET /api/state -> current telemetry envelope (~1 Hz from the UI)."""
    return device.get_telemetry()


def api_send_command(command: str):
    """POST /api/command/{command} -> forwards a WebUI command to the MCU."""
    return device.send_command(command)


web_ui.expose_api("GET", "/api/state", api_get_state)
web_ui.expose_api("POST", "/api/command/{command}", api_send_command)


def loop():
    # Reserved for future periodic/background work (e.g. pushing telemetry
    # proactively, or handling a later "ai" message type). The dashboard
    # currently pulls telemetry itself via /api/state, so this loop does
    # not need to poll the MCU on its own.
    time.sleep(1)


App.run(user_loop=loop)
