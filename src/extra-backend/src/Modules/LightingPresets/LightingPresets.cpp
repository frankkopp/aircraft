// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <MSFS/Legacy/gauges.h>
#include <iostream>

#include "MsfsHandler.h"
#include "LightingPresets.h"

LightingPresets::LightingPresets(MsfsHandler* msfsHandler) : Module(msfsHandler) {}

bool LightingPresets::initialize() {
  std::cout << "LightingPresets::initialize()" << std::endl;

  dataManager = &msfsHandler->getDataManager();

  // Control LVARs - auto updated with every tick - LOAD/SAVE also auto written to sim
  elecAC1Powered = dataManager->make_named_var("A32NX_ELEC_AC_1_BUS_IS_POWERED", UNITS.Number, true, false);
  loadLightingPresetRequest = dataManager->make_named_var("A32NX_LIGHTING_PRESET_LOAD", UNITS.Number, true, true);
  saveLightingPresetRequest = dataManager->make_named_var("A32NX_LIGHTING_PRESET_SAVE", UNITS.Number, true, true);

  // Lighting LVARs - manual update and write when load/saving is requested
  efbBrightness = dataManager->make_named_var("A32NX_EFB_BRIGHTNESS", UNITS.Number, false, false);
  dcduLeftLightLevel = dataManager->make_named_var("A32NX_PANEL_DCDU_L_BRIGHTNESS", UNITS.Number, false, false);
  dcduRightLightLevel = dataManager->make_named_var("A32NX_PANEL_DCDU_R_BRIGHTNESS", UNITS.Number, false, false);
  mcduLeftLightLevel = dataManager->make_named_var("A32NX_MCDU_L_BRIGHTNESS", UNITS.Number, false, false);
  mcduRightLightLevel = dataManager->make_named_var("A32NX_MCDU_R_BRIGHTNESS", UNITS.Number, false, false);

  // Light Potentiometers - manual update and write when load/saving is requested
  lightCabin = dataManager->make_aircraft_var("LIGHT CABIN", 0, "CABIN_LIGHTS_SET", UNITS.Percent, false, false);
  lightCabinLevel  = getLightPotentiometerVar(7);
  ovhdIntegralLightLevel = getLightPotentiometerVar(86);
  glareshieldIntegralLightLevel = getLightPotentiometerVar(84);
  glareshieldLcdLightLevel = getLightPotentiometerVar(87);
  tableLightCptLevel = getLightPotentiometerVar(10);
  tableLightFoLevel = getLightPotentiometerVar(11);
  pfdBrtCptLevel = getLightPotentiometerVar(88);
  ndBrtCptLevel = getLightPotentiometerVar(89);
  wxTerrainBrtCptLevel = getLightPotentiometerVar(94);
  consoleLightCptLevel = getLightPotentiometerVar(8);
  pfdBrtFoLevel = getLightPotentiometerVar(90);
  ndBrtFoLevel = getLightPotentiometerVar(91);
  wxTerrainBrtFoLevel = getLightPotentiometerVar(95);
  consoleLightFoLevel = getLightPotentiometerVar(9);
  ecamUpperLightLevel = getLightPotentiometerVar(92);
  ecamLowerLightLevel = getLightPotentiometerVar(93);
  floodPnlLightLevel = getLightPotentiometerVar(83);
  pedestalIntegralLightLevel = getLightPotentiometerVar(85);
  floodPedLightLevel = getLightPotentiometerVar(76);

  std::cout << "LightingPresets::initialized()" << std::endl;
  isInitialized = true;
  return true;
}

bool LightingPresets::preUpdate(sGaugeDrawData* pData) {
  //  std::cout << "LightingPresets::preUpdate()" << std::endl;
  return true;
}

bool LightingPresets::update(sGaugeDrawData* pData) {
  if (!isInitialized) {
    std::cout << "LightingPresets::update() - not initialized" << std::endl;
    return false;
  }

  // only run when aircraft is powered
  if (!elecAC1Powered->getAsBool()) return true;

  // load becomes priority in case both vars are set.
  if (loadLightingPresetRequest->getAsBool()) {
    loadLightingPreset(loadLightingPresetRequest->getAsInt64());

  }
  else if (saveLightingPresetRequest->getAsBool()) {
    saveLightingPreset(saveLightingPresetRequest->getAsInt64());
  }

  loadLightingPresetRequest->setAsInt64(0);
  saveLightingPresetRequest->setAsInt64(0);

  return true;
}

