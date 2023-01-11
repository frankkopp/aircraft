// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_MSFSHANDLER_H
#define FLYBYWIRE_A32NX_MSFSHANDLER_H

#include <MSFS/Legacy/gauges.h>
#include <MSFS/MSFS.h>
#include <MSFS/MSFS_Render.h>
#include <SimConnect.h>

#include <vector>

#include "DataManager/DataManager.h"

class Module;

/**
 * MsfsHandler is a lightweight abstraction layer for the MSFS SDK and Simconnect to handle the
 * communication with the simulator mainly for standard variables and events.
 * It is not meant to fully replace the SDK but to provide a simple interface for the most common
 * tasks.
 * It does not limit the usage of the SDK or Simconnect in any way!
 */
class MsfsHandler {
  /**
   * A list of all modules that are currently loaded.
   * This list is used to call the preUpdate, update and postUpdate methods of each module.
   * Each module is responsible for registering itself in this list - this is done in the
   * constructor of the module.
   * The order of the modules in this list is important as the update methods are called in
   * the order of the list. The order is determined by the order of creation of the modules.
   */
  std::vector<Module *> modules{};

  /**
   * The data manager is responsible for managing all variables and events.
   * It is used to register variables and events and to update them.
   * It de-duplicates variables and events and only creates one instance of each if multiple modules
   * use the same variable.
   */
  DataManager dataManager{};

  /**
   * Each simconnect instance has a name to identify it.
   */
  std::string simConnectName;

  /**
   * The handle of the simconnect instance.
   */
  HANDLE hSimConnect{};

  /**
   * Flag to indicate if the MsfsHandler instance is initialized.
   */
  bool isInitialized = false;

  // Common variables required by the MsfsHandler itself
  std::shared_ptr<NamedVariable> a32nxIsReady;
  std::shared_ptr<NamedVariable> a32nxIsDevelopmentState;
  std::shared_ptr<DataDefinitionVariable> baseSimData;

  /**
   * This struct is used to define the data definition for the base sim data.
   */
  struct BaseSimData {
    FLOAT64 simulationTime;
  } simData{};

  /**
   * Used to detect pause in the sim.
   */
  FLOAT64 previousSimulationTime{};

  UINT64 tickCounter = 0;

public:
  /**
   * Creates a new MsfsHandler instance.
   * @param name string containing an appropriate simconnect name for the client program.
   */
  explicit MsfsHandler(std::string name);

  /**
   * Initializes the MsfsHandler instance. This method must be called before any other method.
   * Opens a simconnect instance, initializes the data manager and calls initialize on all modules.
   * Is called by the gauge handler when the PANEL_SERVICE_PRE_INSTALL event is received.
   * @return true if the initialization was successful, false otherwise.
   */
  bool initialize();

  /**
   * Calls the preUpdate, update, postUpdate method of the DataManager and all modules.
   * Is called by the gauge handler when the PANEL_SERVICE_PRE_DRAW event is received.
   * @param pData pointer to the sGaugeDrawData struct.
   * @return true if the update was successful, false otherwise.
   */
  bool update(sGaugeDrawData *pData);

  /**
   * Calls the shutdown method of the DataManager and all modules and closes the simconnect instance.
   * Is called by the gauge handler when the PANEL_SERVICE_PRE_KILL event is received.
   * @return true if the shutdown was successful, false otherwise.
   */
  bool shutdown();

  /**
   * Callback method for modules to register themselves.
   * @param pModule pointer to the module that should be registered.
   */
  void registerModule(Module *pModule);

private:
  /**
   * Handles opening the simconnect connection..
   * @return
   */
  bool initializeSimConnect();


// Getters and setters
public:

  DataManager &getDataManager() { return dataManager; }

  [[nodiscard]]
  bool getA32NxIsReady() const { return a32nxIsReady->getAsBool(); }

  [[nodiscard]]
  FLOAT64 getA32NxIsDevelopmentState() const { return a32nxIsDevelopmentState->get(); }

  [[nodiscard]]
  FLOAT64 getPreviousSimulationTime() const { return previousSimulationTime; }

  [[nodiscard]]
  UINT64 getTickCounter() const { return tickCounter; }
};

#endif // FLYBYWIRE_A32NX_MSFSHANDLER_H
