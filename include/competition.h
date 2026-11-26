#pragma once

#include <Arduino.h>
#include "beacon_config.h"

void competitionInit();
void competitionLoop();
void competitionFlushPendingEvents();
String competitionNormalizeTag(const uint8_t *uid, uint8_t uidLength);
bool competitionTagIsAllowed(const String &tag);
bool competitionPublishValidatedTag(const String &tag, const String &player,
                                    const String &nodeId, uint32_t eventId);
