#ifndef MENU_H
#define MENU_H
#include <stdint.h>

typedef uint8_t (* MenuItemFunction)();

typedef struct MenuItem_s {
    const char * title;
    struct MenuItem_s * p_parent;
    struct MenuItem_s * p_child;
    struct MenuItem_s * p_sibling;
    MenuItemFunction action;
} MenuItem;

const MenuItem * menu_root_node();

uint16_t menu_children(const MenuItem * p_node, MenuItem ** pp_child);

void menu_print(const MenuItem * p_node, uint8_t selected_level);

#endif
