// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_LIGHTINGPRESETS_H
#define FLYBYWIRE_A32NX_LIGHTINGPRESETS_H

#include "Module.h"
#include "DataManager.h"
#include "inih/ini.h"

class MsfsHandler;

struct LightingValues {
  // EFB
  FLOAT64 efbBrightness;                  // A32NX_EFB_BRIGHTNESS
  // OVHD
  FLOAT64 cabinLightLevel;                // 7 (0, 50, 100)
  FLOAT64 ovhdIntegralLightLevel;         // 86
  // Glareshield
  FLOAT64 glareshieldIntegralLightLevel;  // 84
  FLOAT64 glareshieldLcdLightLevel;       // 87
  FLOAT64 tableLightCptLevel;             // 10
  FLOAT64 tableLightFoLevel;              // 11
  // Instruments
  FLOAT64 pfdBrtCptLevel;                 // 88
  FLOAT64 ndBrtCptLevel;                  // 89
  FLOAT64 wxTerrainBrtCptLevel;           // 94
  FLOAT64 consoleLightCptLevel;           // 8 (0, 50, 100)
  FLOAT64 pfdBrtFoLevel;                  // 90
  FLOAT64 ndBrtFoLevel;                   // 91
  FLOAT64 wxTerrainBrtFoLevel;            // 95
  FLOAT64 consoleLightFoLevel;            // 9 (0, 50, 100)
  // ISIS display has automatic brightness adjustment.
  FLOAT64 dcduLeftLightLevel;             // A32NX_PANEL_DCDU_L_BRIGHTNESS  0.0..1.0
  FLOAT64 dcduRightLightLevel;            // A32NX_PANEL_DCDU_R_BRIGHTNESS  0.0..1.0
  FLOAT64 mcduLeftLightLevel;             // A32NX_MCDU_L_BRIGHTNESS        0.0..1.0
  FLOAT64 mcduRightLightLevel;            // A32NX_MCDU_R_BRIGHTNESS        0.0..1.0
  // Pedestal
  FLOAT64 ecamUpperLightLevel;            // 92
  FLOAT64 ecamLowerLightLevel;            // 93
  FLOAT64 floodPnlLightLevel;             // 83
  FLOAT64 pedestalIntegralLightLevel;     // 85
  FLOAT64 floodPedLightLevel;             // 76
};

class LightingPresets : public Module {
private:
  const std::string CONFIGURATION_FILEPATH = "\\work\\InteriorLightingPresets.ini";

  // CConvenience pointer to the data manager
  DataManager* dataManager{};

  // Control LVARs
  std::shared_ptr<NamedVariable> elecAC1Powered;
  std::shared_ptr<NamedVariable> loadLightingPresetRequest;
  std::shared_ptr<NamedVariable> saveLightingPresetRequest;

  // Lighting LVARs
  std::shared_ptr<NamedVariable> efbBrightness;
  std::shared_ptr<NamedVariable> dcduLeftLightLevel;
  std::shared_ptr<NamedVariable> dcduRightLightLevel;
  std::shared_ptr<NamedVariable> mcduLeftLightLevel;
  std::shared_ptr<NamedVariable> mcduRightLightLevel;

  // Lighting Aircraft Vars
  std::shared_ptr<AircraftVariable> lightCabin;
  std::shared_ptr<AircraftVariable> lightCabinLevel;
  std::shared_ptr<AircraftVariable> ovhdIntegralLightLevel;
  std::shared_ptr<AircraftVariable> glareshieldIntegralLightLevel;
  std::shared_ptr<AircraftVariable> glareshieldLcdLightLevel;
  std::shared_ptr<AircraftVariable> tableLightCptLevel;
  std::shared_ptr<AircraftVariable> tableLightFoLevel;
  std::shared_ptr<AircraftVariable> pfdBrtCptLevel;
  std::shared_ptr<AircraftVariable> ndBrtCptLevel;
  std::shared_ptr<AircraftVariable> wxTerrainBrtCptLevel;
  std::shared_ptr<AircraftVariable> consoleLightCptLevel;
  std::shared_ptr<AircraftVariable> pfdBrtFoLevel;
  std::shared_ptr<AircraftVariable> ndBrtFoLevel;
  std::shared_ptr<AircraftVariable> wxTerrainBrtFoLevel;
  std::shared_ptr<AircraftVariable> consoleLightFoLevel;
  std::shared_ptr<AircraftVariable> ecamUpperLightLevel;
  std::shared_ptr<AircraftVariable> ecamLowerLightLevel;
  std::shared_ptr<AircraftVariable> floodPnlLightLevel;
  std::shared_ptr<AircraftVariable> pedestalIntegralLightLevel;
  std::shared_ptr<AircraftVariable> floodPedLightLevel;

