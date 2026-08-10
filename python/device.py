"""
DeviceBridge — Linux-side aggregation layer over the Arduino Bridge RPC
calls exposed by communication.cpp.

The MCU remains authoritative for all sensor/session/buzzer state; this
module only aggregates individual RPC calls into a protocol envelope
and forwards validated commands. No session logic is duplicated here.
"""

from arduino.app_utils import Bridge

import protocol

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


class DeviceBridge:
    def __init__(self):
        # Last known-good telemetry. Served (flagged stale) if a poll
        # fails, so a transient Bridge hiccup doesn't blank the dashboard.
        self._last_data = None

    def get_telemetry(self):
        """Polls the MCU over Bridge RPC and returns a telemetry envelope.

        On failure, returns an "error" envelope, or the last-good
        telemetry marked stale if one is available.
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
