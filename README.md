# darwintiler
Super simple tiling helper for MacOS (and now x11/Linux), inspired by QuickTile.

![Screenshot1](https://user-images.githubusercontent.com/11465187/28549014-743e56d4-70a5-11e7-8285-bdc460dd984a.png "Screenshot 1")

## Building
darwintiler is written in nim and c. There are no external dependencies.

All you need is the [nim compiler](http://nim-lang.org/) and a c compiler.

The binary will be placed in the "build" folder.

```bash
cd darwintiler
make release
```

## Configuration and Usage
Create a configuration file in ~/.darwintiler.json

A complete example of the configuration file is below.

```json
{
  "gap": 12,
  "displayEdgeGap": 6,

  "bindings": {
    "cmd+shift+u": "TopLeft",
    "cmd+shift+i": "Top",
    "cmd+shift+o": "TopRight",
    "cmd+shift+j": "Left",
    "cmd+shift+k": "Middle",
    "cmd+shift+l": "Right",
    "cmd+shift+m": "BottomLeft",
    "cmd+shift+,": "Bottom",
    "cmd+shift+.": "BottomRight"
  }
}
```

The focused window can be tiled using the specified hotkeys:
![Screenshot2](https://user-images.githubusercontent.com/11465187/28549696-ddc6740c-70a9-11e7-82db-1307a2bfb8df.gif "Screenshot 2")

#### MacOS Permissions
On MacOS, this program requires you to allow explicit access to the Apple Accessibility Services API (Preferences -> Security and Privacy).

On MacOS Sierra, you can no longer add CLI apps to this dialog, so you should either use [macappshell](https://github.com/Xeoncross/macappshell) or similar program to create a proper .app (alternatively, you can add your entire terminal to this list and always invoke darwintiler from your terminal (not recommended for security reasons)).

Background Image: [Kuvshinov Ilya](http://kuvshinov-ilya.deviantart.com/)
