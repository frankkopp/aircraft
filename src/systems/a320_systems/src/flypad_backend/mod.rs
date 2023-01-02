// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

mod aircraft_presets;
mod aircraft_procedures;

use systems::simulation::{
    InitContext, Read, SimulationElement, SimulationElementVisitor, SimulatorReader, SimulatorWriter, UpdateContext,
    VariableIdentifier, Write,
};

use self::aircraft_presets::AircraftPresets;

pub struct FlyPadBackend {
    aircraft_presets: AircraftPresets,
}

impl FlyPadBackend {
    pub fn new(context: &mut InitContext) -> FlyPadBackend {
        println!("New FlyPadBackEnd");
        FlyPadBackend { aircraft_presets: AircraftPresets::new(context) }
    }

    pub fn update(&mut self, context: &UpdateContext) {
        self.aircraft_presets.update(context);
    }
}

impl SimulationElement for FlyPadBackend {
    fn accept<T: SimulationElementVisitor>(&mut self, visitor: &mut T) {
        self.aircraft_presets.accept(visitor);
        visitor.visit(self);
    }
}
