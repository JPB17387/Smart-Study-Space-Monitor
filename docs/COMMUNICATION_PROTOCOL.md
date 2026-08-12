# Communication Protocol

## Overview

The project uses a versioned application envelope model to separate protocol semantics from the underlying transport. The active deployment uses RouterBridge / RPClite between the Arduino MCU and the App Lab Python host, and HTTP JSON messages between the host and the browser dashboard.

## Envelope format

```json
{
  "type": "telemetry",
  "version": 1,
  "data": {
    "motion": true,
    "light": 72,
    "focus": true,
    "sessionTime": 120,
    "idleTime": 2,
    "recommendation": "Good conditions for focused study."
  }
}
```

The general shape is:

```json
{
  "type": "telemetry|command|event|error",
  "version": 1,
  "data": { ... }
}
```

## Message types

### Telemetry

Represents the current state of the study environment.

Example fields:

- `motion`: boolean
- `light`: integer percent
- `focus`: boolean
- `sessionTime`: integer seconds
- `idleTime`: integer seconds
- `recommendation`: string
- `recommendationSource`: `ai` or `rule`
- `aiStatus`: `unavailable`, `ready`, `generating`, or `error`
- `stale`: boolean, when telemetry is reported from a last-known-good state

### Command

Represents a browser or host command that should be forwarded to the MCU.

The current valid command set is:

- `start_session`
- `pause_session`
- `resume_session`
- `stop_session`
- `start_break`

### Event

Represents a higher-level state transition or dispatch acknowledgement.

Example:

```json
{
  "type": "event",
  "version": 1,
  "data": {
    "event": "start_session_ack"
  }
}
```

### Error

Represents a transport, validation, or operational failure.

Example:

```json
{
  "type": "error",
  "version": 1,
  "data": {
    "code": "BRIDGE_UNAVAILABLE",
    "message": "Could not reach the UNO Q microcontroller"
  }
}
```

## Versioning

The protocol version is intentionally explicit. The current schema is version `1` and is carried in the envelope as `version`. This allows future compatibility and keeps the transport layer simple.

## Error behavior

- Invalid commands return an `error` envelope.
- Bridge failures return an `error` envelope.
- Stale telemetry is flagged instead of silently disappearing.
- AI failures do not block the system; they surface as `aiStatus` and continue with the fallback recommendation.

## Extensibility

The protocol is structured so future message kinds can be added without breaking the current contract. Planned future expansion may include:

- AI-specific payloads
- diagnostics payloads
- richer notification events
- health and status messages

## Command whitelist

The host validates commands before forwarding them to the MCU. This preserves the safety model and prevents arbitrary command execution outside the project’s own command set.

## Security model

The current deployment assumes a local or trusted network environment. This is deliberate and consistent with the project’s embedded, local-device architecture. The protocol itself is not intended for public Internet exposure without additional controls.
