import tables, json, strUtils, hashes

proc getKeycodeForKeystring(str: cstring): int {.importc}
proc getModifierForModifierstring(str: cstring): int {.importc}

type Modifier* = enum
  None  = 0x00,

  Shift = 0x01,
  Ctrl  = 0x02,
  Alt   = 0x04,
  Cmd   = 0x08

type KeyEvent* = object
  modifiers*: int
  keyCode*: int

  debugString*: string

proc `==`(a, b: KeyEvent): bool =
  return a.keyCode == b.keyCode and a.modifiers == b.modifiers

proc hash(keyEvent: KeyEvent): Hash =
  var h: Hash = 0

  h = h !& hash(keyEvent.modifiers)
  h = h !& hash(keyEvent.keyCode)

  result = !$h

type Config* = object
  gap*: int
  displayEdgeGap*: int

  bindmap*: Table[KeyEvent, string]

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
    let modifier = getModifierForModifierstring(token)

    if modifier != 0:
      modifiers = modifiers or modifier
    else:
      keyCode = getKeycodeForKeystring(token)

  KeyEvent(
    modifiers: modifiers,
    keyCode: keyCode,
    debugString: inStr)

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
