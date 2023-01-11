// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_NAMEDVARIABLE_H
#define FLYBYWIRE_A32NX_NAMEDVARIABLE_H

#include <string>
#include <SimConnect.h>

#include "CacheableVariable.h"

/**
 * Specialized class for named variables (LVARS)
 */
class NamedVariable: public CacheableVariable {

public:
  /**
   * Creates an instance of a named variable.
   * If the variable is not found in the sim it will be created.
   * @param varName The name of the variable in the sim.
   * @param unit The unit ENUM of the variable as per the sim.
   * @param autoReading Used by external classes to determine if the variable should be updated
   * automatically from the sim.
   * @param autoWriting Used by external classes to determine if the variable should be written
   * back to the sim automatically.
   * @param maxAgeTime The maximum age of an auto updated the variable in seconds.
   * @param maxAgeTicks The maximum age of an auto updated the variable in sim ticks.
   */
  explicit NamedVariable(
    const std::string& varName,
    ENUM unit = UNITS.Number,
    bool autoReading = false,
    bool autoWriting = false,
    FLOAT64 maxAgeTime = 0.0,
    UINT64 maxAgeTicks = 0);

  FLOAT64 getFromSim() override;

  void setToSim() override;
};

#endif // FLYBYWIRE_A32NX_NAMEDVARIABLE_H
