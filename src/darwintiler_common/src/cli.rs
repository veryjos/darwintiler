use std::env;

use clap::Clap;

use crate::Config;

const UNSET_MARKER: &'static str = "__UNSET__";

/// CLI arguments for an instance of darwintiler.
#[derive(Clap, Debug)]
#[clap(name="darwintiler")]
pub struct CliArgs {
    /// Path to config file.
    #[clap(short, long, default_value = UNSET_MARKER)]
    pub config: String,
}

impl CliArgs {
    /// Parses CLI arguments and returns an instance of [CliArgs].
    pub fn from_cli() -> CliArgs {
        let mut args = CliArgs::parse();

        if args.config == UNSET_MARKER {
            args.config = default_config_path()
        }

        args
    }
}

/// Gets the default config file path (~/.config/darwintiler.toml).
fn default_config_path() -> String {
    let config_home = env::var("XDG_CONFIG_HOME")
        .or_else(|_| env::var("HOME")
                 .map(|home_path| format!("{}/.config", home_path)))
        .unwrap();

    format!("{}/darwintiler.toml", config_home)
}
