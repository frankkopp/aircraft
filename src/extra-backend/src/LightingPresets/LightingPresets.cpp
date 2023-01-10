// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <MSFS/Legacy/gauges.h>
#include <iostream>

#include "LightingPresets.h"

LightingPresets::LightingPresets(MsfsHandler *msfsHandler) : Module(msfsHandler) {
  std::cout << "LightingPresets::LightingPresets()" << std::endl;
}

bool LightingPresets::initialize() {
  std::cout << "LightingPresets::initialize()" << std::endl;
  return isInitialized = true;
}

bool LightingPresets::preUpdate(sGaugeDrawData *pData) {
//  std::cout << "LightingPresets::preUpdate()" << std::endl;
  return true;
}
bool LightingPresets::update(sGaugeDrawData *pData) {
//  std::cout << "LightingPresets::update()" << std::endl;
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
