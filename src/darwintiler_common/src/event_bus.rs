use shared_memory::*;

const EVENT_BUS_NAME: &'static str = "darwintiler_event_bus";
const EVENT_BUS_SIZE: usize = 4096;

/// Trait for all events to implement.
pub trait BusEvent {
    fn id(&self) -> &str;
}

/// Event bus, a small wrapper over the shared_memory crate.
pub struct EventBus {
    /// Creates a new 
    fn host(id: &str) -> EventBusHandle {
        let conf = ShmemConf::new().size(EVENT_BUS_SIZE)
            .flink(EVENT_BUS_NAME)
            .create()
            .expect("Failed to create event bus.")

        EventBusHandle
    }

    fn connect() -> EventBusHandle {
        let shmem = ShmemConf::new().size(EVENT_BUS_SIZE)
            .flink(EVENT_BUS_NAME)
            .open()
            .expect("Failed to connect to event bus, is the darwintiler server running?");

        EventBusHandle {
            shmem,
        }
    }
}

pub struct EventBusHandle {
    shmem: Shmem,
}

impl EventBusHandle {
    /// Creates a new event bus handle.
    fn new(shmem: Shmem) {
        EventBusHandle {
            shmem,
        }
    }

    /*
    /// Blocks until an event is available and returns the event.
    pub fn read_event(&self) -> T {
    }

    /// Posts a new event to the event bus.
    pub fn post_event<T: Event>(&self, T: event) {
    }
    */
}
