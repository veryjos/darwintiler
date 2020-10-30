use std::collections::HashMap;
use std::iter::FromIterator;

use pest::Parser;
use pest_derive::Parser;
use serde::{Deserializer, Deserialize};

/// Represents a key binding.
#[derive(Debug, Hash, PartialEq, Eq)]
pub struct Keybind {
    /// String identifiers for each key in this keybinding.
    pub keystrings: Vec<String>,
}

/// Keybind parser implemented with pest.
#[derive(Parser)]
#[grammar_inline = r#"
  char = { !("+") ~ ANY }
  ident = { char+ }
  keybind = { ident ~ ("+"{1} ~ ident)* }
"#]
struct KeybindParserImpl;

impl KeybindParserImpl {
    fn parse_to_keybind(serialized: &str) -> Keybind {
        let keybind = KeybindParserImpl::parse(Rule::keybind, serialized)
            .expect("Failed to parse key binding")
            .next().unwrap();

        Keybind {
            keystrings: keybind.into_inner()
                .map(|ident| ident.as_str().to_owned())
                .collect(),
        }
    }
}

/// Parses a map of key bindings to strings.
pub fn deserialize_keybindings<'de, D>(deserializer: D) -> Result<HashMap<Keybind, String>, D::Error>
        where D: Deserializer<'de> {
    let map: HashMap<String, String> = HashMap::deserialize(deserializer)?;

    Ok(HashMap::from_iter(map.iter()
        .map(|(key, value)| (KeybindParserImpl::parse_to_keybind(&key), value.to_owned()))))
}
