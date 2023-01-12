// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_AIRCRAFTVARIABLE_H
#define FLYBYWIRE_A32NX_AIRCRAFTVARIABLE_H

#include <iostream>

#include "CacheableVariable.h"
#include "Event.h"

typedef std::shared_ptr<Event> EventPtr;

/**
 * Specialized class for aircraft cacheable variables (aka simvars or A:VARS).
 *
 * This class uses events or calculator code to write to a variable as
 * AircraftVariables are read-only.
 *
 * If no setter code or event is provided the variable will be read-only.
 */
class AircraftVariable : public CacheableVariable {
private:
  /**
   * the event used in the calculator code to write to the variable.
   */
  std::string setterEventName;

  /**
   * the event used to write to the variable.
   */
  EventPtr setterEvent{};

public:

  AircraftVariable() = delete; // no default constructor
  AircraftVariable(const AircraftVariable &) = delete; // no copy constructor
  AircraftVariable &operator=(const AircraftVariable &) = delete; // no copy assignment

  /**
   * Creates an instance of a writable aircraft variable.
   *
   * If a setter event name or event object is provided the variable will be writable.
   * (the reason both are given as there seem to be differences in how the sim handles
   * calculator code and events).
   *
   * @param varName The name of the variable in the sim.
   * @param varIndex The index of the variable in the sim.
   * @param unit The unit of the variable as per the sim. See Units.h
   * @param autoReading Used by external classes to determine if the variable should be updated
   * automatically from the sim.
   * @param autoWriting Used by external classes to determine if the variable should be written
   * back to the sim automatically.
   * @param maxAgeTime The maximum age of an auto updated the variable in seconds.
   * @param maxAgeTicks The maximum age of an auto updated the variable in sim ticks.
   * @param setterEventName The calculator code to write to the variable.
   */
  explicit AircraftVariable(
    const std::string &varName,
    int varIndex = 0,
    std::string setterEventName = "",
    EventPtr setterEvent = nullptr,
    Unit unit = UNITS.Number,
    bool autoReading = false,
    bool autoWriting = false,
    FLOAT64 maxAgeTime = 0.0,
    UINT64 maxAgeTicks = 0);

  FLOAT64 readFromSim() override;
  void writeToSim() override;

  void setAutoWrite(bool autoWriting) override;
  void set(FLOAT64 value) override;

private:
  void useEventSetter();
  void useCalculatorCodeSetter();
};

#endif //FLYBYWIRE_A32NX_AIRCRAFTVARIABLE_H
