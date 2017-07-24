# Package

version       = "0.1.0"
author        = "Joseph Delgado"
description   = "Tiling helper for MacOS"
license       = "MIT"

srcDir        = "src"
binDir        = "build"

bin           = @["darwintiler"]

skipExt       = @["nim"]

# Dependencies

requires "nim >= 0.17.0"

