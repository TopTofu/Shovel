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
#include <sprite.c>

int main() {
    initMainWindow();

    float x = 50;
    float y = 50;

    initFonts("Resources/Fonts/");
    layout.font = getFont("Bahnschrift_Regular");

    EventBuffer* keyboard_events = listen_to_keyboard();

    while (checkForWindowMessage()) {
        glClearColor(0.1, 0.4, 0.4, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update_context();

        uiFrameBegin();

        static char* myID = ":D";
        
        // layout.buttonBase = {0.3, 0.5, 1, 1};
        // drag(myID, &x, &y, 130, 50);
        // button("drag", myID, x, y, 130, 50);

        // layout.buttonBase = {1, 0.5, 0.3, 1};
        // if (button("to right", ID2(myID), 50, 300, 130, 50)) {
        //     layout.textPadX += 5;
        // }

        // if (buttonOutline("to left", ID3(myID), 300, 300, 130, 50)) {
        //     layout.textPadX -= 5;
        // }
        
        static float red = 1;
        slider(&red, 100, 400, 100, &red, 0, 1, "red");

        static float green = 1;
        slider(&green, 100, 440, 100, &green, 0, 1, "green");

        static float blue = 1;
        slider(&blue, 100, 480, 100, &blue, 0, 1, "blue");

        static float alpha = 1;
        slider(&alpha, 100, 520, 100, &alpha, 0, 1, "alpha");

        quad({30, 30}, {300, 0}, {0, 300}, {red, green, blue, alpha});
        
        static int x = 5;
        sliderInt(&x, 600, 100, 200, &x, 0, 100, "how many quads?");

        for (int i = 0; i < x; i++) {
            quad({600 + ((float)i * 20), 300}, {15, 0}, {0, 15}, {0.5, 0.2, 1, 1});
        }

        static bool lmao = false;
        buttonToggle("on", "off", &lmao, 500, 500, 30, 30, &lmao);

        static string text = new_string(30);
        static int n = 0;
        static bool active = false;
        static char* placeholder = "some input here"; 
        textInput(&text, 650, 500, 200, text, &n, &active, placeholder);

        uiFrameEnd();

        static vec2 pos = {0,0};
        Event* e = get_next(keyboard_events);
        while(e) {
            if (e->pressed) {
                switch(e->code) {
                    case KEY_LEFT: {
                        pos.x -= 5;
                    } break;
                    case KEY_RIGHT: {
                        pos.x += 5;
                    } break;
                    case KEY_UP: {
                        pos.y -= 5;
                    } break;
                    case KEY_DOWN: {
                        pos.y += 5;
                    } break;
                }
            }
            delete e;
            e = get_next(keyboard_events);
        }

        quad(pos, {0, 15}, {15, 0}, {1, 0.5, 0.5, 1});

        SwapBuffers(MainWindow.deviceContext);
    }

    return 0;
}

