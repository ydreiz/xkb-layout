#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>

#define LAYOUT_MAX_SIZE 128

typedef struct {
  char *items[LAYOUT_MAX_SIZE];
  size_t count;
} Layouts;

static unsigned char *get_window_property(Display *dpy, Window win, Atom atom,
                                          unsigned long *size) {
  Atom a_type;
  int a_format;
  unsigned long n_items, b_after;
  unsigned char *prop = NULL;

  if (XGetWindowProperty(dpy, win, atom, 0, 1024, False, AnyPropertyType,
                         &a_type, &a_format, &n_items, &b_after,
                         &prop) != Success) {
    return NULL;
  }

  if (a_type == None || n_items == 0) {
    if (prop)
      XFree(prop);
    return NULL;
  }

  if (a_format != 8) {
    if (prop)
      XFree(prop);
    return NULL;
  }

  *size = n_items;
  return prop;
}

static int parse_layouts(const unsigned char *data, unsigned long size,
                         Layouts *layouts) {
  if (data == NULL || size == 0 || layouts == NULL) {
    return 0;
  }

  unsigned long i = 0;
  int nulls_seen = 0;
  while (i < size && nulls_seen < 2) {
    if (data[i] == '\0') {
      nulls_seen++;
    }
    i++;
  }

  if (nulls_seen == 2 && i < size) {
    size_t remaining_size = size - i;
    char *layouts_str = strndup((const char *)&data[i], remaining_size);
    if (!layouts_str) {
      return 0;
    }

    char *str_ptr = layouts_str;
    char *token;
    layouts->count = 0;

    while ((token = strsep(&str_ptr, ",")) != NULL &&
           layouts->count < LAYOUT_MAX_SIZE) {
      layouts->items[layouts->count++] = strdup(token);
    }

    free(layouts_str);
    return layouts->count > 0 ? 1 : 0;
  }

  return 0;
}

static void free_layouts(Layouts *layouts) {
  for (size_t i = 0; i < layouts->count; ++i) {
    free(layouts->items[i]);
  }
  layouts->count = 0;
}

int main(void) {
  Display *dpy = XOpenDisplay(NULL);
  if (dpy == NULL) {
    fprintf(stderr, "X11: failed open display\n");
    return 1;
  }

  XkbStateRec state;
  if (XkbGetState(dpy, XkbUseCoreKbd, &state) != Success) {
    fprintf(stderr, "X11: failed get xkb state\n");
    XCloseDisplay(dpy);
    return 1;
  }
  unsigned char current_layout = state.group;

  Atom atom = XInternAtom(dpy, "_XKB_RULES_NAMES", True);
  if (atom == None) {
    fprintf(stderr, "X11: failed get atom property\n");
    XCloseDisplay(dpy);
    return 1;
  }

  unsigned long size = 0;
  unsigned char *data =
      get_window_property(dpy, DefaultRootWindow(dpy), atom, &size);

  Layouts parsed_layouts = {0};
  if (!parse_layouts(data, size, &parsed_layouts)) {
    fprintf(stderr, "X11: failed get or parse layout list\n");
    if (data)
      XFree(data);
    XCloseDisplay(dpy);
    return 1;
  }

  if (current_layout >= parsed_layouts.count) {
    fprintf(stderr, "X11: layout index out of bounds\n");
  } else {
    fprintf(stdout, "%s\n", parsed_layouts.items[current_layout]);
  }

  free_layouts(&parsed_layouts);
  if (data) {
    XFree(data);
  }

  XCloseDisplay(dpy);
  return 0;
}
