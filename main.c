#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>

#ifdef __clang__
#define GET_COLUMN() __builtin_COLUMN()
#else
#define GET_COLUMN() 0 // GCC doesn't support __builtin_COLUMN()
#endif

#define ERROR(msg)                                                             \
  do {                                                                         \
    fprintf(stderr, "%s:%d:%d: [ERROR] %s\n", __FILE__, __LINE__,              \
            GET_COLUMN(), msg);                                                \
  } while (0)

#define ERROR_EXIT(msg)                                                        \
  do {                                                                         \
    ERROR((msg));                                                              \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

int handle_x_errors(Display *dpy, XErrorEvent *err) {
  char error_text[1024];
  char message[2048];

  XGetErrorText(dpy, err->error_code, error_text, sizeof(error_text));

  snprintf(message, sizeof(message), "X11 Error %s (Request code: %d)\n",
           error_text, err->request_code);
  ERROR(message);

  return 0;
}

int main() {
  Display *dpy = XOpenDisplay(NULL);
  if (dpy == NULL) {
    ERROR_EXIT("xkb-layout failed to open display");
  }
  XSetErrorHandler(handle_x_errors);

  XkbStateRec xkb_state_ret;
  XkbGetState(dpy, XkbUseCoreKbd, &xkb_state_ret);

  // === Get the _XKB_RULES_NAMES property from the root window ===
  Atom prop = XInternAtom(dpy, "_XKB_RULES_NAMES", true);
  if (prop == None) {
    ERROR_EXIT("xkb-layout failed to intern property");
  }
  Atom actual_type_ret;
  int actual_format_ret;
  unsigned long size_items_ret, bytes_after_ret;
  unsigned char *data_ret = NULL;
  size_t status =
      XGetWindowProperty(dpy, DefaultRootWindow(dpy), prop, 0, 1024, False,
                         AnyPropertyType, &actual_type_ret, &actual_format_ret,
                         &size_items_ret, &bytes_after_ret, &data_ret);
  if (status != Success || data_ret == NULL) {
    ERROR_EXIT("xkb-layout failed to get window property");
  }
  unsigned long i = 0;
  int nulls_seen = 0;
  while (i < size_items_ret && nulls_seen < 2) {
    if (data_ret[i] == '\0')
      nulls_seen++;
    i++;
  }
  char *layouts = (i < size_items_ret) ? strdup((char *)&data_ret[i]) : NULL;
  XFree(data_ret);
  if (layouts == NULL) {
    ERROR_EXIT("xkb-layout failed to parse layouts from property");
  }
  // === End of property retrieval ===

  // === Find the current layout by index ===
  char *current = NULL;
  int count = 0;
  char *token = strtok(layouts, ",");
  while (token) {
    if (count == xkb_state_ret.group) {
      current = token;
      break;
    }
    count++;
    token = strtok(NULL, ",");
  }
  if (current == NULL) {
    ERROR_EXIT("xkb-layout failed to find current layout");
  }
  puts(current);
  // == End of layout finding == =

  free(layouts);
  XCloseDisplay(dpy);
  return 0;
}
