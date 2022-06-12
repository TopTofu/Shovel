#pragma once

#include <Vector.cpp>
#include <Immediate.cpp>

struct InterfaceID {
    void* id;
    void* parent;
};

struct InterfaceState {
    bool rightDown, rightUp;
    bool leftDown, leftUp;
    bool middleDown, middleUp;

    int mx, my;

    InterfaceID active;
    InterfaceID hot;

    InterfaceID hot_to_be; // we render the button before we call the buttonLogic function and reset the state before every frame; this would mean that we set hot in the logic function but reset it before we draw it with the hot state in mind. therefore we want to set the hot_to_be in the current frame and set it to hot at the start of the next frame. 
};

struct InterfaceLayout {
    int outlinePixels = 3;
    
    Color baseButton = {1, 0.5, 0.5, 1};
    Color lightButton = {1, 0.75, 0.75, 1};
};

InterfaceState ui;
InterfaceLayout layout;

#define anyActive() (ui.active.id != NULL)
#define isHot(id) (id == ui.hot.id)
#define isActive(id) (id == ui.active.id)


void clearUI() {
    ui.rightDown = ui.rightUp = false;
    ui.leftDown = ui.leftUp = false;
    ui.middleDown = ui.middleUp = false;
}

void clearActive() {
    ui.active.id = NULL;
    ui.active.parent = NULL;
    clearUI();
}

void setHot(void* id) {
    ui.hot_to_be.id = id; 
}

void setActive(void* id) {
    ui.active.id = id;
}

bool buttonLogic(void* id, bool over) {
    bool result = false;

    if (!anyActive()) {
        if (over) setHot(id);
        if (isHot(id) && ui.leftDown) setActive(id);
    }

    if (isActive(id)) {
        if (over) setHot(id); // @CLEANUP is this needed?
        if (ui.leftUp) {
            if (isHot(id)) result = true;
            clearActive();
        }
    }

    return result;
}

bool inRect(int x, int y, int w, int h)
{
   return (ui.mx >= x && ui.mx <= x + w && ui.my >= y && ui.my <= y + h);
}

bool button(char* label, void* id, float x, float y, float w, float h) {
    quad({x, y}, {w, 0}, {0, h}, isHot(id) ? layout.lightButton : layout.baseButton);

    return buttonLogic(id, inRect(x, y, w, h));
}

bool buttonOutline(char* label, void* id, float x, float y, float w, float h) {
    quadOutline({x, y}, {w, 0}, {0, h}, isHot(id) ? layout.lightButton : layout.baseButton, darken(layout.baseButton), layout.outlinePixels);

    return buttonLogic(id, inRect(x, y, w, h));
}

void uiFrameBegin() {
    ui.hot.id = ui.hot_to_be.id;

    ui.hot_to_be.id = NULL; 
    ui.hot.parent = NULL;
}

void uiFrameEnd() {
    clearUI();
}