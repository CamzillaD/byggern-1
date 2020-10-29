#include "menu.h"
#include "display.h"
#include <stdio.h>

static void menu_no_action(){
    return;
}

static MenuItem m_menu_items[] = {
    {
        "Main Menu",
        NULL,
        m_menu_items + 1,
        NULL,
        menu_no_action
    },
    {
        "Play Game",
        m_menu_items + 0,
        NULL,
        m_menu_items + 2,
        menu_no_action
    },
    {
        "Reset scores",
        m_menu_items + 0,
        NULL,
        m_menu_items + 3,
        menu_no_action
    },
    {
        "Calibrate",
        m_menu_items + 0,
        m_menu_items + 4,
        NULL,
        menu_no_action
    },
    {
        "Joystick",
        m_menu_items + 3,
        NULL,
        m_menu_items + 5,
        menu_no_action
    },
    {
        "Slider",
        m_menu_items + 3,
        NULL,
        NULL,
        menu_no_action
    }
};

const MenuItem * menu_root_node(){
    return m_menu_items;
}

uint16_t menu_children(const MenuItem * p_node, MenuItem ** pp_child){
    if(p_node->p_child == NULL){
        return 0;
    }

    *pp_child = p_node->p_child;

    uint16_t children = 0;
    MenuItem * it = p_node->p_child;

    while(it != NULL){
        children++;
        it = it->p_sibling;
    }

    return children;
}

void menu_print(const MenuItem * p_node, uint8_t selected_level){
    display_print(0,p_node->title, 0);

    MenuItem * p_child;
    uint16_t children = menu_children(p_node, &p_child);

    display_print(1, "------------------", 0);
    for(uint16_t c = 0; c < children; c++){
        uint8_t print_arrow = (c == selected_level);
        display_print(c + 2, p_child->title, print_arrow);
        p_child++;
    }
}