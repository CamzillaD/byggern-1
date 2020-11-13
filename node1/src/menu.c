#include "menu.h"
#include "display.h"
#include "internode.h"
#include <stdio.h>

static uint8_t menu_no_action(){
    return 1;
}

static uint8_t menu_reset_remote(){
    internode_reset();
    return 1;
}

static uint8_t menu_start_game(){
    return 0;
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
        menu_start_game
    },
    {
        "Reset remote",
        m_menu_items + 0,
        NULL,
        NULL,
        menu_reset_remote
    }
};

const MenuItem * menu_root_node(){
    return m_menu_items;
}

uint16_t menu_children(const MenuItem * p_node, MenuItem ** pp_child){
    if(p_node->p_child == NULL){
        return 0;
    }

    if(pp_child != NULL){
        *pp_child = p_node->p_child;
    }

    uint16_t children = 0;
    MenuItem * it = p_node->p_child;

    while(it != NULL){
        children++;
        it = it->p_sibling;
    }

    return children;
}

void menu_print(const MenuItem * p_node, uint8_t selected_level){
    display_print(0, p_node->title, 0);

    MenuItem * p_child;
    uint16_t children = menu_children(p_node, &p_child);

    display_print(1, "------------------", 0);

    for(uint16_t c = 0; c < children; c++){
        uint8_t print_arrow = (c == selected_level);
        display_print(c + 2, p_child->title, print_arrow);
        p_child++;
    }
}