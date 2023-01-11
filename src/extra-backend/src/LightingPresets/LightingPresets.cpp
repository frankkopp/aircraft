// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <MSFS/Legacy/gauges.h>
#include <iostream>

#include "MsfsHandler.h"
#include "LightingPresets.h"

LightingPresets::LightingPresets(MsfsHandler *msfsHandler) : Module(msfsHandler) {}

bool LightingPresets::initialize() {
  std::cout << "LightingPresets::initialize()" << std::endl;

  DataManager &dataManager = msfsHandler->getDataManager();
  elecAC1Powered = dataManager.make_named_var("A32NX_ELEC_AC_1_BUS_IS_POWERED", UNITS.Number, true, false);
  loadLightingPresetRequest = dataManager.make_named_var("A32NX_LIGHTING_PRESET_LOAD", UNITS.Number, true, true);
  saveLightingPresetRequest = dataManager.make_named_var("A32NX_LIGHTING_PRESET_SAVE", UNITS.Number, true, true);

  std::cout << "LightingPresets::initialized()" << std::endl;
  return isInitialized = true;
}

bool LightingPresets::preUpdate(sGaugeDrawData *pData) {
//  std::cout << "LightingPresets::preUpdate()" << std::endl;
  return true;
}

bool LightingPresets::update(sGaugeDrawData *pData) {
  if (!isInitialized) {
    std::cout << "LightingPresets::update() - not initialized" << std::endl;
    return false;
  }

  // only run when aircraft is powered
  if (!elecAC1Powered->getAsBool()) return true;

  // load becomes priority in case both vars are set.
  if (loadLightingPresetRequest->getAsBool()) {
    std::cout << "LightingPresets::update() - load lighting preset: "
    << loadLightingPresetRequest->get() << std::endl;
  }
  else if (saveLightingPresetRequest->getAsBool()) {
    std::cout << "LightingPresets::update() - save lighting preset: "
    << saveLightingPresetRequest->get() << std::endl;
  }

  // TODO: implement

  loadLightingPresetRequest->setAsBool(false);
  saveLightingPresetRequest->setAsBool(false);

  return true;
}
bool LightingPresets::postUpdate(sGaugeDrawData *pData) {
//  std::cout << "LightingPresets::postUpdate()" << std::endl;
  return true;
}
bool LightingPresets::shutdown() {
//  std::cout << "LightingPresets::shutdown()" << std::endl;
  return true;
}
