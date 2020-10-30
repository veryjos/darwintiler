use crate::Config;

/// Platform agnostic backend trait.
pub trait BackendTrait<'a> {
    /// Creates a new backend of type [Self].
    fn new(config: &'a Config) -> Self where Self: Sized;
}
