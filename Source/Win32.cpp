#pragma once

#include <Vars.h>
#include <windows.h>
#include <Window.cpp>
#include <Util.cpp>
#include <text_input.c>
#include <Input.c>
#include <ui.c>

LRESULT CALLBACK MainWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_SIZE: {
            MainWindow.width = LOWORD(lParam);
            MainWindow.height = HIWORD(lParam);
            glViewport(0, 0, MainWindow.width, MainWindow.height);
        } break;

        case WM_CLOSE: {
            PostQuitMessage(0); // this will run into our checkForWindowMessage method which results in the main loop to end
        } break;

        case WM_KEYDOWN: {
            key_map[wParam] = true;
        } break;

        case WM_KEYUP: {
            key_map[wParam] = false;
        } break;

        case WM_CHAR: {
            type_char(wParam);
        } break;

        case WM_MOUSEMOVE: {
            ui.mx = LOWORD(lParam);
            ui.my = HIWORD(lParam);
        } break;

        case WM_LBUTTONDOWN: {
            ui.leftDown = true;
        } break;

        case WM_LBUTTONUP: {
            ui.leftUp = true;
        } break;

        case WM_RBUTTONDOWN: {

        } break;

        case WM_RBUTTONUP: {

        } break;

        case WM_MBUTTONDOWN: {

        } break;

        case WM_MBUTTONUP: {

        } break;
        
        default: {
           return DefWindowProc(window, message, wParam, lParam); 
        }
    }

    return 0;
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

    // glEnable(GL_DEPTH_TEST); //@TODO for a 2D game i think this shouldnt be enabled since we might want to sort the z order ourselves
    // glDepthFunc(GL_LESS);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
}

void initMainWindow() {
    HINSTANCE instance = GetModuleHandle(0);

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

bool checkForWindowMessage() {
    MSG message;

    PeekMessage(&message, NULL, 0, 0, PM_REMOVE);

    TranslateMessage(&message);
    DispatchMessage(&message);

    if (message.message == WM_QUIT) return false;
    
    return true;
}

int findAllFilesInDirectory(char* directory, char** fileNames, int maxCount, char* fileFormat = "*.*") {
    WIN32_FIND_DATA data;

    char* dir = new char[strlen(directory) + strlen(fileFormat)];
    strcpy(dir, directory);
    strcpy(dir + strlen(directory), fileFormat);

    HANDLE find = FindFirstFile(dir, &data);
    delete[] dir;

    int i = 0;
    if (find != INVALID_HANDLE_VALUE) {
        do {
            int len = strlen(data.cFileName);
            fileNames[i] = new char[len + 1];
            strcpy(fileNames[i], data.cFileName);
            i++;
        } while (FindNextFile(find, &data) && i < maxCount);
        FindClose(find);
    }

    return i;
}