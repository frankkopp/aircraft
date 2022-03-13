// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

use ini::Ini;
use std::{fs, fs::File};
use systems::{
    accept_iterable,
    overhead::{AutoOffFaultPushButton, OnOffFaultPushButton},
    shared::{ElectricalBusType, ElectricalBuses},
    simulation::{
        InitContext, Read, SimulationElement, SimulationElementVisitor, SimulatorReader,
        SimulatorWriter, UpdateContext, VariableIdentifier, Write,
    },
};

pub struct A320Presets {
    ini_data: Ini,

    load_preset_request: i32,
    load_preset_request_id: VariableIdentifier,

    cabin_light: i32,
    cabin_light_id: VariableIdentifier,

    overhead_integral_light: i32,
    overhead_integral_light_id: VariableIdentifier,
}

impl A320Presets {
    pub fn new(context: &mut InitContext) -> Self {
        Self {
            ini_data: Ini::new(),

            // LVAR can be used directly
            load_preset_request: 0,
            load_preset_request_id: context.get_identifier("LOAD_LIGHTING_PRESET".to_owned()),

            // Sim var needs to be added to src/systems/a320_systems_wasm/src/lib.rs
            cabin_light: 0,
            cabin_light_id: context.get_identifier("LIGHT CABIN".to_owned()),

            overhead_integral_light: 0,
            overhead_integral_light_id: context.get_identifier("LIGHT POTENTIOMETER:86".to_owned()),
        }
    }

    pub fn update(&mut self, context: &UpdateContext) {
        if self.load_preset_request > 0 {
            //
            // ///////////////////////////////////////////////
            // DEBUG: Prototype and experiments
            // ///////////////////////////////////////////////

            // do something
            for i in 0..self.load_preset_request {
                println!("{}. Loading preset: {}", i, self.load_preset_request);
            }

            // read LIGHT CABIN
            println!("Cabin Light: {}", self.cabin_light);

            // read OVHD Int Lt level
            println!("OVHD Int Lt: {}", self.overhead_integral_light);

            // set OVHD Int Lt level

            // Test to create ini and read from it
            /*
            const FILE_PATH: &'static str = "\\work/Test.ini";
            self.ini_data
                .with_section(Some("User"))
                .set("name", "Raspberry树莓")
                .set("value", "Pi");
            self.ini_data
                .with_section(Some("Library"))
                .set("name", "Sun Yat-sen U")
                .set("location", "Guangzhou=world");
            let f = self.ini_data.write_to_file(FILE_PATH);
            let f = match f {
                Ok(file) => file,
                Err(error) => println!("Problem opening the file: {:?}", error),
            };
            let i = Ini::load_from_file(FILE_PATH);
            let i = match i {
                Ok(file) => {
                    println!("Reading INI file successful!");
                    for (sec, prop) in file.iter() {
                        println!("Section: {:?}", sec);
                        for (k, v) in prop.iter() {
                            println!("{}:{}", k, v);
                        }
                    }
                }
                Err(error) => println!("Problem opening the file: {:?}", error),
            };*/

            // reset signaling LVAR
            self.load_preset_request = 0;
            println!("Loading preset {} done!", self.load_preset_request);
        }
    }
}

impl SimulationElement for A320Presets {
    fn accept<T: SimulationElementVisitor>(&mut self, visitor: &mut T) {
        visitor.visit(self);
    }

    fn read(&mut self, reader: &mut SimulatorReader) {
        self.load_preset_request = reader.read(&self.load_preset_request_id);
        self.cabin_light = reader.read(&self.cabin_light_id);
        self.overhead_integral_light = reader.read(&self.overhead_integral_light_id);
    }

    fn write(&self, writer: &mut SimulatorWriter) {
        writer.write(&self.load_preset_request_id, self.load_preset_request);
    }
}

#[cfg(test)]
mod tests {
    use crate::A320Presets;

    use systems::simulation::{
        test::{ReadByName, SimulationTestBed, TestBed, WriteByName},
        Aircraft, InitContext, SimulationElement, SimulationElementVisitor, UpdateContext,
    };

    struct PresetsTestAircraft {
        presets: A320Presets,
    }
    impl PresetsTestAircraft {
        fn new(context: &mut InitContext) -> Self {
            Self {
                presets: A320Presets::new(context),
            }
        }
    }
    impl Aircraft for PresetsTestAircraft {
        fn update_after_power_distribution(&mut self, context: &UpdateContext) {
            self.presets.update(context);
        }
    }
    impl SimulationElement for PresetsTestAircraft {
        fn accept<T: SimulationElementVisitor>(&mut self, visitor: &mut T) {
            self.presets.accept(visitor);
            visitor.visit(self);
        }
    }

    struct PresetsTestBed {
        test_bed: SimulationTestBed<PresetsTestAircraft>,
    }
    impl TestBed for PresetsTestBed {
        type Aircraft = PresetsTestAircraft;

        fn test_bed(&self) -> &SimulationTestBed<PresetsTestAircraft> {
            &self.test_bed
        }

        fn test_bed_mut(&mut self) -> &mut SimulationTestBed<PresetsTestAircraft> {
            &mut self.test_bed
        }
    }
    impl PresetsTestBed {
        fn new() -> Self {
            Self {
                test_bed: SimulationTestBed::<PresetsTestAircraft>::new(|context| {
                    PresetsTestAircraft::new(context)
                }),
            }
        }

        fn and_run(mut self) -> Self {
            self.run();
            self
        }

        fn set_request_id(mut self, id: i32) -> Self {
            self.write_by_name("LOAD_LIGHTING_PRESET", id);
            self
        }

        fn read_request_id(&mut self) -> i32 {
            self.read_by_name("LOAD_LIGHTING_PRESET")
        }
    }

    #[test]
    fn reset_request() {
        let mut test_bed = PresetsTestBed::new();
        test_bed = test_bed.set_request_id(1);
        test_bed = test_bed.and_run();

        assert_eq!(test_bed.read_request_id(), 0);
    }
}
