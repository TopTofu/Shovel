#include <stdio.h>
#include <math.h>
#include <Win32.cpp>
#include <ui.c>
#include <Input.c>
#include <Context.c>
#include <Immediate.cpp>
#include <Font.cpp>

int main() {
    initMainWindow();

    float x = 50;
    float y = 50;

    initFonts("Resources/Fonts/");
    layout.font = getFont("Bahnschrift_Regular");

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
            quad({600 + ((float)i * 20), 300}, {15, 0}, {0, 15}, {0.5, 0.2, 0.7, 1});
        }

        static bool lmao = false;
        buttonToggle("on", "off", &lmao, 500, 500, 30, 30, &lmao);

        static char* text = "";
        static int n = 0;
        static bool* typing = false;
        static char* placeholder = "some input here"; 
        textInput(&text, 650, 500, 200, text, &n, &typing, placeholder);

        uiFrameEnd();

        SwapBuffers(MainWindow.deviceContext);
    }

    return 0;
}

