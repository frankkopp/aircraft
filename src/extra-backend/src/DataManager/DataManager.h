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

/**
 * DataManager is responsible for managing all variables and events.
 * It is used to register variables and events and to update them.
 * It de-duplicates variables and events and only creates one instance of each if multiple modules
 * use the same variable.
 * It is still possible to use the SDK and Simconnect directly but it is recommended to use the
 * DataManager instead as the data manager is able to de-duplicate variables and events and automatically
 * update and write back variables from/to the sim.
 */
class DataManager {
private:
  /**
   * A map of all registered variables.
   */
  std::map<std::string, std::shared_ptr<CacheableVariable>> variables{};

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
  bool initialize();

  /**
   * Called by the MsfsHandler update() method.
   * Updates all variables marked for automatic reading.
   * @param pData Pointer to the data structure of gauge pre-draw event
   * @return true if successful, false otherwise
   */
  bool preUpdate(sGaugeDrawData *pData);

  /**
 * Called by the MsfsHandler update() method.
 * @param pData Pointer to the data structure of gauge pre-draw event
 * @return true if successful, false otherwise
 */
  bool update(sGaugeDrawData *pData);

  /**
 * Called by the MsfsHandler update() method.
 * Writes all variables marked for automatic writing back to the sim.
 * @param pData Pointer to the data structure of gauge pre-draw event
 * @return true if successful, false otherwise
 */
  bool postUpdate(sGaugeDrawData *pData);

  /**
   * Callback for SimConnect_GetNextDispatch events in the MsfsHandler used for data definition
   * variable batches.
   * Must map data request IDs to know IDs of data definition variables and return true if the
   * requestID has been handled.
   * @param pData Pointer to the data structure of gauge pre-draw event
   * @return true if request ID has been processed, false otherwise
   */
  bool processSimObjectData(SIMCONNECT_RECV_SIMOBJECT_DATA *pData);

  /**
   * Called by the MsfsHandler shutdown() method.
   * Can be used for any extra cleanup.
   * @return true if successful, false otherwise
   */
  bool shutdown();

  /**
   * Creates a new named variable and adds it to the list of managed variables
   * @param varName Name of the variable in the sim
   * @param unit Unit of the variable - @see Units.h
   * @param autoReading Flag to indicate if the variable should be read automatically
   * @param autoWriting Flag to indicate if the variable should be written automatically
   * @param maxAgeTime Maximum age of the variable in seconds
   * @param maxAgeTicks Maximum age of the variable in ticks
   * @return A shared pointer to the variable
   */
  std::shared_ptr<NamedVariable>  make_named_var(
      std::string varName,
      ENUM unit = UNITS.Number,
      bool autoReading = false,
      bool autoWriting = false,
      FLOAT64 maxAgeTime = 0.0,
      UINT64 maxAgeTicks = 0);

  /**
   * Creates a new named variable and adds it to the list of managed variables.
   * Note: Aircraft variables can't be written back to the sim. Use a DataDefinitionVariable
   * instead.
   * @param varName Name of the variable in the sim
   * @param index Index of the indexed variable in the sim
   * @param unit Unit of the variable - @see Units.h
   * @param autoReading Flag to indicate if the variable should be read automatically
   * @param maxAgeTime Maximum age of the variable in seconds
   * @param maxAgeTicks Maximum age of the variable in ticks
   * @return A shared pointer to the variable
   */
  std::shared_ptr<AircraftVariable>  make_aircraft_var(
    std::string varName,
    int index,
    ENUM unit = UNITS.Number,
    bool autoReading = false,
    FLOAT64 maxAgeTime = 0.0,
    UINT64 maxAgeTicks = 0);

};

#endif // FLYBYWIRE_A32NX_DATAMANAGER_H
