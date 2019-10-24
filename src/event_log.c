#include "event_log.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "utilities.h"



// Global variables
EventLog eventlog;
int32_t  was_eventlog_init = 0;



void eventmessage_free_text(EventMessage *message)
{
    free(message->text);
    message->text       = NULL;
    message->text_color = TCOD_white;
}


/*
EventLog * eventlog_open()
{
    assert(was_eventlog_init);
    return &eventlog;
}
*/

void eventlog_initialise(size_t maximum_number_of_messages)
{
    if (was_eventlog_init) return;
    was_eventlog_init = 1;

    eventlog.number_of_messages         = 0;
    eventlog.maximum_number_of_messages = maximum_number_of_messages;
    eventlog.messages                   = (EventMessage*) calloc(maximum_number_of_messages, sizeof(EventMessage));
}

void eventlog_terminate()
{
    if (!was_eventlog_init) return;

    eventlog_clear();
    eventlog.maximum_number_of_messages = 0;
    free(eventlog.messages);

    was_eventlog_init = 0;
}

void eventlog_clear()
{
    for (size_t i = 0; i < eventlog.number_of_messages; i++)
    {
        EventMessage *message = &eventlog.messages[i];
        eventmessage_free_text(message);
    }

    eventlog.number_of_messages = 0;
}

void eventlog_print(TCOD_color_t color, char *format, ...)
{
    // If we are full, remove the top-most message.
    if (eventlog.number_of_messages == eventlog.maximum_number_of_messages)
    {
        eventmessage_free_text(&eventlog.messages[eventlog.maximum_number_of_messages - 1]);
    }

    for (size_t i = eventlog.maximum_number_of_messages - 1; i > 0; i--)
    {
        eventlog.messages[i].text       = eventlog.messages[i - 1].text;
        eventlog.messages[i].text_color = eventlog.messages[i - 1].text_color;
    }


    // Format the message text.
    char  *message_text = NULL;
    {
        char buffer[1024];
        va_list arguments;
        va_start(arguments, format);
        vsnprintf(buffer, 1023, format, arguments);
        va_end(arguments);

        size_t message_length = strlen(buffer);
        message_text = (char*) calloc(message_length + 1, sizeof(char));

        memcpy(message_text, buffer, message_length * sizeof(char));
    }

    // Finally, populate the bottom-most message.
    eventlog.messages[0].text       = message_text;
    eventlog.messages[0].text_color = color;

    if (eventlog.number_of_messages < eventlog.maximum_number_of_messages)
    {
        eventlog.number_of_messages += 1;
    }
}