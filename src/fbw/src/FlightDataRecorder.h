/*
 * A32NX
 * Copyright (C) 2020 FlyByWire Simulations and its contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <fstream>

#include "AutopilotLaws.h"
#include "AutopilotStateMachine.h"
#include "Autothrust.h"
#include "EngineData.h"
#include "FlyByWire.h"
#include "zfstream.h"

class FlightDataRecorder {
 public:
  void initialize();

  void update(AutopilotStateMachineModelClass* autopilotStateMachine,
              AutopilotLawsModelClass* autopilotLaws,
              AutothrustModelClass* autoThrust,
              FlyByWireModelClass* flyByWire,
              const EngineData& engineData);

  void terminate();

 private:
  const std::string CONFIGURATION_FILEPATH = "\\work\\FlightDataRecorder.ini";

  bool isEnabled = false;
  int sampleCounter = false;
  int maximumSampleCounter = 0;
  int maximumFileCount = 0;
  std::shared_ptr<gzofstream> fileStream;

  void manageFlightDataRecorderFiles();

  std::string getFlightDataRecorderFilename();

  void cleanUpFlightDataRecorderFiles();
};