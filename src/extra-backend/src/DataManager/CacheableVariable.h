// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_CACHEABLEVARIABLE_H
#define FLYBYWIRE_A32NX_CACHEABLEVARIABLE_H

#include <optional>
#include <string>
#include <sstream>
#include <utility>

#include <MSFS/Legacy/gauges.h>

#include "Units.h"

/**
 * Virtual base class for sim variable like named variables, aircraft variables and DataDefinitions.
 * Specialized classes must implement the requestFromSim and setToSim methods.
 */
class CacheableVariable {
protected:
  /**
   * The name of the variable in the sim
   * E.g. "A32NX_IS_READY" or "SIM ON GROUND"
   */
  const std::string varName{};

  /**
   * The index of an indexed sim variable
   */
  int index = 0;

  /**
   * The unit ENUM of the variable as per the sim
   * @See Units.h
   * @See https://docs.flightsimulator.com/html/Programming_Tools/SimVars/Simulation_Variable_Units.htm
   */
  ENUM unit{};

  /**
   * Used by external classes to determine if the variable should updated from the sim when
   * a sim update call occurs.
   * E.g. if autoRead is true the variable will be updated from the sim every time the
   * DataManager::preUpdate() method is called
   */
  bool autoRead = false;

  /**
   * Used by external classes to determine if the variable should written tothe sim when
   * a sim update call occurs.
   * E.g. if autoWrite is true the variable will be updated from the sim every time the
   * DataManager::postUpdate() method is called
   */
  bool autoWrite = false;

  /**
   * The time stamp of the last update from the sim
   */
  FLOAT64 timeStampSimTime{};

  /**
   * The maximum age of the value in sim time before it is updated from the sim by the
   * requestUpdateFromSim() method.
   */
  FLOAT64 maxAgeTime = 0;

  /**
   * The tick counter of the last update from the sim
   */
  UINT64 tickStamp = 0;

  /**
   * The maximum age of the value in ticks before it is updated from the sim by the
   */
  UINT64 maxAgeTicks = 0;

  /**
   * The value of the variable as it was last read from the sim or updated by the
   * set() method. If the variable has not been read from the sim yet and has never been set
   * this value will be FLOAT64 default value.
   * Prints an error to std::cerr if the cache is empty.
   * (MSFS does not allow exceptions)
   */
  std::optional<FLOAT64> cachedValue{};

  /**
   * Flag to indicate if the variable has been changed since the last read from the sim.
   */
  bool dirty = false;

  /**
   * The sim's data ID for the variable or the data definition id for a data definition variable.
   */
  ID dataID = -1;

public:

  virtual ~CacheableVariable() = default;

  /**
   * Constructor
   * @param varName The name of the variable in the sim
   * @param varIndex The index of an indexed sim variable
   * @param unit The unit ENUM of the variable as per the sim
   * @param autoReading Used by external classes to determine if the variable should be automatically updated from the sim
   * @param autoWriting Used by external classes to determine if the variable should be automatically written to the sim
   * @param maxAgeTime The maximum age of the variable in seconds when using requestUpdateFromSim()
   * @param maxAgeTicks The maximum age of the variable in ticks when using updateToSim()
   */
  explicit CacheableVariable(
    std::string nameInSim,
    int index,
    ENUM unit,
    bool autoReading,
    bool autoWriting,
    FLOAT64 maxAgeTime,
    UINT64 maxAgeTicks)
    : varName(std::move(nameInSim)),
      index(index),
      unit(unit),
      autoRead(autoReading),
      autoWrite(autoWriting),
      maxAgeTime(maxAgeTime),
      maxAgeTicks(maxAgeTicks) {}


  /**
   * Returns the cached value or the default value (FLOAT64{}) if the cache is empty.
   * Prints an error to std::cerr if the cache is empty.
   * If the value has been set by the set() method since the last read from the sim (is dirty)
   * but has not been written to the sim yet an error message is printed to std::cerr.
   * (MSFS does not allow exceptions)
   * @return cached value or default value
   */
  [[nodiscard]]
  FLOAT64 get() const;

  /**
   * Reads the value from the sim and updates the cache (clears dirty flag).
   * This does not update the timeStampSimTime or tickStamp.
   * Must be implemented by specialized classes. This method is called by the
   * requestUpdateFromSim() method.
   * @return the value read from the sim
   */
  virtual FLOAT64 getFromSim() = 0;

