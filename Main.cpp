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

        {
            glPushMatrix();
            glOrtho(0, MainWindow.width, MainWindow.height, 0, 0, 1);

            glBindTexture(GL_TEXTURE_2D, texture);
            glBegin(GL_QUADS);
            glColor3f(1,1,1);
            
            glTexCoord2f(0, 1);
            glVertex2f(0, 0);

            glTexCoord2f(1, 1);
            glVertex2f(bmp->header.width * scale, 0);

            glTexCoord2f(1, 0);
            glVertex2f(bmp->header.width * scale, bmp->header.height * scale);

            glTexCoord2f(0, 0);
            glVertex2f(0, bmp->header.height * scale);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
            glPopMatrix();
        }

        uiFrameBegin();
        slider(&scale, 10, 500, 200, &scale, 1, 20);
        
        default_event_handle(keyboard_events);

        debug_panel();

        uiFrameEnd();

        SwapBuffers(MainWindow.deviceContext);
    }

    return 0;
}

