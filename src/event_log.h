#ifndef EVENT_LOG_H
#define EVENT_LOG_H 1

#include <stdint.h>
#include <libtcod.h>



// Forward declarations
typedef struct EventView EventView;



typedef struct EventMessage
{
    char        *text;
    TCOD_color_t text_color;

} EventMessage;

typedef struct EventLog
{
    size_t number_of_messages;
    size_t maximum_number_of_messages;

    EventMessage *messages;

} EventLog;



// Global variables
extern EventLog eventlog;



EventLog * eventlog_open();
void       eventlog_initialise(size_t maximum_number_of_messages);
void       eventlog_terminate();
void       eventlog_clear();
void       eventlog_print(TCOD_color_t color, char *format, ...);

#endif