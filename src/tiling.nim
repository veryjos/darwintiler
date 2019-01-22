proc getDesktopBounds(x: var int, y: var int, w: var int, h: var int) {.importc}
proc moveWindow(x: int, y: int, w: int, h: int) {.importc}

proc tileWindow*(x: int, y: int, gap: int, displayEdgeGap: int) =
  var
    dx, dy, dw, dh = 0

  getDesktopBounds(dx, dy, dw, dh)

  dx += displayEdgeGap
  dy += displayEdgeGap
  dw -= displayEdgeGap * 2
  dh -= displayEdgeGap * 2

  let
    dr = dx + dw
    db = dy + dh

  # Initialize the window location to the top left corner of the desktop
  var
    wx = dx
    wy = dy
    ww = 0
    wh = 0

  # If we're moving into one of the horizontal edges, modify the window width
  if x == -1 or x == 1:
    ww = (dw / 2).int
  else:
    ww = dw

  # If we're moving into one of the vertical edges, modify the window height
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

  # Apply gaps
  wx += gap
  wy += gap

  ww -= gap * 2
  wh -= gap * 2

  moveWindow(wx, wy, ww, wh)
