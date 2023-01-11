// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_LIGHTINGPRESETS_H
#define FLYBYWIRE_A32NX_LIGHTINGPRESETS_H

#include "Module.h"
#include "DataManager.h"

class MsfsHandler;

class LightingPresets : public Module {
  std::shared_ptr<NamedVariable> elecAC1Powered;
  std::shared_ptr<NamedVariable> loadLightingPresetRequest;
  std::shared_ptr<NamedVariable> saveLightingPresetRequest;

public:
  explicit LightingPresets(MsfsHandler *msfsHandler);
  bool initialize() override;
  bool preUpdate(sGaugeDrawData *pData) override;
  bool update(sGaugeDrawData *pData) override;
  bool postUpdate(sGaugeDrawData *pData) override;
  bool shutdown() override;
};

#endif // FLYBYWIRE_A32NX_LIGHTINGPRESETS_H
