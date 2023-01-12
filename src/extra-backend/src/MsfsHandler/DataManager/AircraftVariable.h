// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_AIRCRAFTVARIABLE_H
#define FLYBYWIRE_A32NX_AIRCRAFTVARIABLE_H

#include <iostream>

#include "CacheableVariable.h"
#include "Event.h"

typedef std::shared_ptr<Event> EventPtr;

/**
 * This class uses events or calculator code to write to a variable as
 * AircraftVariables are read-only
 * TODO: add event as an alternative to calculator code
 * TODO: updates all doc comments
 * TODO: comment that a event needs to have the order index/data to be able to be used
 */
class AircraftVariable : public CacheableVariable {
private:
  std::string setterEventName;
  EventPtr setterEvent{};

public:

  AircraftVariable() = delete; // no default constructor
  AircraftVariable(const AircraftVariable &) = delete; // no copy constructor
  AircraftVariable &operator=(const AircraftVariable &) = delete; // no copy assignment

  /**
   * Creates an instance of a writable aircraft variable.
   * @param varName The name of the variable in the sim.
   * @param unit The unit ENUM of the variable as per the sim.
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


  void setAutoWrite(bool autoWriting) override {
    if (setterEventName.empty() && setterEvent == nullptr) {
      std::cerr << "AircraftVariable::setAutoWrite() called on [" << varName
                << "] but no setter event name is set" << std::endl;
      return;
    }
    CacheableVariable::setAutoWrite(autoWriting);
  };

  void set(FLOAT64 value) override {
    if (setterEventName.empty() && setterEvent == nullptr) {
      std::cerr << "AircraftVariable::set() called on [" << varName
                << "] but no setter event name is set" << std::endl;
      return;
    }
    CacheableVariable::set(value);
  };

private:
  void useEventSetter();
  void useCalculatorCodeSetter();
};

#endif //FLYBYWIRE_A32NX_AIRCRAFTVARIABLE_H
