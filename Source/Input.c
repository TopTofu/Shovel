#pragma once
#include <Util.cpp>

bool* key_map = new bool[255];

#define KEY_A 65
#define KEY_B 66
#define KEY_C 67
#define KEY_D 68
#define KEY_E 69
#define KEY_F 70
#define KEY_G 71
#define KEY_H 72
#define KEY_I 73
#define KEY_J 74
#define KEY_K 75
#define KEY_L 76
#define KEY_M 77
#define KEY_N 78
#define KEY_O 79
#define KEY_P 80
#define KEY_Q 81
#define KEY_R 82
#define KEY_S 83
#define KEY_T 84
#define KEY_U 85
#define KEY_V 86
#define KEY_W 87
#define KEY_X 88
#define KEY_Y 89
#define KEY_Z 90

#define KEY_BACK 0x08
#define KEY_TAB 0x09
#define KEY_ENTER 0x0D
#define KEY_SHIFT 0x10
#define KEY_CONTROL 0x11 
#define KEY_ALT 0x12
#define KEY_CAPSLOCK 0x14 
#define KEY_ESCAPE 0x1B
#define KEY_SPACE 0x20
#define KEY_PAGEUP 0x21
#define KEY_PAGEDOWN 0x22
#define KEY_END 0x23
#define KEY_HOME 0x24

#define KEY_LEFT 0x25
#define KEY_UP 0x26
#define KEY_DOWN 0x27
#define KEY_RIGHT 0x28

#define KEY_PRINTSCREEN 0x2C
#define KEY_INSERT 0x2D
#define KEY_DELETE 0x2E
#define KEY_HELP 0x2F

#define KEY_0 0x30
#define KEY_1 0x31
#define KEY_2 0x32
#define KEY_3 0x33
#define KEY_4 0x34
#define KEY_5 0x35
#define KEY_6 0x36
#define KEY_7 0x37
#define KEY_8 0x38
#define KEY_9 0x39

#define KEY_F1 0x70
#define KEY_F2 0x71
#define KEY_F3 0x72
#define KEY_F4 0x73
#define KEY_F5 0x74
#define KEY_F6 0x75
#define KEY_F7 0x76
#define KEY_F8 0x77
#define KEY_F9 0x78
#define KEY_F10 0x79
#define KEY_F11 0x7A
#define KEY_F12 0x7B

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
    bool repeat;
    Event* next;
};


#define MAX_EVENTS 50
Event* queue = new Event[MAX_EVENTS];
int lastEvent = -1;

void post_event(Event e) {
    e.next = 0;
    if (lastEvent > -1) {
        queue[lastEvent].next = &e;
    }

    if (lastEvent < MAX_EVENTS - 1) {
        queue[++lastEvent] = e; 
    }
}