bool LightingPresets::postUpdate(sGaugeDrawData* pData) {
  //  std::cout << "LightingPresets::postUpdate()" << std::endl;
  return true;
}

bool LightingPresets::shutdown() {
  //  std::cout << "LightingPresets::shutdown()" << std::endl;
  return true;
}

// =================================================================================================
// PRIVATE METHODS
// =================================================================================================

void LightingPresets::loadLightingPreset(int64_t loadPresetRequest) {
  std::cout << "LightingPresets: Loading preset: " << loadPresetRequest << std::endl;
  if (readFromStore(loadPresetRequest)) {
    applyToAircraft();
    std::cout << "LightingPresets: Lighting Preset: " << loadPresetRequest
              << " successfully loaded."
              << std::endl;
    return;
  }
  std::cout << "LightingPresets: Loading Lighting Preset: " << loadPresetRequest << " failed."
            << std::endl;
}

void LightingPresets::saveLightingPreset(int64_t savePresetRequest) {
  std::cout << "LightingPresets: Save to Lighting Preset: " << savePresetRequest << std::endl;
  readFromAircraft();
  if (saveToStore(savePresetRequest)) {
    std::cout << "LightingPresets: Lighting Preset: " << savePresetRequest << " successfully saved."
              << std::endl;
    return;
  }
  std::cout << "LightingPresets: Saving Lighting Preset: " << savePresetRequest << " failed."
            << std::endl;
}

void LightingPresets::readFromAircraft() {
  localLightValues.efbBrightness = efbBrightness->getFromSim();
  localLightValues.cabinLightLevel = lightCabinLevel->getFromSim();
  localLightValues.ovhdIntegralLightLevel = ovhdIntegralLightLevel->getFromSim();
  localLightValues.glareshieldIntegralLightLevel = glareshieldIntegralLightLevel->getFromSim();
  localLightValues.glareshieldLcdLightLevel = glareshieldLcdLightLevel->getFromSim();
  localLightValues.tableLightCptLevel = tableLightCptLevel->getFromSim();
  localLightValues.tableLightFoLevel = tableLightFoLevel->getFromSim();
  localLightValues.pfdBrtCptLevel = pfdBrtCptLevel->getFromSim();
  localLightValues.ndBrtCptLevel = ndBrtCptLevel->getFromSim();
  localLightValues.wxTerrainBrtCptLevel = wxTerrainBrtCptLevel->getFromSim();
  localLightValues.consoleLightCptLevel = consoleLightCptLevel->getFromSim();
  localLightValues.pfdBrtFoLevel = pfdBrtFoLevel->getFromSim();
  localLightValues.ndBrtFoLevel = ndBrtFoLevel->getFromSim();
  localLightValues.wxTerrainBrtFoLevel = wxTerrainBrtFoLevel->getFromSim();
  localLightValues.consoleLightFoLevel = consoleLightFoLevel->getFromSim();
  localLightValues.dcduLeftLightLevel = dcduLeftLightLevel->getFromSim();
  localLightValues.dcduRightLightLevel = dcduLeftLightLevel->getFromSim();
  localLightValues.mcduLeftLightLevel = mcduLeftLightLevel->getFromSim();
  localLightValues.mcduRightLightLevel = mcduRightLightLevel->getFromSim();
  localLightValues.ecamUpperLightLevel = ecamUpperLightLevel->getFromSim();
  localLightValues.ecamLowerLightLevel = ecamLowerLightLevel->getFromSim();
  localLightValues.floodPnlLightLevel = floodPnlLightLevel->getFromSim();
  localLightValues.pedestalIntegralLightLevel = pedestalIntegralLightLevel->getFromSim();
  localLightValues.floodPedLightLevel = floodPedLightLevel->getFromSim();
}

