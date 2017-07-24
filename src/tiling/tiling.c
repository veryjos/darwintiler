#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

/**
 * Gets the bounds of the desktop, *excluding* the dock
 */
void getDesktopBounds(int* x, int* y, int* w, int* h) {
  HIRect bounds;
  HIWindowGetAvailablePositioningBounds(kCGNullDirectDisplay,kHICoordSpace72DPIGlobal,
    &bounds);

  *x = bounds.origin.x;
  *y = bounds.origin.y;
  *w = bounds.size.width;
  *h = bounds.size.height;
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

/*
 * Moves the focused window to a specified location.
 */
void moveWindowImpl(int x, int y, int w, int h) {
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
