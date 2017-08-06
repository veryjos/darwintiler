import tables, json, strUtils, hashes

{.compile: "bindings.c"}
proc keyCodeForKeyString(str: cstring): int {.importc}

type Modifier* = enum
  None = 256,

  Shift = 131330,
  Ctrl = 262401,
  Alt = 524576,
  Cmd = 1048840

type KeyEvent* = object
  modifiers*: int
  keyCode*: int

proc hash(keyEvent: KeyEvent): Hash =
  var h: Hash = 0

  h = h !& hash(keyEvent.modifiers)
  h = h !& hash(keyEvent.keyCode)

  result = !$h

type Config* = object
  gap*: int
  displayEdgeGap*: int

  bindmap: Table[KeyEvent, string]

proc hasBinding*(s: Config, keyEvent: KeyEvent): bool =
  s.bindmap.hasKey(keyEvent)

proc getBinding*(s: Config, keyEvent: KeyEvent): string =
  s.bindmap.getOrDefault(keyEvent)

proc parseKeyBinding(inStr: string): KeyEvent =
  var
    modifiers = Modifier.None.int
    keyCode = 0

  let tokens = split(inStr, "+")
  
  for token in tokens:
    case token:
      of "shift":
        modifiers = modifiers or Modifier.Shift.int
      of "control", "ctrl":
        modifiers = modifiers or Modifier.Ctrl.int
      of "alt":
        modifiers = modifiers or Modifier.Alt.int
      of "cmd":
        modifiers = modifiers or Modifier.Cmd.int
      else:
        keyCode = keyCodeForKeyString(token)

  KeyEvent(
    modifiers: modifiers,
    keyCode: keyCode)

proc loadConfig*(filePath: string): Config =
  let rootNode = parseFile(filePath)

  result = Config(
    gap: (rootNode["gap"].getNum(12)).int,
    displayEdgeGap: (rootNode["displayEdgeGap"].getNum(6)).int,

    bindMap: initTable[KeyEvent, string]()
  )

  for binding, node in rootNode["bindings"]:
    let action = node.getStr()
    let keyEvent = parseKeyBinding(binding)

    result.bindmap.add(keyEvent, action)
