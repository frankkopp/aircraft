// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_DATADEFINITIONVARIABLE_H
#define FLYBYWIRE_A32NX_DATADEFINITIONVARIABLE_H

#include <vector>
#include <sstream>

#include "IDGenerator.h"

/**
 * A class that represents a data definition variable (custom sim object).<br/>
 *
 * Data definition variables are used to define a sim data objects that can be used to retrieve and
 * write data from and to the sim.<br/>
 *
 * For this a memory area needs to be reserved e.g. via a data struct instance. This data struct
 * instance and its size (sizeof) need to be passed to the constructor of this class.
 *
 * Usage in three steps:<br/>
 * 1. a vector of data definitions will be registered with the sim as data definitions (provided in
 *    the constructor)<br/>
 * 2. a data request will be send to the sim to have the sim prepare the requested data<br/>
 * 3. the sim will send an message (SIMCONNECT_RECV_ID_SIMOBJECT_DATA) to signal that the data is
 *    ready to be read. This event also contains a pointer to the provided data. <br/>
 *
 * The DataManager class will provide the requestData() method to read the sim's message queue.
 * Currently SIMCONNECT_PERIOD is not used and data is requested on demand via the DataManager.
 */
class DataDefinitionVariable {
public:

  /**
   * DataDefinition to be used to register a data definition with the sim. <p/>
   * name: the name of the variable <br/>
   * index: the index of the variable <br/>
   * unit: the unit of the variable <br/>
   */
  struct DataDefinition {
    std::string name;
    int index;
    Unit unit;
  };

private:
  /**
   * SimConnect handle is required for data definitions.
   */
  HANDLE hSimConnect;

  /**
   * Arbitrary name for the data definition variable for debugging purposes
   */
  std::string name{};

  /**
   * List of data definitions to add to the sim object data
   * Used for "SimConnect_AddToDataDefinition"
   */
  std::vector<DataDefinition> dataDefinitions;

  /**
   * Each data definition variable has its own unique id so the sim can map registered data sim
   * objects to data definitions.
   */
  DWORD dataDefId = 0;

  /**
   * Each request for sim object data requires a unique id so the sim can provide the request ID
   * in the response (message SIMCONNECT_RECV_ID_SIMOBJECT_DATA).
   */
  DWORD requestId = 0;

  /**
   * Pointer to the data struct that will be used to store the data from the sim.
   */
  void* pDataStruct = nullptr;

  /**
   * Size of the data struct that will be used to store the data from the sim.
   *
   */
  size_t structSize = 0;

  /**
  * Used by external classes to determine if the variable should updated from the sim when
  * a sim update call occurs. <br/>
  * E.g. if autoRead is true the variable will be updated from the sim every time the
  * DataManager::preUpdate() method is called
  */
  bool autoRead = false;

  /**
   * Used by external classes to determine if the variable should written to the sim when
   * a sim update call occurs. <br/>
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

  DataDefinitionVariable() = delete; // no default constructor
  DataDefinitionVariable(const DataDefinitionVariable&) = delete; // no copy constructor
  DataDefinitionVariable& operator=(const DataDefinitionVariable&) = delete; // no copy assignment
  ~DataDefinitionVariable() = default;

  /**
   * Creates a new instance of a DataDefinitionVariable.
   * @param hSimConnect Handle to the SimConnect object.
   * @param name Arbitrary name for the data definition variable for debugging purposes
   * @param dataDefinitions List of data definitions to add to the sim object data
   * @param dataDefinitionId Each data definition variable has its own unique id so the sim can map registered data sim objects to data definitions.
   * @param requestId Each request for sim object data requires a unique id so the sim can provide the request ID in the response (message SIMCONNECT_RECV_ID_SIMOBJECT_DATA).
   * @param pDataStruct Pointer to the data struct that will be used to store the data from the sim.
   * @param structSize Size of the data struct that will be used to store the data from the sim.
   * @param autoReading Used by external classes to determine if the variable should updated from the sim when a sim update call occurs.
   * @param autoWriting Used by external classes to determine if the variable should written to the sim when a sim update call occurs.
   * @param maxAgeTime The maximum age of the value in sim time before it is updated from the sim by the requestUpdateFromSim() method.
   * @param maxAgeTicks The maximum age of the value in ticks before it is updated from the sim by the requestUpdateFromSim() method.
   */
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

  /**
   * Sends a data request to the sim to have the sim prepare the requested data.
   * @return true if the request was successful, false otherwise
   * @See SimConnect_RequestDataOnSimObject
   */
  [[nodiscard]]
  bool requestFromSim() const;

  /**
   * Checks the age (time/ticks) of the data and requests an update from the sim if the data is too old.
   * @param timeStamp the current sim time (taken from the sim update event)
   * @param tickCounter the current tick counter (taken from a custom counter at each update event
   * @return false if the request was not successful, true otherwise
   *         (also true when max age is not exceeded - no request will be sent to the sim in this case
   */
  [[nodiscard]]
  bool requestUpdateFromSim(FLOAT64 timeStamp, UINT64 tickCounter);

  /**
   * Called by the DataManager when a SIMCONNECT_RECV_ID_SIMOBJECT_DATA message for this
   * variables request ID is received.
   * @param pointer to the SIMCONNECT_RECV_SIMOBJECT_DATA structure
   * @See SIMCONNECT_RECV_SIMOBJECT_DATA
   */
  void updateFromSimObjectData(const SIMCONNECT_RECV_SIMOBJECT_DATA* pData);

  /**
   * Writes the data to the sim without updating the time stamps for time and ticks.
   * @return true if the write was successful, false otherwise
   */
  bool writeToSim();

  /**
   * Writes the data to the sim and updates the time stamps for time and ticks.
   * @param timeStamp the current sim time (taken from the sim update event)
   * @param tickCounter the current tick counter (taken from a custom counter at each update event)
   * @return true if the write was successful, false otherwise
   */
  bool updateToSim(FLOAT64 timeStamp, UINT64 tickCounter);

  // Getters and setters

  [[nodiscard]]
  const std::string &getName() const { return name; }

  [[maybe_unused]] [[nodiscard]]
  const std::vector<DataDefinition> &getDataDefinitions() const { return dataDefinitions; }

  [[maybe_unused]] [[nodiscard]]
  void* getPDataStruct() const { return pDataStruct; }

  [[maybe_unused]] [[nodiscard]]
  DWORD getDataDefID() const { return dataDefId; }

  [[nodiscard]]
  DWORD getRequestId() const { return requestId; }

  [[nodiscard]]
  bool isAutoRead() const { return autoRead; }

  [[maybe_unused]]
  void setAutoRead(bool autoReading) { autoRead = autoReading; }

  [[nodiscard]]
  bool isAutoWrite() const { return autoWrite; }

  [[maybe_unused]]
  void setAutoWrite(bool autoWriting) { autoRead = autoWriting; }

  [[maybe_unused]] [[nodiscard]]
  FLOAT64 getTimeStamp() const { return timeStampSimTime; }

  [[nodiscard]]
  FLOAT64 getMaxAgeTime() const { return maxAgeTime; }

  [[maybe_unused]]
  void setMaxAgeTime(FLOAT64 maxAgeTimeInMilliseconds) { maxAgeTime = maxAgeTimeInMilliseconds; }

  [[maybe_unused]] [[nodiscard]]
  UINT64 getTickStamp() const { return tickStamp; }

  [[nodiscard]]
  UINT64 getMaxAgeTicks() const { return maxAgeTicks; }

  [[maybe_unused]]
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
