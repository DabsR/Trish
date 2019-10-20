#ifndef MENU_H
#define MENU_H 1

typedef struct MenuOption
{
    char *text;

} MenuOption;

typedef struct Menu
{
    char *title;

    MenuOption *options;
    size_t number_of_options;

} Menu;

#endif