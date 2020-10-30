use darwintiler_common::{BackendTrait, Config};

#[cfg(target_os = "linux")]
use darwintiler_backend_x11::Backend;

fn main() {
    /// Create a new platform backend from the given config.
    let config = Config::from_cli();
    let backend = Backend::new(&config);
}
