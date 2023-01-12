// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_DATAMANAGER_H
#define FLYBYWIRE_A32NX_DATAMANAGER_H

#include <vector>
#include <memory>
#include <map>
#include <string>

#include <MSFS/Legacy/gauges.h>
#include <SimConnect.h>

#include "Units.h"
#include "NamedVariable.h"
#include "AircraftVariable.h"
#include "AircraftVariable.h"
#include "DataDefinitionVariable.h"
#include "Event.h"

typedef std::shared_ptr<NamedVariable> NamedVariablePtr;
typedef std::shared_ptr<AircraftVariable> AircraftVariablePtr;
typedef std::shared_ptr<DataDefinitionVariable> DataDefinitionVariablePtr;
typedef std::shared_ptr<Event> EventPtr;

/**
 * DataManager is responsible for managing all variables and events.
 * It is used to register variables and events and to update them.
 * It de-duplicates variables and events and only creates one instance of each if multiple modules
 * use the same variable.
 * It is still possible to use the SDK and Simconnect directly but it is recommended to use the
 * DataManager instead as the data manager is able to de-duplicate variables and events and automatically
 * update and write back variables from/to the sim.
 *
 * TODO
 *  - write DataDefinitionVariable to sim
 *  - de duplication
 *  - add support for events
 *  - add register methods for variables (currently only factory methods register variables
 *  - add ClientDataArea Variable
 *  - maybe rename classes DataDefinitionVariable to SimObject or similar
 */
class DataManager {
private:
  /**
   * A map of all registered variables.
   */
  std::map<std::string, std::shared_ptr<CacheableVariable>> variables{};

  /**
   * A vector of all registered data definitions.
   */
  std::vector<std::shared_ptr<DataDefinitionVariable>> dataDefinitionVariables{};

  /**
   * A vector of all registered events
   */
  std::map<std::string, std::shared_ptr<Event>> events{};

  /**
   * Handle to the simconnect instance.
   */
  HANDLE hSimConnect{};

  /**
   * Flag to indicate if the data manager is initialized.
   */
  bool isInitialized = false;

  /**
   * Counter for ticks to determine the age of variables.
   */
  UINT64 tickCounter{};

  /**
   * timeStamp for this update cycle to determine the age of variables.
   */
  FLOAT64 timeStamp{};

  /**
   * Instance of an IDGenerator to generate unique IDs for variables and events.
   */
  IDGenerator idGenerator{};

public:
  /**
   * Creates an instance of the DataManager.
   */
  DataManager();

  /**
   * Initializes the data manager.
   * This method must be called before any other method of the data manager.
   * Usually called in the MsfsHandler initialization.
   */
  bool initialize(HANDLE hdl);

  /**
   * Called by the MsfsHandler update() method.
   * Updates all variables marked for automatic reading.
   * @param pData Pointer to the data structure of gauge pre-draw event
   * @return true if successful, false otherwise
   */
  bool preUpdate(sGaugeDrawData* pData);

  /**
 * Called by the MsfsHandler update() method.
 * @param pData Pointer to the data structure of gauge pre-draw event
 * @return true if successful, false otherwise
 */
  bool update(sGaugeDrawData* pData);

  /**
 * Called by the MsfsHandler update() method.
 * Writes all variables marked for automatic writing back to the sim.
 * @param pData Pointer to the data structure of gauge pre-draw event
 * @return true if successful, false otherwise
 */
  bool postUpdate(sGaugeDrawData* pData);

  /**
   * Callback for SimConnect_GetNextDispatch events in the MsfsHandler used for data definition
   * variable batches.
   * Must map data request IDs to know IDs of data definition variables and return true if the
   * requestId has been handled.
   * @param pData Pointer to the data structure of gauge pre-draw event
   * @return true if request ID has been processed, false otherwise
   */
  bool processSimObjectData(const SIMCONNECT_RECV_SIMOBJECT_DATA* pData);