  // Lighting values
  LightingValues localLightValues{};

public:

  LightingPresets() = delete;

  /**
 * Creates a new LightingPresets instance and takes a reference to the MsfsHandler instance.
 * @param msfsHandler The MsfsHandler instance that is used to communicate with the simulator.
 */
  explicit LightingPresets(MsfsHandler* msfsHandler);

  bool initialize() override;
  bool preUpdate(sGaugeDrawData* pData) override;
  bool update(sGaugeDrawData* pData) override;
  bool postUpdate(sGaugeDrawData* pData) override;
  bool shutdown() override;

  /**
 * Produces a string with the current settings and their values.
 * @return string with the current settings and their values.
 */
  [[nodiscard]]
  std::string sprint() const;

private:
  /**
   * Loads a specified preset
   * @param loadPresetRequest the number of the preset to be loaded
   */
  void loadLightingPreset(int64_t loadPresetRequest);

  /**
   * Save a specified preset
   * @param savePresetRequest the number of the preset to be saved
   */
  void saveLightingPreset(int64_t savePresetRequest);

  /**
   * Read the current lighting level from the aircraft.
   */
  void readFromAircraft();

  /**
   * Applies the currently loaded preset to the aircraft
   */
  void applyToAircraft();

  /**
   * Reads a stored preset from the persistence store.
   * @return true if successful, false otherwise.
   */
  bool readFromStore(int64_t presetNr);

  /**
   * Stores the current values into the persistent store.
   * @return true if successful, false otherwise.
   */
  bool saveToStore(int64_t presetNr);

  /**
   * Load lighting level based on a given LightValue data structure
   * @param lv a loadFromData of LightValue data
   */
  void loadFromData(LightingValues lv);

  [[nodiscard]]
  std::shared_ptr<AircraftVariable> getLightPotentiometerVar(int index) const;

  /**
   * cabin lights in the A32NX need to be controlled by two vars
   * one for the switch position and one for the actual light
   * @param level
  */
  void setValidCabinLightValue(FLOAT64 level);

  /**
   * Convenience method to check for the existence of a key in a section and the option to
   * provide a default value in case the key does not exist.
   * Does not change the ini structure.
   * @param ini mINI::INIStructure
   * @param section section name as std::string
   * @param key key name as std::string
   * @param defaultValue a default value that is returned if the key does not exist
   * @return the value of the key or the default value if the key does not exist
   */
  static FLOAT64
  iniGetOrDefault(const mINI::INIStructure &ini, const std::string &section, const std::string &key,
                  FLOAT64 defaultValue);

  // formatter:off
  const LightingValues DEFAULT_50 = {50.0, 50.0, 50.0, 50.0, 50.0, 50.0, 50.0, 50.0, 50.0, 50.0,
                                     50.0, 50.0, 50.0, 50.0, 50.0, 0.5, 0.5, 0.5, 0.5, 50.0, 50.0,
                                     50.0, 50.0, 50.0};

  [[maybe_unused]]
  const LightingValues DEFAULT_10 = {10.0, 0.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0,
                                     10.0, 10.0, 10.0, 10.0, 10.0, 0.1, 0.1, 0.1, 0.0, 10.0, 10.0,
                                     10.0, 10.0, 10.0};

  [[maybe_unused]]
  const LightingValues DEFAULT_100 = {100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0,
                                      100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 1.0, 1.0, 1.0, 1.0,
                                      100.0, 100.0, 100.0, 100.0, 100.0};
};

#endif // FLYBYWIRE_A32NX_LIGHTINGPRESETS_H