void LightingPresets::applyToAircraft() {
  efbBrightness->setAndWriteToSim(localLightValues.efbBrightness);
  setValidCabinLightValue(localLightValues.cabinLightLevel);
  ovhdIntegralLightLevel->setAndWriteToSim(localLightValues.ovhdIntegralLightLevel);
  glareshieldIntegralLightLevel->setAndWriteToSim(localLightValues.glareshieldIntegralLightLevel);
  glareshieldLcdLightLevel->setAndWriteToSim(localLightValues.glareshieldLcdLightLevel);
  tableLightCptLevel->setAndWriteToSim(localLightValues.tableLightCptLevel);
  tableLightFoLevel->setAndWriteToSim(localLightValues.tableLightFoLevel);
  pfdBrtCptLevel->setAndWriteToSim(localLightValues.pfdBrtCptLevel);
  ndBrtCptLevel->setAndWriteToSim(localLightValues.ndBrtCptLevel);
  wxTerrainBrtCptLevel->setAndWriteToSim(localLightValues.wxTerrainBrtCptLevel);
  consoleLightCptLevel->setAndWriteToSim(localLightValues.consoleLightCptLevel);
  pfdBrtFoLevel->setAndWriteToSim(localLightValues.pfdBrtFoLevel);
  ndBrtFoLevel->setAndWriteToSim(localLightValues.ndBrtFoLevel);
  wxTerrainBrtFoLevel->setAndWriteToSim(localLightValues.wxTerrainBrtFoLevel);
  consoleLightFoLevel->setAndWriteToSim(localLightValues.consoleLightFoLevel);
  dcduLeftLightLevel->setAndWriteToSim(localLightValues.dcduLeftLightLevel);
  dcduRightLightLevel->setAndWriteToSim(localLightValues.dcduRightLightLevel);
  mcduLeftLightLevel->setAndWriteToSim(localLightValues.mcduLeftLightLevel);
  mcduRightLightLevel->setAndWriteToSim(localLightValues.mcduRightLightLevel);
  ecamUpperLightLevel->setAndWriteToSim(localLightValues.ecamUpperLightLevel);
  ecamLowerLightLevel->setAndWriteToSim(localLightValues.ecamLowerLightLevel);
  floodPnlLightLevel->setAndWriteToSim(localLightValues.floodPnlLightLevel);
  pedestalIntegralLightLevel->setAndWriteToSim(localLightValues.pedestalIntegralLightLevel);
  floodPedLightLevel->setAndWriteToSim(localLightValues.floodPedLightLevel);
}

bool LightingPresets::readFromStore(int64_t presetNr) {
  // create ini file and data structure
  mINI::INIStructure ini;
  mINI::INIFile iniFile(CONFIGURATION_FILEPATH);

  // load file
  bool result = iniFile.read(ini);

  const std::string preset = "preset " + std::to_string(presetNr);

  // check if preset is available
  // if not use a 50% default preset
  if (!ini.has(preset)) {
    loadFromData(DEFAULT_50);
    return true;
  }

  // reading data structure from ini
  localLightValues.efbBrightness = iniGetOrDefault(ini, preset, "efb_brightness", 80.0);
  localLightValues.cabinLightLevel = iniGetOrDefault(ini, preset, "cabin_light", 50.0);
  localLightValues.ovhdIntegralLightLevel = iniGetOrDefault(ini, preset, "ovhd_int_lt", 50.0);
  localLightValues.glareshieldIntegralLightLevel = iniGetOrDefault(ini, preset, "glareshield_int_lt", 50.0);
  localLightValues.glareshieldLcdLightLevel = iniGetOrDefault(ini, preset, "glareshield_lcd_lt", 50.0);
  localLightValues.tableLightCptLevel = iniGetOrDefault(ini, preset, "table_cpt_lt", 50.0);
  localLightValues.tableLightFoLevel = iniGetOrDefault(ini, preset, "table_fo_lt", 50.0);
  localLightValues.pfdBrtCptLevel = iniGetOrDefault(ini, preset, "pfd_cpt_lvl", 50.0);
  localLightValues.ndBrtCptLevel = iniGetOrDefault(ini, preset, "nd_cpt_lvl", 50.0);
  localLightValues.wxTerrainBrtCptLevel = iniGetOrDefault(ini, preset, "wx_cpt_lvl", 50.0);
  localLightValues.consoleLightCptLevel = iniGetOrDefault(ini, preset, "console_cpt_lt", 50.0);
  localLightValues.pfdBrtFoLevel = iniGetOrDefault(ini, preset, "pfd_fo_lvl", 50.0);
  localLightValues.ndBrtFoLevel = iniGetOrDefault(ini, preset, "nd_fo_lvl", 50.0);
  localLightValues.wxTerrainBrtFoLevel = iniGetOrDefault(ini, preset, "wx_fo_lvl", 50.0);
  localLightValues.consoleLightFoLevel = iniGetOrDefault(ini, preset, "console_fo_lt", 50.0);
  localLightValues.dcduLeftLightLevel = iniGetOrDefault(ini, preset, "dcdu_left_lvl", 50.0) / 100;
  localLightValues.dcduRightLightLevel = iniGetOrDefault(ini, preset, "dcdu_right_lvl", 50.0) / 100;
  localLightValues.mcduLeftLightLevel = iniGetOrDefault(ini, preset, "mcdu_left_lvl", 50.0) / 100;
  localLightValues.mcduRightLightLevel = iniGetOrDefault(ini, preset, "mcdu_right_lvl", 50.0) / 100;
  localLightValues.ecamUpperLightLevel = iniGetOrDefault(ini, preset, "ecam_upper_lvl", 50.0);
  localLightValues.ecamLowerLightLevel = iniGetOrDefault(ini, preset, "ecam_lower_lvl", 50.0);
  localLightValues.floodPnlLightLevel = iniGetOrDefault(ini, preset, "flood_pnl_lt", 50.0);
  localLightValues.pedestalIntegralLightLevel = iniGetOrDefault(ini, preset, "pedestal_int_lt", 50.0);
  localLightValues.floodPedLightLevel = iniGetOrDefault(ini, preset, "flood_ped_lvl", 50.0);

  return result;
}

