#include "menu.h"
#include <stdio.h>

static void menu_no_action(){
    return;
}

static MenuItem m_menu_items[] = {
    {
        "Seminole (NoMBe)",
        NULL,
        m_menu_items + 1,
        NULL,
        menu_no_action
    },
    {
        "Running to the Sea (Röyksopp)",
        m_menu_items + 0,
        NULL,
        m_menu_items + 2,
        menu_no_action
    },
    {
        "Stay Closer (ZHU)",
        m_menu_items + 0,
        m_menu_items + 4,
        m_menu_items + 3,
        menu_no_action
    },
    {
        "Battas (Mazde)",
        m_menu_items + 0,
        NULL,
        NULL,
        menu_no_action
    },
    {
        "Garden (Elder Island)",
        m_menu_items + 2,
        NULL,
        m_menu_items + 5,
        menu_no_action
    },
    {
        "Down Down (Laikipia)",
        m_menu_items + 2,
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
