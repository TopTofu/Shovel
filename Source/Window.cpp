#pragma once

struct Window { 
    int width;
    int height;

    HWND handle;
    HDC deviceContext;
    HGLRC renderContext;
};

Window MainWindow;