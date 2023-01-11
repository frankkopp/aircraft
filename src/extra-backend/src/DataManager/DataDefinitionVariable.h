// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_DATADEFINITIONVARIABLE_H
#define FLYBYWIRE_A32NX_DATADEFINITIONVARIABLE_H

#include <vector>
#include <sstream>

#include "IDGenerator.h"

// TODO: Comment
class DataDefinitionVariable {
public:
  struct DataDefinition {
    std::string name;
    int index;
    ENUM unit;
  };

private:
  HANDLE hSimConnect;

  std::string name{};

  std::vector<DataDefinition> dataDefinitions;

  ID dataDefId = -1;

  ID requestId = -1;

  void* pDataStruct = nullptr;

  size_t structSize = 0;

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


public:
  DataDefinitionVariable() = delete;

  DataDefinitionVariable(
    HANDLE hSimConnect,
    std::string name,
    std::vector<DataDefinition> &dataDefinitions,
    ID dataDefinitionId,
    ID requestId,
    void* pDataStruct,
    size_t structSize,
    bool autoReading = false,
    bool autoWriting = false,
    FLOAT64 maxAgeTime = 0,
    UINT64 maxAgeTicks = 0);

  // TODO: Comment
  bool requestFromSim() const;

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
  [[nodiscard]]
  bool requestUpdateFromSim(FLOAT64 timeStamp, UINT64 tickCounter);

  bool updateFromSimObjectData(const SIMCONNECT_RECV_SIMOBJECT_DATA* data);

  // TODO: Comment
  void setToSim();

  /**
   * Writes the cached value to the sim if the dirty flag is set.
   * If the cached value has never been set this method does nothing.
   * This does not update the timeStampSimTime or tickStamp.
   */
  void updateToSim(FLOAT64 timeStamp, UINT64 tickCounter);

  // Getters and setters

  [[nodiscard]]
  const std::string &getName() const;

  [[nodiscard]]
  const std::vector<DataDefinition> &getDataDefinitions() const { return dataDefinitions; }

  [[nodiscard]]
  void* getPDataStruct() const { return pDataStruct; }

  [[nodiscard]]
  ID getDataDefID() const { return dataDefId; }

  [[nodiscard]]
  ID getRequestId() const { return requestId; }

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
  std::string str() const {
    std::stringstream os;
    os << "DataDefinition{ name='" << getName() << "'";
    os << " definitions=" << dataDefinitions.size();
    os << " ptrStruct=" << pDataStruct;
    os << " structSize=" << structSize;
    os << " autoRead=" << (isAutoRead() ? "autoR" : "manualR");
    os << " autoWrite=" << (isAutoWrite() ? "autoW" : "manualW");
    os << " maxAgeTime=" << getMaxAgeTime() << "ms";
    os << " maxAgesTicks=" << getMaxAgeTicks() << "ticks";
    os << " }";
    return os.str();
  }
};

/**
 * Overloaded operator to write the value of a CacheableVariable to an ostream
 * @param os
 * @param variable
 * @return the ostream
 */
inline std::ostream &operator<<(std::ostream &os, const DataDefinitionVariable &variable) {
  os << variable.str();
  return os;
}

#endif //FLYBYWIRE_A32NX_DATADEFINITIONVARIABLE_H
