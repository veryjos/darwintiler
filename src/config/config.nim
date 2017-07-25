import strtabs, json, strutils

{.compile: "bindings.c"}
proc keyCodeForKeyString(str: cstring): int {.importc}

type
  # I would use something like this instead of the key hash:
  #Modifier* = enum
  #  Cmd, Shift, Alt, Ctrl
  #
  #KeyEvent* = object
  #  modifiers: set[Modifier]
  #  keyCode: int

  Config* = object
    gap*: int
    displayEdgeGap*: int
    bindmap: StringTableRef

proc hasBinding*(s: Config, keyHash: string): bool =
  s.bindmap.hasKey(keyHash)

proc getBinding*(s: Config, keyHash: string): string =
  s.bindmap.getOrDefault(keyHash)

proc createKeyHash(inStr: string): string =
  var modNum = 256
  var keyNum = 0

  let tokens = split(inStr, "+")

  for token in tokens:
    case token:
      of "cmd":
        modNum = modNum or 1048840
      of "shift":
        modNum = modNum or 131330
      of "alt":
        modNum = modNum or 524576
      of "control", "ctrl":
        modNum = modNum or 262401
      else:
        keyNum = keyCodeForKeyString(token)

  return $modNum & ":" & $keyNum

proc loadConfig*(filePath: string): Config =
  let rootNode = parseFile(filePath)

  result = Config(
    gap: (rootNode["gap"].getNum(12)).int,
    displayEdgeGap: (rootNode["displayEdgeGap"].getNum(6)).int,
    bindMap: newStringTable()
  )
  for key, node in rootNode["bindings"]:
    let action = node.getStr()
    let keyHash = createKeyHash(key)
    result.bindmap[keyHash] = action
