#pragma once
#include <Util.cpp>

bool* key_map = new bool[255];

bool get_key(char code) {
    return key_map[code];
}

void clear_key(char code) {
    key_map[code] = false;
}

enum EventType {
    KEYBOARD,
    MOUSE,
    TEXT,
};

struct Event {
    char code;
    EventType type;
    bool pressed;
    bool repeat;
    Event* next;
};

struct EventBuffer {
    Event* first;
    Event* last;
    u32 count;  
};

EventBuffer* currBuffer = 0;
EventBuffer* prevBuffer = 0;

void set_event_buffer(EventBuffer* buffer) {
    prevBuffer = currBuffer;
    currBuffer = buffer;
}

EventBuffer* listen_to_keyboard() {
    EventBuffer* result = new EventBuffer{};
    result->first = 0;
    result->last = 0;
    result->count = 0;

    set_event_buffer(result);
    return result;
}

void release_event_buffer(EventBuffer* buffer) {
    if (buffer != currBuffer) return;
    currBuffer = prevBuffer;
    prevBuffer = 0;
}

void post_event(Event* e) {
    if (!currBuffer) return;

    if (currBuffer->count <= 0) {
        e->next = 0;
        currBuffer->first = e;
        currBuffer->last = e;
        currBuffer->count = 1;
    } else {
        e->next = 0;
        currBuffer->last->next = e;
        currBuffer->last = e;
        currBuffer->count++;
    }
}

Event* get_next(EventBuffer* buffer) {
    if (buffer != currBuffer) return 0;
    if (!buffer->first) return 0;

    Event* result = buffer->first;// @TODO will these two lines interfere? will this "overwrite" result?
    buffer->first = result->next;
    
    if (buffer->first == buffer->last)
        // if the last == first we are done
        buffer->first = buffer->last = 0;

    buffer->count--;

    return result;

}


void destroy_event_buffer(EventBuffer* buffer) {
    Event* e = buffer->first;
    while(e) {
        Event* temp = e->next;
        delete e;
        e = temp;
    }

    release_event_buffer(buffer);

    delete buffer;
}