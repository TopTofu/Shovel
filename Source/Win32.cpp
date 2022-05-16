#pragma once

#include <Vars.h>
#include <windows.h>
#include <Window.cpp>
#include <Util.cpp>

LRESULT CALLBACK MainWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(window, message, wParam, lParam);
}

void initOpenGL() {
    PIXELFORMATDESCRIPTOR pixelFormat = {};
    pixelFormat.nSize = sizeof(pixelFormat);
    pixelFormat.nVersion = 1;
    pixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pixelFormat.cColorBits = 32;
    pixelFormat.cAlphaBits = 8;
    pixelFormat.iLayerType = PFD_MAIN_PLANE;

    int suggestedPixelFormatIndex = ChoosePixelFormat(MainWindow.deviceContext, &pixelFormat);
    PIXELFORMATDESCRIPTOR suggestedPixelFormat;
    DescribePixelFormat(MainWindow.deviceContext, suggestedPixelFormatIndex, sizeof(suggestedPixelFormat), &suggestedPixelFormat);
    SetPixelFormat(MainWindow.deviceContext, suggestedPixelFormatIndex, &suggestedPixelFormat);

    MainWindow.renderContext = wglCreateContext(MainWindow.deviceContext);

    if(!wglMakeCurrent(MainWindow.deviceContext, MainWindow.renderContext)) {
        log("Could not make the OpenGL context current");
        return;
    }

    gladLoadGL();

    glViewport(0, 0, MainWindow.width, MainWindow.height);

    glEnable(GL_DEPTH_TEST); //@TODO for a 2D game i think this shouldnt be enabled since we might want to sort the z order ourselfs
    glDepthFunc(GL_LESS);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
}

void initMainWindow(HINSTANCE instance) {
    MainWindow = {};

    MainWindow.width = DEFAULT_WINDOW_WIDTH;
    MainWindow.height = DEFAULT_WINDOW_HEIGHT;

    WNDCLASSEX windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.hInstance = instance;
    windowClass.lpfnWndProc = MainWindowProc;
    windowClass.lpszClassName = "ShovelWindow";

    if (!RegisterClassEx(&windowClass)) {
        log("Could not register the window class! Code: %i", GetLastError());
        return;
    }

    // @INFO need to adjust the window rect since it's dimensions include the border and that messes slightly with the viewport
    // @NOTE when creating a fullscreen window this is not necessary
    RECT rc = {0, 0, MainWindow.width, MainWindow.height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    MainWindow.handle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, windowClass.lpszClassName, "Shovel", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);

    if (!MainWindow.handle) {
        log("Could not create the main window! Code: %i", GetLastError());
        return;
    }

    MainWindow.deviceContext = GetDC(MainWindow.handle);
    initOpenGL();

    ShowWindow(MainWindow.handle, 1); //@NOTE might want to change to fullscreen
    UpdateWindow(MainWindow.handle);

    // TRACKMOUSEEVENT toTrack = {};
    // toTrack.cbSize = sizeof(TRACKMOUSEEVENT);
    // toTrack.dwFlags = TME_LEAVE;
    // toTrack.hwndTrack = MainWindow.handle;
    // TrackMouseEvent(&toTrack);
}