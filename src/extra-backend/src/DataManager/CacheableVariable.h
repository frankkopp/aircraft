// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

//
// Created by frank on 09.01.2023.
//

#ifndef FLYBYWIRE_A32NX_CACHEABLEVARIABLE_H
#define FLYBYWIRE_A32NX_CACHEABLEVARIABLE_H

#include <chrono>
#include <optional>

#include <MSFS/Legacy/gauges.h>
#include <MSFS/MSFS.h>
#include <MSFS/MSFS_Render.h>
#include <SimConnect.h>

#include "Units.h"

using namespace std::chrono_literals;

class CacheableVariable {
protected:
  const std::string nameInSim{};

  int index = 0;

  ENUM unit{};

  bool autoUpdate = false;

  std::chrono::duration<int64_t, std::milli> maxAgeTime = 0ms;

  int64_t maxAgeTicks = 0;

  std::optional<FLOAT64> cachedValue{};

  bool dirty = false;

  ID dataID = 0;

public:
  // clang-format off
  explicit CacheableVariable(
    std::string nameInSim,
    int index = 0,
    ENUM unit = UNITS.Number,
    bool autoUpdate = false,
    const std::chrono::duration<int64_t, std::milli> &maxAgeTime = 0ms,
    int64_t maxAgeTicks = 0);
  // clang-format on

  [[nodiscard]] FLOAT64 get() const;

  void set(FLOAT64 value);

  virtual FLOAT64 getFromSim() = 0;

  virtual void setToSim(FLOAT64 value) = 0;

  virtual void writeToSim() = 0;

private:
  // Getters and Setters
public:
  [[nodiscard]]
  const std::string &getNameInSim() const { return nameInSim; }

  [[nodiscard]]
  ENUM getUnit() const { return unit; }

  [[nodiscard]]
  int getIndex() const { return index; }

  [[nodiscard]]
  bool isAutoUpdate() const { return autoUpdate; }

  void setAutoUpdate(bool autoUpdating) { autoUpdate = autoUpdating; }

  [[nodiscard]]
  const std::chrono::duration<int64_t, std::milli> &
  getMaxAgeTime() const { return maxAgeTime; }

  void setMaxAgeTime(std::chrono::duration<int64_t, std::milli> maxAgeTimeInMilliseconds) {
    maxAgeTime = maxAgeTimeInMilliseconds;
  }

  [[nodiscard]]
  int64_t getMaxAgeTicks() const { return maxAgeTicks; }

  void setMaxAgeTicks(int64_t maxAgeTicks) { maxAgeTicks = maxAgeTicks; }

  [[nodiscard]]
  bool isStoredToSim() const { return !dirty; }
};

inline std::ostream &operator<<(std::ostream &os, const CacheableVariable &variable) {
  os << "Variable{ name=\"";
  if (variable.getIndex() == 0) {
    os << variable.getNameInSim();
  }
  else {
    os << variable.getNameInSim() << "[" << variable.getIndex() << "]";
  }
  os << "\" value=" << variable.get();
  os << " dirty=" << (variable.isStoredToSim() ? "false" : "true");
  os << " unit=\"" << UNITS.unitStrings[variable.getUnit()] << "\"";
  os << " auto=" << (variable.isAutoUpdate() ? "auto" : "manual");
  os << " maxAgeTime=" << variable.getMaxAgeTime().count() << "ms";
  os << " maxAgesTicks=" << variable.getMaxAgeTicks() << "ticks";
  os << " }";

  return os;
}

#endif // FLYBYWIRE_A32NX_CACHEABLEVARIABLE_H
