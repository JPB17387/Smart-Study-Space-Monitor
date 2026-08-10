"""
Application-level message envelope for the Smart Study AI Platform.

    {
        "type": "telemetry" | "command" | "event" | "error" | "ai",
        "version": 1,
        "data": { ... }
    }

This is the *application* protocol, kept deliberately separate from
transport. Transport is the Arduino RouterBridge/RPClite/MsgPack link
between the MCU and Linux side (see communication.cpp and device.py).
Keeping `type`/`version` here means new message kinds (e.g. "ai") can
be added later without redesigning the communication layer.
"""

import json

PROTOCOL_VERSION = 1

VALID_TYPES = ("telemetry", "command", "event", "error", "ai")


def _envelope(msg_type, data):
    return {
        "type": msg_type,
        "version": PROTOCOL_VERSION,
        "data": data,
    }


def build_telemetry(data):
    """data: dict with motion/light/focus/sessionTime/idleTime/recommendation."""
    return _envelope("telemetry", data)


def build_command(command):
    return _envelope("command", {"command": command})


def build_event(event):
    return _envelope("event", {"event": event})


def build_error(code, message):
    return _envelope("error", {"code": code, "message": message})


def parse_message(line):
    """Parses and validates a raw JSON string against the envelope shape.

    Not used on the Bridge/RPC path (that path already exchanges native
    Python values via device.py). Kept for any future transport that
    carries the protocol as JSON text -- e.g. an external AI service
    posting a "type": "ai" message.
    """
    try:
        message = json.loads(line)
    except (json.JSONDecodeError, TypeError):
        return None

    if not isinstance(message, dict):
        return None

    if message.get("version") != PROTOCOL_VERSION:
        return None

    if message.get("type") not in VALID_TYPES:
        return None

    if not isinstance(message.get("data"), dict):
        return None

    return message
