// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#[cfg(not(target_arch = "wasm32"))]
use crate::msfs::legacy::execute_calculator_code;
#[cfg(target_arch = "wasm32")]
use msfs::legacy::execute_calculator_code;

use crate::flypad_backend::aircraft_procedures::AircraftProcedures;

use systems::simulation::{
    InitContext, Read, SimulationElement, SimulationElementVisitor, SimulatorReader, SimulatorWriter, UpdateContext,
    VariableIdentifier, Write,
};

pub struct AircraftPresets {
    procedures: AircraftProcedures,

    test_simvar_id: VariableIdentifier,
    test_simvar: i64,

    load_aircraft_preset_request_id: VariableIdentifier,
    load_aircraft_preset_request: i64,

    progress_aircraft_percent_id: VariableIdentifier,
    progress_aircraft_percent: f64,

    progress_aircraft_preset_id: VariableIdentifier,
    progress_aircraft_preset: i64,
}

impl AircraftPresets {
    pub fn new(context: &mut InitContext) -> AircraftPresets {
        AircraftPresets {
            procedures: AircraftProcedures {},

            test_simvar_id: context.get_identifier("TEST_SIMVAR".to_owned()),
            test_simvar: 0,

            load_aircraft_preset_request_id: context.get_identifier("AIRCRAFT_PRESET_LOAD".to_owned()),
            load_aircraft_preset_request: 0,

            progress_aircraft_percent_id: context.get_identifier("AIRCRAFT_PRESET_LOAD_PROGRESS".to_owned()),
            progress_aircraft_percent: 0.0,

            progress_aircraft_preset_id: context.get_identifier("AIRCRAFT_PRESET_LOAD_CURRENT_ID".to_owned()),
            progress_aircraft_preset: 0,
        }
    }

    pub fn update(&mut self, context: &UpdateContext) {
        self.test_simvar += 1;
        if self.test_simvar % 100 == 0 {
            println!("AircraftPresets TEST = {} ({}ms)", self.test_simvar, context.delta().as_millis());
            println!("AircraftPresets Loading Preset request {}", self.load_aircraft_preset_request);
            println!("AircraftPresets Progress Percent {}", self.progress_aircraft_percent);
            println!("AircraftPresets Progress Preset ID {}", self.progress_aircraft_preset);
            println!("AircraftPresets SIM ON GROUND {}", context.is_on_ground());
        }
    }

    fn test(&self) {
        execute_calculator_code::<()>("(A:EXTERNAL POWER ON:1, BOOL) ! if{ 1 (>K:TOGGLE_EXTERNAL_POWER) }");
    }
}

impl SimulationElement for AircraftPresets {
    fn accept<T: SimulationElementVisitor>(&mut self, visitor: &mut T) {
        visitor.visit(self);
    }

    fn read(&mut self, reader: &mut SimulatorReader) {
        self.test_simvar = reader.read(&self.test_simvar_id);
        self.load_aircraft_preset_request = reader.read(&self.load_aircraft_preset_request_id);
        self.progress_aircraft_percent = reader.read(&self.progress_aircraft_percent_id);
        self.progress_aircraft_preset = reader.read(&self.progress_aircraft_preset_id);
    }

    fn write(&self, writer: &mut SimulatorWriter) {
        writer.write(&self.test_simvar_id, self.test_simvar);
        writer.write(&self.load_aircraft_preset_request_id, self.load_aircraft_preset_request);
        writer.write(&self.progress_aircraft_percent_id, self.progress_aircraft_percent);
        writer.write(&self.progress_aircraft_preset_id, self.progress_aircraft_preset);
    }
}
