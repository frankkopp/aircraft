// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

//
// Created by frank on 09.01.2023.
//

#ifndef FLYBYWIRE_A32NX_CACHEABLEVARIABLE_H
#define FLYBYWIRE_A32NX_CACHEABLEVARIABLE_H

#include <chrono>

#include "Variable.h"

using namespace std::chrono_literals;

class CacheableVariable : public Variable {
private:
  bool autoUpdate;
  std::chrono::duration<int64_t, std::milli> maxAgeTime = 0ms;
  int64_t maxAgeTicks;

  std::optional<FLOAT64> cachedValue{};
  bool dirty = false;

public:
  CacheableVariable() = delete;
  CacheableVariable(const std::string &nameInSim, int index = 0, ENUM unit = UNITS.Number, bool autoUpdate = false,
                    const std::chrono::duration<int64_t, std::milli> &maxAgeTime = 0ms, int64_t maxAgeTicks = 0);

  FLOAT64 get();
  void set(FLOAT64 value);

private:

  // Getters and Setters
public:
  [[nodiscard]]
  bool isAutoUpdate() const { return autoUpdate; }
  void setAutoUpdate(bool autoUpdate) { this->autoUpdate = autoUpdate; }

  [[nodiscard]]
  const std::chrono::duration<int64_t, std::milli> &getMaxAgeTime() const { return maxAgeTime; }
  void setMaxAgeTime(std::chrono::duration<int64_t, std::milli> maxAgeTimeInMilliseconds) {
    maxAgeTime = maxAgeTimeInMilliseconds;
  }

  [[nodiscard]]
  int64_t getMaxAgeTicks() const { return maxAgeTicks; }
  void setMaxAgeTicks(int64_t maxAgeTicks) { maxAgeTicks = maxAgeTicks; }


};

#endif // FLYBYWIRE_A32NX_CACHEABLEVARIABLE_H