bool LightingPresets::saveToStore(int64_t presetNr) {
  // create ini file and data structure
  mINI::INIStructure ini;
  mINI::INIFile iniFile(CONFIGURATION_FILEPATH);

  // load file
  bool result = iniFile.read(ini);

  // add/update preset
  const std::string preset = "preset " + std::to_string(presetNr);
  ini[preset]["efb_brightness"] = std::to_string(localLightValues.efbBrightness);
  ini[preset]["cabin_light"] = std::to_string(localLightValues.cabinLightLevel);
  ini[preset]["ovhd_int_lt"] = std::to_string(localLightValues.ovhdIntegralLightLevel);
  ini[preset]["glareshield_int_lt"] = std::to_string(localLightValues.glareshieldIntegralLightLevel);
  ini[preset]["glareshield_lcd_lt"] = std::to_string(localLightValues.glareshieldLcdLightLevel);
  ini[preset]["table_cpt_lt"] = std::to_string(localLightValues.tableLightCptLevel);
  ini[preset]["table_fo_lt"] = std::to_string(localLightValues.tableLightFoLevel);
  ini[preset]["pfd_cpt_lvl"] = std::to_string(localLightValues.pfdBrtCptLevel);
  ini[preset]["nd_cpt_lvl"] = std::to_string(localLightValues.ndBrtCptLevel);
  ini[preset]["wx_cpt_lvl"] = std::to_string(localLightValues.wxTerrainBrtCptLevel);
  ini[preset]["console_cpt_lt"] = std::to_string(localLightValues.consoleLightCptLevel);
  ini[preset]["pfd_fo_lvl"] = std::to_string(localLightValues.pfdBrtFoLevel);
  ini[preset]["nd_fo_lvl"] = std::to_string(localLightValues.ndBrtFoLevel);
  ini[preset]["wx_fo_lvl"] = std::to_string(localLightValues.wxTerrainBrtFoLevel);
  ini[preset]["console_fo_lt"] = std::to_string(localLightValues.consoleLightFoLevel);
  ini[preset]["dcdu_left_lvl"] = std::to_string(localLightValues.dcduLeftLightLevel * 100);
  ini[preset]["dcdu_right_lvl"] = std::to_string(localLightValues.dcduRightLightLevel * 100);
  ini[preset]["mcdu_left_lvl"] = std::to_string(localLightValues.mcduLeftLightLevel * 100);
  ini[preset]["mcdu_right_lvl"] = std::to_string(localLightValues.mcduRightLightLevel * 100);
  ini[preset]["ecam_upper_lvl"] = std::to_string(localLightValues.ecamUpperLightLevel);
  ini[preset]["ecam_lower_lvl"] = std::to_string(localLightValues.ecamLowerLightLevel);
  ini[preset]["flood_pnl_lt"] = std::to_string(localLightValues.floodPnlLightLevel);
  ini[preset]["pedestal_int_lt"] = std::to_string(localLightValues.pedestalIntegralLightLevel);
  ini[preset]["flood_ped_lvl"] = std::to_string(localLightValues.floodPedLightLevel);

  result &= iniFile.write(ini, true);

  return result;
}

