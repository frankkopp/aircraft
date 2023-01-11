// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_MODULE_H
#define FLYBYWIRE_A32NX_MODULE_H

#include <MSFS/Legacy/gauges.h>
class MsfsHandler;

/**
 * Base class and interface for all modules.
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
   * Happens during the PANEL_SERVICE_PRE_INSTALL message from the sim..
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
