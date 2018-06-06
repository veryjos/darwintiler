import config, tiling
import tables, os, ospaths

# C Imports and linking for each platform's backend
when system.hostOS == "macosx":
  {.passL: "-framework ApplicationServices"}
  {.passL: "-framework Carbon"}

  {.compile: "backends/macos.c"}
elif system.hostOS == "linux":
  {.passL: "-lX11" }

  {.compile: "backends/x11.c"}

proc initializeBackend: cint {.importc}
proc deinitializeBackend: cint {.importc}

proc createAndRunEventLoop: cint {.importc}
proc hookKey(modifiers: int, keycode: int): cint {.importc}

var pConfig: Config

proc doAction(action: string) =
  let gap = pConfig.gap
  let displayEdgeGap = pConfig.displayEdgeGap

  case action:
    of "TopLeft":
      tileWindow(-1, -1, gap, displayEdgeGap)
    of "Top":
      tileWindow(0, -1, gap, displayEdgeGap)
    of "TopRight":
      tileWindow(1, -1, gap, displayEdgeGap)

    of "Left":
      tileWindow(-1, 0, gap, displayEdgeGap)
    of "Middle":
      tileWindow(0, 0, gap, displayEdgeGap)
    of "Right":
      tileWindow(1, 0, gap, displayEdgeGap)

    of "BottomLeft":
      tileWindow(-1, 1, gap, displayEdgeGap)
    of "Bottom":
      tileWindow(0, 1, gap, displayEdgeGap)
    of "BottomRight":
      tileWindow(1, 1, gap, displayEdgeGap)

# Handler for when keys are pressed, called from C.
# Return 1 to override, 0 to let the event pass
proc nimLandHandler(keyCode: int, repeated: int, modifiers: int): int {.exportc.} =
  let keyEvent = KeyEvent(
    modifiers: modifiers,
    keyCode: keyCode)

  if pConfig.hasBinding(keyEvent):
    # We don't want to do an action on keyrepeat...
    if repeated == 0:
      doAction(pConfig.getBinding(keyEvent))

    # ...but we still want to snuff the event
    return 1
  
  return 0

proc main() =
  let configPath = getHomeDir() / ".darwintiler.json"

  # Initialize native backend
  let initializeErr = initializeBackend()
  if initializeErr != 0:
    echo "Failed to initialize native backend"
    quit(QuitFailure)

  try:
    pConfig = loadConfig(configPath)

  except:
    echo "Couldn't read config file at ", configPath, " please create a config file"
    quit(QuitFailure)

  # Hook each key for the specific platform
  for keyEvent, _ in pConfig.bindmap:
    let hookErr = hookKey(keyEvent.modifiers, keyEvent.keycode)

    if hookErr != 0:
      echo "Failed to hook key ", keyEvent.debugString, ". Is this already hooked by some other process?"
      quit(QuitFailure)

  # Create and run the platform specific event loop
  let err = createAndRunEventLoop()

  if err != 0:
    echo "Failed to initialize event loop"
    quit(QuitFailure)

main()
