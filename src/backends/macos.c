#include "../nimcache/darwintiler.h"

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

#include "macos_keycodes.h"

/**
 * Checks for accessibility API authorizaton
 */
int isAuthorized() {
  if (AXAPIEnabled() || AXIsProcessTrusted())
    return 1;

  return 0;
}

/**
 * Initializes the MacOS backend for darwintiler
 */
int initializeBackend() {
  // Check if we're authorized to run this app
  if (!isAuthorized()) {
    fprintf(stderr, "AXIsProcessTrusted returned false; does darwintiler have accessibility API permissions?\n");

    return 1;
  }

  return 0;
}

/**
 * Kills the MacOS backend for darwintiler
 */
void deinitializeBackend() {
}

/**
 * Gets the frontmost app, which we can use to get the focused window
 */
static AXUIElementRef getFrontMostApp() {
  pid_t pid;
  ProcessSerialNumber psn;

  GetFrontProcess(&psn);
  GetProcessPID(&psn, &pid);

  return AXUIElementCreateApplication(pid);
}

/**
 * Hooks a key with the specified modifiers
 */
int hookKey(int modifiers, int keycode) {
  // no-op on MacOS backend for now.
  //
  // The key event-tap just gets every single event, and
  // the "nimLandHandler" procedure filters it on its own list
  // of bindings.
  //
  // This was added because x11 requires an XGrabKey call for each
  // global hook. Maybe something similar exists on MacOS, but I'm
  // unaware of it.
  
  return 0;
}

/**
 * Gets the bounds of the active desktop, *excluding* the dock.
 */
void getDesktopBounds(int* x, int* y, int* w, int* h) {
  // Get the cursor position
  CGEventRef event = CGEventCreate(NULL);
  CGPoint cursorLocation = CGEventGetLocation(event);
  CFRelease(event);

  // Get the display which contains the cursor, that's the one we want to tile on
  int numDisplays;
  CGDirectDisplayID displays[16];
  CGGetDisplaysWithPoint(cursorLocation, 16, displays, &numDisplays);

  HIRect bounds;
  HIWindowGetAvailablePositioningBounds(displays[0], kHICoordSpace72DPIGlobal,
    &bounds);

  *x = bounds.origin.x;
  *y = bounds.origin.y;
  *w = bounds.size.width;
  *h = bounds.size.height;
}

/*
 * Moves the focused window to a specified location.
 */
void moveWindow(int x, int y, int w, int h) {
  AXValueRef temp;
  AXUIElementRef frontMostApp;
  AXUIElementRef frontMostWindow;
  CGSize windowSize;
  CGPoint windowPosition;

  windowPosition.x = x;
  windowPosition.y = y;
  windowSize.width = w;
  windowSize.height = h;

  frontMostApp = getFrontMostApp();

  AXUIElementCopyAttributeValue(
    frontMostApp, kAXFocusedWindowAttribute, (CFTypeRef *)&frontMostWindow
  );

  temp = AXValueCreate(kAXValueCGPointType, &windowPosition);
  AXUIElementSetAttributeValue(frontMostWindow, kAXPositionAttribute, temp);
  CFRelease(temp);

  temp = AXValueCreate(kAXValueCGSizeType, &windowSize);
  AXUIElementSetAttributeValue(frontMostWindow, kAXSizeAttribute, temp);
  CFRelease(temp);
}

/**
 * Returns a modifier value for the key string passed in.
 * e.g., "shift", "ctrl" || "control", "alt", "cmd" || "command"
 */
int getModifierForModifierstring(char* modifierString) {
  if (strcmp(modifierString, "shift") == 0) {
    return kCGEventFlagMaskShift;
  } else if (strcmp(modifierString, "alt") == 0) {
    return kCGEventFlagMaskAlternate;
  } else if (strcmp(modifierString, "cmd") == 0 ||
             strcmp(modifierString, "command") == 0) {
    return kCGEventFlagMaskCommand;
  } else if (strcmp(modifierString, "ctrl") == 0 ||
             strcmp(modifierString, "control") == 0) {
    return kCGEventFlagMaskControl;
  } else if (strcmp(modifierString, "fn") == 0 ||
             strcmp(modifierString, "function") == 0) {
    return kCGEventFlagMaskSecondaryFn;
  } else {
    return 0;
  }
}

/**
 * Gets the keycode for a key string..
 */
int getKeycodeForKeystring(char* keystring) {
  return keyCodeForKeyString(keystring);
}

/**
 * Dispatches events to a handler exported in the main nim file
 */
static CGEventRef eventTapDispatcher(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) {
  int repeated  = CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat);
  int keycode   = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
  int modifiers = (int)CGEventGetFlags(event);

  // TODO: Find some other way to handle this other than removing the unknown bits.
  modifiers &= (kCGEventFlagMaskShift | kCGEventFlagMaskAlternate |
                 kCGEventFlagMaskCommand | kCGEventFlagMaskControl |
                 kCGEventFlagMaskSecondaryFn);

  int override = nimLandHandler(keycode, repeated, modifiers);

  if (override) {
    return 0;
  } else {
    return event;
  }
}

/*
 * Creates a keytap from ApplicationServices
 *
 * Returns 0 on success, 1 on failure.
 */
int createAndRunEventLoop() {
  CFRunLoopSourceRef runLoopSource;
  CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);
  CFMachPortRef eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, eventTapDispatcher, 0);

  if (!eventTap) {
    return 1;
  }

  runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
  CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

  CGEventTapEnable(eventTap, true);

  CFRunLoopRun();

  return 0;
}
