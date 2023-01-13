// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_MODULE_H
#define FLYBYWIRE_A32NX_MODULE_H

#include <memory>

#include <MSFS/Legacy/gauges.h>

class MsfsHandler;
class DataManager;

/**
 * Base class and interface for all modules to ensure that they are compatible with the MsfsHandler.
 * <p/>
 * Make sure to add an error (std::cerr) message if anything goes wrong and especially if
 * initialize(), preUpdate(), update() or postUpdate() return false.
 * MSFS does not support Exception so good logging and error messages are the only way to inform the
 * user/developer if somethings went wrong and where and what happened.
 * Non-excessive positive logging about what is happening is also a good idea and helps
 * tremendously with finding any issues as it will be easier to locate the cause of the issue.
 */
class Module {
protected:
  /**
   * The MsfsHandler instance that is used to communicate with the simulator.
   */
  MsfsHandler *msfsHandler;

  /**
   * Flag to indicate if the module has been initialized.
   */
  bool isInitialized = false;

public:

  Module() = delete;

  /**
   * Creates a new module and takes a reference to the MsfsHandler instance.
   * @param msfsHandler The MsfsHandler instance that is used to communicate with the simulator.
   */
  explicit Module(MsfsHandler *msfsHandler);

  /**
   * Called by the MsfsHandler instance once to initialize the module.
   * Happens during the PANEL_SERVICE_PRE_INSTALL message from the sim.
   * @return true if the module was successfully initialized, false otherwise.
   */
  virtual bool initialize() = 0;

  /**
   * Called first by the MsfsHandler instance during the PANEL_SERVICE_PRE_DRAW phase from the sim.
   * @param pData sGaugeDrawData structure containing the data for the current frame.
   * @return false if an error occurred, true otherwise.
   */
  virtual bool preUpdate(sGaugeDrawData *pData) = 0;

  /**
   * Called second by the MsfsHandler instance during the PANEL_SERVICE_PRE_DRAW phase from the sim.
   * @param pData sGaugeDrawData structure containing the data for the current frame.
   * @return false if an error occurred, true otherwise.
   */
  virtual bool update(sGaugeDrawData *pData) = 0;

  /**
   * Called last by the MsfsHandler instance during the PANEL_SERVICE_PRE_DRAW phase from the sim.
   * @param pData sGaugeDrawData structure containing the data for the current frame.
   * @return
   */
  virtual bool postUpdate(sGaugeDrawData *pData) = 0;

  /**
   * Called by the MsfsHandler instance during the PANEL_SERVICE_PRE_KILL phase from the sim.
   * @return false if an error occurred, true otherwise.
   */
  virtual bool shutdown() = 0;
};

#endif // FLYBYWIRE_A32NX_MODULE_H
