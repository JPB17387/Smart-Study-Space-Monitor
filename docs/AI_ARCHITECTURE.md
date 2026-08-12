# AI Architecture

## Purpose

The AI recommendation system is a production enhancement layer for the Smart Study AI Platform. It augments the deterministic MCU recommendation with more natural, context-aware guidance when the local LLM Brick is available.

## Core principle

The MCU recommendation is always the baseline and fallback source of truth. The AI layer must never be the only source of a recommendation. If AI is unavailable, the dashboard still receives a valid recommendation from the MCU.

## Flow

MCU rule-based recommendation
        ↓
Python AI recommendation layer
        ↓
Local `LargeLanguageModel` Brick
        ↓
AI recommendation when available
        ↓
Fallback to rule-based recommendation on any error

## Implementation details

- `sketch/recommendation.cpp` produces the MCU recommendation text.
- `python/device.py` reads the MCU recommendation and converts it to an AI context payload.
- `python/ai_recommendation.py` wraps the local LLM Brick with:
  - background execution
  - throttling
  - caching
  - context gating
  - strict fallback behavior

## Background execution

AI inference runs in a separate background thread so it does not block the polling loop or the main telemetry path. This makes the dashboard responsive even when the LLM is slow or temporarily unavailable.

## Throttling and context gating

The provider avoids re-requesting AI on every telemetry update by:

- comparing the current context to the last request
- limiting requests to a minimum interval
- grouping study-time values into coarse buckets
- keeping the previous cached AI recommendation when the context has not meaningfully changed

## Cache behavior

The provider stores the last valid AI recommendation and reuses it until the context changes enough to justify a fresh inference. This reduces churn and keeps the dashboard stable.

## Fallback behavior

The provider returns a tuple of:

- recommendation text
- whether the text came from AI or rule logic
- AI status

Valid AI statuses are:

- `unavailable`
- `ready`
- `generating`
- `error`

If the model is unavailable or errors, the UI continues to show the reliable baseline recommendation.

## Failure behavior

AI failure is treated as a non-fatal condition. The dashboard stays operational, telemetry continues, commands still work, and the MCU remains authoritative. The AI layer is an enhancement, not a required dependency.

## Output safety

Recommendation output is sanitized before caching. This includes:

- trimming whitespace
- replacing code fences with spaces
- collapsing repeated whitespace
- limiting the maximum visible recommendation length to a safe dashboard-friendly size

This prevents the dashboard recommendation field from being dominated by unexpectedly long model output.
