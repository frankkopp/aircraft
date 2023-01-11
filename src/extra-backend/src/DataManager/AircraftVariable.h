// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_AIRCRAFTVARIABLE_H
#define FLYBYWIRE_A32NX_AIRCRAFTVARIABLE_H

#include <iostream>

#include "CacheableVariable.h"

/**
 * Specialized class for aircraft variables.
 * Aircraft variables can't be written via the SDK.
 * Use a DataDefinition variable instead.
 *
 * AircraftVariable can't by copy constructed or assigned. They can be moved.
 * Create a AircraftVariable instance instead.
 *
 * @see CacheableVariable
 */
class AircraftVariable : public CacheableVariable {

public:

  AircraftVariable() = delete; // no default constructor
  AircraftVariable(const AircraftVariable&) = delete; // no copy constructor
  AircraftVariable& operator=(const AircraftVariable&) = delete; // no copy assignment

  /**
   * Creates an instance of an aircraft variable.
   * @param varName The name of the variable in the sim
   * @param varIndex The index of an indexed variable in the sim
   * @param unit The unit ENUM of the variable as per the sim.
   * @param autoReading Used by external classes to determine if the variable should updated
   * automatically from the sim
   * @param maxAgeTime The maximum age of an auto updated the variable in seconds.
   * @param maxAgeTicks The maximum age of an auto updated the variable in sim ticks.
   *
   * @see Units.h
   */
  explicit AircraftVariable(
    const std::string& varName,
    int varIndex = 0,
    ENUM unit = UNITS.Number,
    bool autoReading = false,
    FLOAT64 maxAgeTime = 0.0,
    UINT64 maxAgeTicks = 0);

  FLOAT64 getFromSim() override;

  /**
   * Aircraft variables cannot be set outside a data definition.
   * This method prints an error to std::cerr but is otherwise a no-op.
   * (MSFS does not allow exceptions)
   */
  void setAutoWrite(bool autoWriting) override {
    std::cerr << "Aircraft variable " << varName << " cannot be set outside a data definition" << std::endl;
  };

  /**
   * Aircraft variables cannot be set outside a data definition.
   * This method prints an error to std::cerr but is otherwise a no-op.
   * (MSFS does not allow exceptions)
   */
  void set(FLOAT64 value) override {
    std::cerr << "Aircraft variable " << varName << " cannot be set outside a data definition" << std::endl;
  };

  /**
   * Aircraft variables cannot be set outside a data definition.
   * This method prints an error to std::cerr but is otherwise a no-op.
   * (MSFS does not allow exceptions)
   */
  void setToSim() override {
    std::cerr << "Aircraft variable " << varName << " cannot be set outside a data definition" << std::endl;
  };
};


#endif //FLYBYWIRE_A32NX_AIRCRAFTVARIABLE_H
