#include "menu.h"
#include "display.h"
#include <stdio.h>

static void menu_no_action(){
    return;
}

static MenuItem m_menu_items[] = {
    {
        "Top Node",
        NULL,
        m_menu_items + 1,
        NULL,
        menu_no_action
    },
    {
        "Wet",
        m_menu_items + 0,
        m_menu_items + 4,
        m_menu_items + 2,
        menu_no_action
    },
    {
        "Ass",
        m_menu_items + 0,
        NULL,
        m_menu_items + 3,
        menu_no_action
    },
    {
        "Pussy",
        m_menu_items + 0,
        m_menu_items + 6,
        NULL,
        menu_no_action
    },
    {
        "Big D 1.1",
        m_menu_items + 1,
        NULL,
        m_menu_items + 5,
        menu_no_action
    },
    {
        "WAP 1.2",
        m_menu_items + 1,
        NULL,
        NULL,
        menu_no_action
    },
    {
        "Make that 3.1",
        m_menu_items + 3,
        NULL,
        m_menu_items + 7,
        menu_no_action
    },
        {
        "Pullout game 3.2",
        m_menu_items + 3,
        NULL,
        m_menu_items + 8,
        menu_no_action
    },
        {
        "Weeeaak! 3.3",
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
    display_print_heading(p_node->title);

    MenuItem * p_child;
    uint16_t children = menu_children(p_node, &p_child);

    for(uint16_t c = 0; c < children; c++){
    }
}