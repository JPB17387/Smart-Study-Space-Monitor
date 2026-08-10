#include "recommendation.h"

//==================================================
// Local Light Thresholds
//==================================================

static const int LIGHT_THRESHOLD_GOOD   = 60;
static const int LIGHT_THRESHOLD_MEDIUM = 30;

//==================================================
// Flash-Resident Recommendation Strings (PROGMEM)
//==================================================

static const char REC_FOCUS_GOOD[]   PROGMEM = "Great focus! Keep studying.";
static const char REC_FOCUS_MEDIUM[] PROGMEM = "Lighting is acceptable.";
static const char REC_FOCUS_LOW[]    PROGMEM = "Increase lighting for better focus.";
static const char REC_IDLE[]         PROGMEM = "Motion not detected. Resume when ready.";
static const char REC_BREAK[]        PROGMEM = "Take a short break and recharge.";

//==================================================
// Private Module State
//==================================================

static const __FlashStringHelper* currentRecommendation = (const __FlashStringHelper*)REC_FOCUS_GOOD;
static bool recommendationChanged = true;

/**
 * @brief Initializes the Recommendation Provider state.
 */
void initRecommendation()
{
    currentRecommendation = (const __FlashStringHelper*)REC_FOCUS_GOOD;
    recommendationChanged = true;
}

/**
 * @brief Evaluates session state and light level to select appropriate recommendation.
 *
 * Exposes lightweight rule-based logic stored in Flash memory. Retains change flag
 * for display update optimization.
 *
 * @param state Current session state from session module.
 * @param lightPercent Current ambient light percentage.
 */
void updateRecommendation(
    SessionState state,
    int lightPercent)
{
    const __FlashStringHelper* newRec = NULL;

    switch (state)
    {
        case SESSION_FOCUS:
            if (lightPercent >= LIGHT_THRESHOLD_GOOD)
            {
                newRec = (const __FlashStringHelper*)REC_FOCUS_GOOD;
            }
            else if (lightPercent >= LIGHT_THRESHOLD_MEDIUM)
            {
                newRec = (const __FlashStringHelper*)REC_FOCUS_MEDIUM;
            }
            else
            {
                newRec = (const __FlashStringHelper*)REC_FOCUS_LOW;
            }
            break;

        case SESSION_IDLE:
            newRec = (const __FlashStringHelper*)REC_IDLE;
            break;

        case SESSION_BREAK:
            newRec = (const __FlashStringHelper*)REC_BREAK;
            break;

        default:
            newRec = (const __FlashStringHelper*)REC_FOCUS_GOOD;
            break;
    }

    if (currentRecommendation != newRec)
    {
        currentRecommendation = newRec;
        recommendationChanged = true;
    }
}

/**
 * @brief Returns active recommendation string from Flash memory.
 *
 * @return const __FlashStringHelper* Pointer to Flash string.
 */
const __FlashStringHelper* getRecommendation()
{
    return currentRecommendation;
}

/**
 * @brief Returns whether recommendation text changed and clears flag.
 *
 * @return true if updated since last check, false otherwise.
 */
bool hasRecommendationChanged()
{
    bool changed = recommendationChanged;
    recommendationChanged = false;
    return changed;
}
