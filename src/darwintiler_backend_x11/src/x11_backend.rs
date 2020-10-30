use darwintiler_common::{Config, BackendTrait};

/// Backend implementation for X11.
pub struct X11Backend<'a> {
    config: &'a Config,
}

impl<'a> BackendTrait<'a> for X11Backend<'a> {
    fn new(config: &'a Config) -> Self {
        X11Backend {
            config,
        }
    }
}
