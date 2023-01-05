// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

use systems::electrical::Electricity;
use systems::simulation::{
    Aircraft, InitContext, SimulationElement, SimulationElementVisitor, SimulatorReader,
    SimulatorWriter, UpdateContext,
};

pub struct ExtraBackend {}

impl ExtraBackend {
    pub fn new(context: &mut InitContext) -> ExtraBackend {
        println!("New FlyPadBackEnd: {}", context.is_on_ground());
        ExtraBackend {}
    }

    pub fn update(&mut self, context: &UpdateContext) {
        println!("Update FlyPadBackEnd: {}", context.is_on_ground());
    }
}

impl SimulationElement for ExtraBackend {
    fn accept<T: SimulationElementVisitor>(&mut self, visitor: &mut T) {
        println!("FlyPadBackend::accept");
        visitor.visit(self);
    }

    fn read(&mut self, reader: &mut SimulatorReader) {
        println!("FlyPadBackend::read");
    }

    fn write(&self, writer: &mut SimulatorWriter) {
        println!("FlyPadBackend::write");
    }
}

impl Aircraft for ExtraBackend {
    fn update_before_power_distribution(
        &mut self,
        _context: &UpdateContext,
        _electricity: &mut Electricity,
    ) {
        println!("FlyPadBackend::update_before_power_distribution");
        self.update(_context);
    }

    fn update_after_power_distribution(&mut self, _context: &UpdateContext) {
        println!("FlyPadBackend::update_after_power_distribution");
    }
}
