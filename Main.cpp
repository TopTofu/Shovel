#include <stdio.h>
#include <math.h>
#include <Win32.cpp>
#include <ui.c>
#include <Immediate.cpp>
#include <Font.cpp>

int main() {
    initMainWindow();

    float i = 0;
    float x = 50;
    float y = 50;

    initFonts("Resources/Fonts/");
    layout.font = getFont("Bahnschrift_Regular");

    while (checkForWindowMessage()) {
        glClearColor(0.1, 0.4, 0.4, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //immediateQuad({(float)sin(i * 0.001) * 100 + (MainWindow.width / 2.f), (float)cos(i * 0.001) * 100 + (MainWindow.height / 2.f)}, {0, 50}, {50, 0}, {1, 0.5, 0.5, 1});
        i++;

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
        
        static float someFloat = 5;
        slider(&someFloat, 100, 500, 300, &someFloat, 0, 10);

        uiFrameEnd();

        SwapBuffers(MainWindow.deviceContext);
    }

    return 0;
}