  /**
   * Called by the MsfsHandler shutdown() method.
   * Can be used for any extra cleanup.
   * @return true if successful, false otherwise
   */
  bool shutdown();

  /**
   * Must be called to retrieve requested sim object data (data definition variables) from the sim.
   * Will be called everytime preUpdate() is called.
   * Request data by calling DataDefinitions::requestFromSim() on the data definition variable.
   */
  void requestData();

  /**
   * Creates a new named variable and adds it to the list of managed variables
   * @param varName Name of the variable in the sim
   * @param optional unit Unit of the variable (default=Number)
   * @param autoReading optional flag to indicate if the variable should be read automatically (default=false)
   * @param autoWriting optional flag to indicate if the variable should be written automatically (default=false)
   * @param maxAgeTime optional maximum age of the variable in seconds (default=0)
   * @param maxAgeTicks optional Maximum age of the variable in ticks (default=0)
   * @return A shared pointer to the variable
   * @see Units.h for available units
   */
  std::shared_ptr<NamedVariable> make_named_var(
    const std::string &varName,
    ENUM unit = UNITS.Number,
    bool autoReading = false,
    bool autoWriting = false,
    FLOAT64 maxAgeTime = 0.0,
    UINT64 maxAgeTicks = 0);

  /**
   * Creates a new AircraftVariable and adds it to the list of managed variables.
   * @param varName Name of the variable in the sim
   * @param index Index of the indexed variable in the sim
   * @param setterEventName the name of the event to set the variable with an event or calculator code
   * @param optional unit Unit of the variable (default=Number)
   * @param autoReading optional flag to indicate if the variable should be read automatically (default=false)
   * @param autoWriting optional flag to indicate if the variable should be written automatically (default=false)
   * @param maxAgeTime optional maximum age of the variable in seconds (default=0)
   * @param maxAgeTicks optional Maximum age of the variable in ticks (default=0)
   * @return A shared pointer to the variable
   * @see Units.h for available units
   */
  std::shared_ptr<AircraftVariable> make_aircraft_var(
    const std::string &varName,
    int index = 0,
    std::string setterEventName = "",
    EventPtr setterEvent = nullptr,
    ENUM unit = UNITS.Number,
    bool autoReading = false,
    bool autoWriting = false,
    FLOAT64 maxAgeTime = 0.0,
    UINT64 maxAgeTicks = 0);

  /**
   * Creates a new data definition variable and adds it to the list of managed variables.
   * @param name An arbitrary name for the data definition variable for debugging purposes
   * @param dataDefinitions A vector of data definitions for the data definition variable
   * @param dataStruct A pointer to the data structure for the data definition variable.
   * THIS STRUCTURE MUST MATCH THE DATA DEFINITIONS!
   * @param dataStructSize The size of the data structure for the data definition variable.
   * @param autoReading optional flag to indicate if the variable should be read automatically (default=false)
   * @param autoWriting optional flag to indicate if the variable should be written automatically (default=false)
   * @param maxAgeTime optional maximum age of the variable in seconds (default=0)
   * @param maxAgeTicks optional Maximum age of the variable in ticks (default=0)
   * @return A shared pointer to the variable
   */
  std::shared_ptr<DataDefinitionVariable> make_datadefinition_var(
    const std::string &name,
    std::vector<DataDefinitionVariable::DataDefinition> &dataDefinitions,
    void* dataStruct,
    size_t dataStructSize,
    bool autoReading = false,
    bool autoWriting = false,
    FLOAT64 maxAgeTime = 0.0,
    UINT64 maxAgeTicks = 0);

  /**
   * Creates a new event and adds it to the list of managed events.
   */
  std::shared_ptr<Event> make_event(const std::string &eventName);

private:
  void processDispatchMessage(SIMCONNECT_RECV* pRecv, DWORD* pInt);
};

#endif // FLYBYWIRE_A32NX_DATAMANAGER_H
