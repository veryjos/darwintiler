#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

static Display* display = NULL;

/**
 * Initializes the X11 backend for darwintiler
 */
int initializeBackend() {
  // Open the default display
  // TODO: Allow the config to specify which display is opened
  display = XOpenDisplay(0);

  if (!display) {
    fprintf(stderr, "Failed to open X display, are you running an X server?\n");

    return 1;
  }

  // Configure the X server to report detectable auto repeated keys
  int supported;
  XkbSetDetectableAutoRepeat(display, True, &supported);

  if (!supported) {
    fprintf(stderr, "This X server doesn't support detectable repeated key events!\n");

    return 1;
  }

  return 0;
}

/**
 * Kills the X11 backend for darwintiler
 */
void deinitializeBackend() {
  XCloseDisplay(display);
}

Window getFocusedWindow() {
  // Get the root window for the screen
  Window rootWindow = XDefaultRootWindow(display);

  // Get the active window
  Atom NET_ACTIVE_WINDOW = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);

  Atom actual_type;
  int actual_format;
  unsigned long num_items;
  unsigned long bytes_after;
  unsigned char *prop;

  int status = XGetWindowProperty(display, rootWindow,
      NET_ACTIVE_WINDOW, 0, (~0L), False, AnyPropertyType, &actual_type,
      &actual_format, &num_items, &bytes_after, &prop
  );

  if (status != Success) {
    return 0;
  }

  Window focusedWindow = *((Window*)prop);

  free(prop);

  return focusedWindow;
}

Window getRootWindow() {
  return DefaultRootWindow(display);
}

int getModifierForModifierstring(char* modifierString) {
  if (strcmp(modifierString, "shift") == 0) {
    return ShiftMask;
  } else if (strcmp(modifierString, "ctrl")    == 0 || 
             strcmp(modifierString, "control") == 0) {
    return ControlMask;
  } else if (strcmp(modifierString, "alt")  == 0 ||
             strcmp(modifierString, "mod1") == 0) {
    return Mod1Mask;
  } else if (strcmp(modifierString, "mod2")  == 0) {
    return Mod2Mask;
  } else if (strcmp(modifierString, "mod3")  == 0) {
    return Mod3Mask;
  } else if (strcmp(modifierString, "mod4")  == 0) {
    return Mod4Mask;
  } else if (strcmp(modifierString, "mod5")  == 0) {
    return Mod5Mask;
  } else {
    return 0;
  }
}

int getKeycodeForKeystring(char* keyString) {
  return XKeysymToKeycode(display, XStringToKeysym(keyString));
}

/**
 * Hooks a key with the specified modifiers
 */
int hookKey(int modifiers, int keycode) {
  Window rootWindow = getRootWindow();

  return XGrabKey(display, keycode, modifiers, rootWindow, 0, GrabModeAsync, GrabModeAsync) == 0;
}

/**
 * Gets the bounds of the desktop, excluding any docks
 */
void getDesktopBounds(int* x, int* y, int* w, int* h) {
  Window root = getRootWindow();
  Atom _NET_WORKAREA = XInternAtom(display, "_NET_WORKAREA", True);

  Atom actualType;
  int actualFormat;
  unsigned long len, bytesAfterReturn;
  Atom* geometryList;

  int status = XGetWindowProperty(
    display, root, _NET_WORKAREA, 0L, sizeof(Atom), False,
    XA_CARDINAL, &actualType, &actualFormat, &len, &bytesAfterReturn,
    (unsigned char**)&geometryList
  );

  if (status != Success || len != 4) {
    // Use _NET_DESKTOP_GEOMETRY as a fallback
    Atom _NET_DESKTOP_GEOMETRY = XInternAtom(display, "_NET_DESKTOP_GEOMETRY", True);

    int status = XGetWindowProperty(
      display, root, _NET_DESKTOP_GEOMETRY, 0L, sizeof(Atom), False,
      XA_CARDINAL, &actualType, &actualFormat, &len, &bytesAfterReturn,
      (unsigned char**)&geometryList
    );

    if (status != Success || len != 2) {
      fprintf(stderr, "Failed to query desktop position\n");

      return;
    }

    *x = 0; *w = geometryList[0];
    *y = 0; *h = geometryList[1];

    XFree(geometryList);

    return;
  }

  // Get the first geometry
  *x = geometryList[0]; *y = geometryList[1];
  *w = geometryList[2]; *h = geometryList[3];

  XFree(geometryList);
}

/**
 * Moves the focused window to the specified location
 */
int moveWindow(int x, int y, int w, int h) {
  Window focused = getFocusedWindow();

  if (!focused)
    return;

  // Get the decorator extents so we can compensate for them
  const char* atomNames[] = {
    "_GTK_FRAME_EXTENTS",
    "_NET_FRAME_EXTENTS",
    "\0"
  };

  const char** namePtr;
  for (namePtr = atomNames; namePtr[0][0] != 0; ++namePtr) {
    const char* name = *namePtr;
    Atom atom = XInternAtom(display, name, True);

    if (!atom)
      continue;

    Atom actualType;
    int actualFormat;
    unsigned long len, bytesAfterReturn;
    Atom* frameExtentsList = 0;

    int status = XGetWindowProperty(
      display, focused, atom, 0L, sizeof(Atom), False, XA_CARDINAL,
      &actualType, &actualFormat, &len, &bytesAfterReturn,
      (unsigned char**)&frameExtentsList
    );

    if (status == Success) {
      if (len >= 4) {
        if (namePtr == &atomNames[0]) {
          x -= frameExtentsList[0]; w += frameExtentsList[0];
          y -= frameExtentsList[2]; h += frameExtentsList[2];
          w += frameExtentsList[1]; h += frameExtentsList[3];
        } else {
          w -= frameExtentsList[0]; h -= frameExtentsList[2];
        }
      }

      XFree(frameExtentsList);
    }
  }

  XWindowChanges wc;
  wc.x = x;
  wc.y = y;
  wc.width = w;
  wc.height = h;

  XConfigureWindow(display, focused,
      CWX | CWY | CWWidth | CWHeight, &wc);

  XSync(display, False);
}

/**
 * Creates and runs and event loop for X
 *
 * Returns 0 on success, 1 on failure.
 */
int createAndRunEventLoop() {
  Window rootWindow = getRootWindow();
  XSelectInput(display, rootWindow, 0);

  XEvent evt;
  Bool runEventLoop = True;

  Bool autoRepeated = False;
  int override = 0;
  while (runEventLoop) {
    XNextEvent(display, &evt);

    switch (evt.type) {
      case KeyPress:
        // Doesn't matter, we always override
        override = nimLandHandler(evt.xkey.keycode, autoRepeated, evt.xkey.state);
        autoRepeated = True;

        break;

      case KeyRelease:
        autoRepeated = False;
        break;

      default:
        break;
    }
  }

  return 0;
}
