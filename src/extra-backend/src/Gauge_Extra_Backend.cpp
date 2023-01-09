// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef __INTELLISENSE__
#define MODULE_EXPORT __attribute__((visibility("default")))
#define MODULE_WASM_MODNAME(mod) __attribute__((import_module(mod)))
#else
#define MODULE_EXPORT
#define MODULE_WASM_MODNAME(mod)
#define __attribute__(x)
#define __restrict__
#endif

#include <MSFS/Legacy/gauges.h>
#include <MSFS/MSFS.h>
#include <MSFS/MSFS_Render.h>
#include <SimConnect.h>

#include <iostream>

/**
 * Gauge Callback
 * @see
 * https://docs.flightsimulator.com/html/Content_Configuration/SimObjects/Aircraft_SimO/Instruments/C_C++_Gauges.htm?rhhlterm=_gauge_callback&rhsearch=_gauge_callback
 */
__attribute__((export_name("Gauge_Extra_Backend_gauge_callback"))) extern "C" __attribute__((unused)) bool
Gauge_Extra_Backend(__attribute__((unused)) FsContext ctx, int service_id, void *pData) {
  switch (service_id) {
  case PANEL_SERVICE_PRE_INSTALL: {
    std::cout << "EXTRA1: PANEL_SERVICE_PRE_INSTALL" << std::endl;
    //      return FLYPAD_BACKEND.initialize();
    return true;
  }
  case PANEL_SERVICE_POST_INSTALL: {
    std::cout << "EXTRA1: PANEL_SERVICE_POST_INSTALL" << std::endl;
    return true;
  }
  case PANEL_SERVICE_PRE_INITIALIZE: {
    std::cout << "EXTRA1: PANEL_SERVICE_PRE_INITIALIZE" << std::endl;
    return true;
  }
  case PANEL_SERVICE_POST_INITIALIZE: {
    std::cout << "EXTRA1: PANEL_SERVICE_POST_INITIALIZE" << std::endl;
    return true;
  }
  case PANEL_SERVICE_PRE_UPDATE: {
    std::cout << "EXTRA1: PANEL_SERVICE_PRE_UPDATE" << std::endl;
    return true;
  }
  case PANEL_SERVICE_POST_UPDATE: {
    std::cout << "EXTRA1: PANEL_SERVICE_POST_UPDATE" << std::endl;
    return true;
  }
  case PANEL_SERVICE_PRE_DRAW: {
    std::cout << "EXTRA1: PANEL_SERVICE_PRE_DRAW" << std::endl;
    //      auto drawData = static_cast<sGaugeDrawData*>(pData);
    //      return FLYPAD_BACKEND.onUpdate(drawData->dt);
    return true;
  }
  case PANEL_SERVICE_POST_DRAW: {
    std::cout << "EXTRA1: PANEL_SERVICE_POST_DRAW" << std::endl;
    return true;
  }
  case PANEL_SERVICE_PRE_KILL: {
    std::cout << "EXTRA1: PANEL_SERVICE_PRE_KILL" << std::endl;
    //      return FLYPAD_BACKEND.shutdown();
    return true;
  }
  case PANEL_SERVICE_POST_KILL: {
    std::cout << "EXTRA1: PANEL_SERVICE_POST_KILL" << std::endl;
    return true;
  }
  default:
    break;
  }
  return false;
}

/**
 * Gauge Callback
 * @see
 * https://docs.flightsimulator.com/html/Content_Configuration/SimObjects/Aircraft_SimO/Instruments/C_C++_Gauges.htm?rhhlterm=_gauge_callback&rhsearch=_gauge_callback
 */
__attribute__((export_name("Gauge_Extra2_Backend_gauge_callback"))) extern "C" __attribute__((unused)) bool
Gauge_Extra2_Backend(__attribute__((unused)) FsContext ctx, int service_id, void *pData) {
  switch (service_id) {
  case PANEL_SERVICE_PRE_INSTALL: {
    std::cout << "EXTRA2: PANEL_SERVICE_PRE_INSTALL" << std::endl;
    //      return FLYPAD_BACKEND.initialize();
    return true;
  }
  case PANEL_SERVICE_POST_INSTALL: {
    std::cout << "EXTRA2: PANEL_SERVICE_POST_INSTALL" << std::endl;
    return true;
  }
  case PANEL_SERVICE_PRE_INITIALIZE: {
    std::cout << "EXTRA2: PANEL_SERVICE_PRE_INITIALIZE" << std::endl;
    return true;
  }
  case PANEL_SERVICE_POST_INITIALIZE: {
    std::cout << "EXTRA2: PANEL_SERVICE_POST_INITIALIZE" << std::endl;
    return true;
  }
  case PANEL_SERVICE_PRE_UPDATE: {
    std::cout << "EXTRA2: PANEL_SERVICE_PRE_UPDATE" << std::endl;
    return true;
  }
  case PANEL_SERVICE_POST_UPDATE: {
    std::cout << "EXTRA2: PANEL_SERVICE_POST_UPDATE" << std::endl;
    return true;
  }
  case PANEL_SERVICE_PRE_DRAW: {
    std::cout << "EXTRA2: PANEL_SERVICE_PRE_DRAW" << std::endl;
    //      auto drawData = static_cast<sGaugeDrawData*>(pData);
    //      return FLYPAD_BACKEND.onUpdate(drawData->dt);
    return true;
  }
  case PANEL_SERVICE_POST_DRAW: {
    std::cout << "EXTRA2: PANEL_SERVICE_POST_DRAW" << std::endl;
    return true;
  }
  case PANEL_SERVICE_PRE_KILL: {
    std::cout << "EXTRA2: PANEL_SERVICE_PRE_KILL" << std::endl;
    //      return FLYPAD_BACKEND.shutdown();
    return true;
  }
  case PANEL_SERVICE_POST_KILL: {
    std::cout << "EXTRA2: PANEL_SERVICE_POST_KILL" << std::endl;
    return true;
  }
  default:
    break;
  }
  return false;
}