  /**
   * Reads the value fom the sim if the cached value is older than the max age (time or ticks).
   * It updates the cache (clears dirty flag) and the timeStampSimTime and tickStamp.
   * If the value has been set by the set() method since the last read from the sim (is dirty)
   * but has not been written to the sim yet an error message is printed to std::cerr.
   * (MSFS does not allow exceptions)
   * @param timeStamp the current sim time (taken from the sim update event)
   * @param tickCounter the current tick counter (taken from a custom counter at each update event
   * @return the value read from the sim
   */
  FLOAT64 updateFromSim(FLOAT64 timeStamp, UINT64 tickCounter);

  /**
   * Sets the cache value and marks the variable as dirty.
   * Does not write the value to the sim or update the time and tick stamps.
   * @param value the value to set
   */
  virtual void set(FLOAT64 value);

  /**
   * Writes the current value to the sim.
   * Clears the dirty flag.
   * Must be implemented by specialized classes.
   * This method is called by the updateToSim() and setToSim(FLOAT64 v) methods.
   */
  virtual void setToSim() = 0;

  /**
   * Writes the given value to the cache and the sim.
   * Clears the dirty flag.
   * @param value The value to set the variable to.
   */
  virtual void setToSim(FLOAT64 value);

  /**
   * Writes the cached value to the sim if the dirty flag is set.
   * If the cached value has never been set this method does nothing.
   * This does not update the timeStampSimTime or tickStamp.
   */
  void updateToSim();

  /**
   * Returns a string representing the object
   * @return object as string
   */
  [[nodiscard]]
  std::string str() const {
    std::stringstream os;
    os << "Variable{ name='" << getVarName() << "'";
    os << " index=" << getIndex();
    os << " value=" << get();
    os << " dirty=" << (isStoredToSim() ? "false" : "true");
    os << " unit=\"" << UNITS.unitStrings[getUnit()] << "\"";
    os << " autoRead=" << (isAutoRead() ? "autoR" : "manualR");
    os << " autoWrite=" << (isAutoWrite() ? "autoW" : "manualW");
    os << " maxAgeTime=" << getMaxAgeTime() << "ms";
    os << " maxAgesTicks=" << getMaxAgeTicks() << "ticks";
    os << " }";
    return os.str();
  };

private:

// Getters and Setters
public:
  [[nodiscard]]
  const std::string &getVarName() const { return varName; }

  [[nodiscard]]
  ENUM getUnit() const { return unit; }

  [[nodiscard]]
  int getIndex() const { return index; }

  [[nodiscard]]
  bool isAutoRead() const { return autoRead; }

  void setAutoRead(bool autoReading) { autoRead = autoReading; }

  [[nodiscard]]
  bool isAutoWrite() const { return autoWrite; }

  virtual void setAutoWrite(bool autoWriting) { autoRead = autoWriting; }

  [[nodiscard]]
  FLOAT64 getTimeStamp() const { return timeStampSimTime; }

  [[nodiscard]]
  FLOAT64 getMaxAgeTime() const { return maxAgeTime; }

  void setMaxAgeTime(FLOAT64 maxAgeTimeInMilliseconds) { maxAgeTime = maxAgeTimeInMilliseconds; }

  [[nodiscard]]
  UINT64 getTickStamp() const { return tickStamp; }

  [[nodiscard]]
  UINT64 getMaxAgeTicks() const { return maxAgeTicks; }

  void setMaxAgeTicks(int64_t newMaxAgeTicks) { maxAgeTicks = newMaxAgeTicks; }

  [[nodiscard]]
  bool isStoredToSim() const { return !dirty; }

  [[nodiscard]]
  bool getAsBool() const  { return static_cast<bool>(get()); }

  [[nodiscard]]
  INT64 getAsInt64() const  { return static_cast<INT64>(get()); }

  [[nodiscard]]
  void setAsBool(bool b)  { set(b ? 1.0 : 0.0); }

  [[nodiscard]]
  void setAsInt64(UINT64 i) { set(static_cast<FLOAT64>(i)); }
};

/**
 * Overloaded operator to write the value of a CacheableVariable to an ostream
 * @param os
 * @param variable
 * @return the ostream
 */
inline std::ostream &operator<<(std::ostream &os, const CacheableVariable &variable) {
  os << variable.str();
  return os;
}

#endif // FLYBYWIRE_A32NX_CACHEABLEVARIABLE_H
