import tables
import json
import strUtils

{.compile: "bindings.c"}
proc keyCodeForKeyString(str: cstring): int {.importc}

type Config* = object
  gap*: int

  bindmap: Table[string, string]

proc hasBinding*(s: var Config, keyHash: string): bool =
  result = s.bindmap.hasKey(keyHash)

proc getBinding*(s: var Config, keyHash: string): string =
  if s.bindmap.hasKey(keyHash):
    result = s.bindmap[keyHash]
  else:
    result = ""

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
  let rootNode = parseFile("/Users/jos/.darwintiler.json")

  var config = Config(
    gap: (rootNode["gap"].getNum(12)).int,

    bindMap: initTable[string, string]()
  )
  for key, node in rootNode["bindings"]:
    let action = node.getStr()
    let keyHash = createKeyHash(key)

    config.bindmap.add(keyHash, action)

  result = config