void LightingPresets::loadFromData(LightingValues lv) {
  localLightValues = lv;
}

std::string LightingPresets::sprint() const {
  std::ostringstream os;
  os << "EFB Brightness: " << localLightValues.efbBrightness << std::endl;
  os << "Cabin Light: " << localLightValues.cabinLightLevel << std::endl;
  os << "Ovhd Int Lt: " << localLightValues.ovhdIntegralLightLevel << std::endl;
  os << "Glareshield Int Lt: " << localLightValues.glareshieldIntegralLightLevel << std::endl;
  os << "Glareshield Lcd Lt: " << localLightValues.glareshieldLcdLightLevel << std::endl;
  os << "Table Cpt Lt: " << localLightValues.tableLightCptLevel << std::endl;
  os << "Table FO Lt: " << localLightValues.tableLightFoLevel << std::endl;
  os << "PFD Cpt Lvl: " << localLightValues.pfdBrtCptLevel << std::endl;
  os << "ND Cpt Lvl: " << localLightValues.ndBrtCptLevel << std::endl;
  os << "WX Cpt Lvl: " << localLightValues.wxTerrainBrtCptLevel << std::endl;
  os << "Console Cpt Lt: " << localLightValues.consoleLightCptLevel << std::endl;
  os << "PFD FO Lvl: " << localLightValues.pfdBrtFoLevel << std::endl;
  os << "ND FO Lvl: " << localLightValues.ndBrtFoLevel << std::endl;
  os << "WX FO Lvl: " << localLightValues.wxTerrainBrtFoLevel << std::endl;
  os << "Console Fo Lt: " << localLightValues.consoleLightFoLevel << std::endl;
  os << "DCDU Left Lvl: " << localLightValues.dcduLeftLightLevel << std::endl;
  os << "DCDU Right Lvl: " << localLightValues.dcduRightLightLevel << std::endl;
  os << "MCDU Left Lvl: " << localLightValues.mcduLeftLightLevel << std::endl;
  os << "MCDU Right Lvl: " << localLightValues.mcduRightLightLevel << std::endl;
  os << "ECAM Upper Lvl: " << localLightValues.ecamUpperLightLevel << std::endl;
  os << "ECAM Lower Lvl: " << localLightValues.ecamLowerLightLevel << std::endl;
  os << "Floor Cpt Lt: " << localLightValues.floodPnlLightLevel << std::endl;
  os << "Pedestal Int Lt: " << localLightValues.pedestalIntegralLightLevel << std::endl;
  os << "Floor FO Lvl: " << localLightValues.floodPedLightLevel << std::endl;
  return os.str();
}

double LightingPresets::iniGetOrDefault(
  const mINI::INIStructure &ini,
  const std::string &section,
  const std::string &key,
  const double defaultValue) {

  if (ini.get(section).has(key)) {
    // As MSFS wasm does not support exceptions (try/catch) we can't use
    // std::stof here. Workaround with std::stringstreams.
    std::stringstream input(ini.get(section).get(key));
    double value = defaultValue;
    if (input >> value) {
      return value;
    }
    else {
      std::cout << "FLYPAD_BACKEND: reading ini value for \""
                << "[" << section << "] " << key << " = " << ini.get(section).get(key)
                << "\" failed." << std::endl;
    }
  }
  return defaultValue;
}

std::shared_ptr<AircraftVariable>
LightingPresets::getLightPotentiometerVar(int index) const {
  return dataManager->make_aircraft_var(
    "LIGHT POTENTIOMETER",
    index,
    "LIGHT_POTENTIOMETER_SET",
    UNITS.Percent,
    false,
    false,
    0.0,
    0);
}

void LightingPresets::setValidCabinLightValue(FLOAT64 level) {
  // cabin light level needs to either be 0, 50 or 100 for the switch position
  // in the aircraft to work.
  if (level <= 0.0) {
    level = 0.0;
  }
  else if (level > 0.0 && level <= 50.0) {
    level = 50.0;
  }
  else if ((level > 0.0 && level > 50.0)) {
    level = 100.0;
  }
  // cabin lights in the A32NX need to be controlled by two vars
  // one for the switch position and one for the actual light
  lightCabin->setAndWriteToSim(level > 0 ? 1 : 0);
  lightCabinLevel->setAndWriteToSim(level);
}
