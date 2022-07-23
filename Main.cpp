#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long

#include <stdio.h>
#include <math.h>
#include <string.c>
#include <keycodes.c>
#include <Win32.cpp>
#include <Input.c>
#include <ui.c>
#include <Context.c>
#include <Immediate.cpp>
#include <Font.cpp>
#include <bitmap.c>
#include <sprite.c>
#include <debug_console.c>
#include <building.c>


void default_event_handle(EventBuffer* buffer) {
    Event* e = get_next(buffer);
    while (e) {
        if (e->pressed) {
            switch(e->code) {
                case KEY_F1: {
                    debug_toggle();
                } break;
            }
        }

        delete e;
        e = get_next(buffer);
    }
}

int main() {
    initMainWindow();

    float x = 50;
    float y = 50;

    initFonts("Resources/Fonts/");
    layout.font = getFont("Bahnschrift_Regular");
    con.font = layout.font;

    EventBuffer* keyboard_events = listen_to_keyboard();

    while (checkForWindowMessage()) {
        glClearColor(0.1, 0.4, 0.4, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        default_event_handle(keyboard_events);

        uiFrameBegin();

        update_context();

        debug_console();

        uiFrameEnd();

        SwapBuffers(MainWindow.deviceContext);
    }

    return 0;
}

