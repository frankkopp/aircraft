// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

use std::error::Error;

use crate::{ExecuteOn, MsfsAspectBuilder, Variable};

use msfs::legacy::execute_calculator_code;

pub fn presets(builder: &mut MsfsAspectBuilder) -> Result<(), Box<dyn Error>> {
    Ok(())
}

fn set_light_potentiometer(index: i32, percent: f64) {
    execute_calculator_code::<()>(&format!(
        "{} {} (>K:2:LIGHT_POTENTIOMETER_SET)",
        percent, index
    ));
}

fn set_light_cabin(mut lvl: f64) {
    // cabin light level needs to either be 0, 50 or 100 for the switch position
    // in the aircraft to work
    let mut switch = "0";
    if lvl <= 0.0 {
        lvl = 0.0;
    } else if lvl > 0.0 && lvl <= 50.0 {
        lvl = 50.0;
        switch = "1";
    } else if lvl > 0.0 && lvl > 50.0 {
        lvl = 100.0;
        switch = "1";
    }
    // set the switch position via calculator code
    execute_calculator_code::<()>(&format!(
        "{} (>K:2:CABIN_LIGHTS_SET) {} (>K:LIGHT_POTENTIOMETER_7_SET)",
        switch, lvl
    ));
}
