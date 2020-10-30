use std::collections::HashMap;
use std::fs;
use std::fs::File;

use crate::CliArgs;
use crate::{ Keybind, deserialize_keybindings };

use config_struct::{Error, StructOptions};
use serde_derive::Deserialize;
use toml::Value;

/// Configuration for an instance of Darwintiler.
#[derive(Deserialize, Debug)]
pub struct Config {
    /// Gap between the display edge and all windows in pixels.
    #[serde(default)]
    pub edge_gap: i32,

    /// Gap between windows in pixels.
    #[serde(default)]
    pub gap: i32,

    #[serde(deserialize_with="deserialize_keybindings")]
    /// Represents keybindings and their associated actions.
    pub bindings: HashMap<Keybind, String>,
}

impl Config {
    /// Creates a new config from the CLI arguments given to the server.
    pub fn from_cli() -> Config {
        let args = CliArgs::from_cli();
    
        // Read config file from the path specified in the CLI arguments.
        let serialized_config = fs::read_to_string(&args.config)
            .expect("Failed to load config file.");

        toml::from_str(&serialized_config)
            .expect("Failed to parse config file.")
    }
}
