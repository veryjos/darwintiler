{.compile: "tiling.c"}
proc getDesktopBounds(x: var int, y: var int, w: var int, h: var int) {.importc}
proc moveWindowImpl(x: int, y: int, w: int, h: int) {.importc}

proc tileWindow*(x: int, y: int, gap: int) =
  var dx = 0
  var dy = 0
  var dw = 0
  var dh = 0
  getDesktopBounds(dx, dy, dw, dh)

  let dr = dx + dw
  let db = dy + dh

  # Initialize the window location to the top left corner of the desktop
  var wx = dx
  var wy = dy
  var ww = 0
  var wh = 0

  if x == -1 or x == 1:
    ww = (dw / 2).int
  else:
    ww = dw

  if y == -1 or y == 1:
    wh = (dh / 2).int
  else:
    wh = dh

  wx = x * dw
  if wx < dx:
    wx = dx
  elif wx + ww > dr:
    wx = dr - ww

  wy = y * dh
  if wy < dy:
    wy = dy
  elif wy + wh > db:
    wy = db - wh

  wx += gap
  wy += gap

  ww -= gap * 2
  wh -= gap * 2

  moveWindowImpl(wx, wy, ww, wh)
