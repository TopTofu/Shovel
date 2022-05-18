#include <stdio.h>
#include <math.h>
#include <Win32.cpp>
#include <ui.cpp>
#include <Immediate.cpp>

int main() {
    initMainWindow();

    float i = 0;

    while (checkForWindowMessage()) {
        glClearColor(0.1, 0.4, 0.4, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        beginQuads();
        immediateQuad({(float)sin(i * 0.001) * 100 + (MainWindow.width / 2.f), (float)cos(i * 0.001) * 100 + (MainWindow.height / 2.f)}, {0, 50}, {50, 0}, {1, 0.5, 0.5, 1});
        flush();
        i++;

        SwapBuffers(MainWindow.deviceContext);
    }

    return 0;
}

