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
#include <debug_panel.c>
#include <building.c>


void default_event_handle(EventBuffer* buffer) {
    Event* e = get_next(buffer);
    while (e) {
        if (e->pressed) {
            switch(e->code) {
                case KEY_P: {
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

    EventBuffer* keyboard_events = listen_to_keyboard();

    bitmap* bmp = load_bmp("Resources/Sprites/test.bmp");
    u32 texture = bitmap_to_texture(bmp);

    while (checkForWindowMessage()) {
        glClearColor(0.1, 0.4, 0.4, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update_context();

        static float scale = 4;
        quad_tex({50, 50}, {(float)bmp->header.width, 0}, {0, (float)bmp->header.height}, texture, scale);

        uiFrameBegin();
        slider(&scale, 10, 500, 200, &scale, 1, 20);
        
        default_event_handle(keyboard_events);

        debug_panel();

        uiFrameEnd();

        SwapBuffers(MainWindow.deviceContext);
    }

    return 0;
}

