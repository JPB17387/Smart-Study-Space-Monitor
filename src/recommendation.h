#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

#include <Arduino.h>
#include "session.h"

/**
 * @brief Initializes the Recommendation Provider layer.
 */
void initRecommendation();

/**
 * @brief Updates recommendation state based on session state and ambient light.
 *
 * @param state Current SessionState from Session module.
 * @param lightPercent Current ambient light percentage (0-100).
 */
void updateRecommendation(
    SessionState state,
    int lightPercent
);

/**
 * @brief Returns current Flash-resident recommendation text string.
 *
 * @return Pointer to Flash memory string (const __FlashStringHelper*).
 */
const __FlashStringHelper* getRecommendation();

/**
 * @brief Checks if recommendation content has changed since last call.
 *
 * @return true if recommendation changed, false otherwise.
 */
bool hasRecommendationChanged();

#endif // RECOMMENDATION_H
