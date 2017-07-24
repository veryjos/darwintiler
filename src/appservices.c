#include "nimcache/darwintiler.h"

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

/**
 * Checks for accessibility API authorizaton
 */
int isAuthorized() {
  if (AXAPIEnabled() || AXIsProcessTrusted())
    return 1;

  return 0;
}

/**
 * Dispatches events to a handler exported in the main nim file
 */
static CGEventRef eventTapDispatcher(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
  int repeated = CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat);
  int keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
  int modifiers = (int)CGEventGetFlags(event);

  int override = nimLandHandler(keycode, repeated, modifiers);

  if (override) {
    return 0;
  }

  return event;
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
