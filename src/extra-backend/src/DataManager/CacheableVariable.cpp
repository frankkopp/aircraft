// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>

#include "CacheableVariable.h"

CacheableVariable::CacheableVariable(const std::string &nameInSim, int index, ENUM unit, bool autoUpdate,
                                     const std::chrono::duration<int64_t, std::milli> &maxAgeTime, int64_t maxAgeTicks)
    : Variable(nameInSim, index, unit), autoUpdate(autoUpdate), maxAgeTime(maxAgeTime), maxAgeTicks(maxAgeTicks) {}

FLOAT64 CacheableVariable::get() {
  if (cachedValue.has_value()) {
    return cachedValue.value();
  }
  std::cerr << "CacheableVariable::get() called on \"" << nameInSim << "\" but no value is cached" << std::endl;
  return FLOAT64{};
}

void CacheableVariable::set(FLOAT64 value) {
  cachedValue = value;
  dirty = true;
}

