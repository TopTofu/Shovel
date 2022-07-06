#pragma once

#include <Vector.cpp>
#include <Immediate.cpp>
#include <Font.cpp>

struct InterfaceID {
    void* id;
    void* parent;
};

struct InterfaceState {
    bool rightDown, rightUp;
    bool leftDown, leftUp;
    bool middleDown, middleUp;

    int mx, my;
    int dragX, dragY;

    InterfaceID active;
    InterfaceID hot;

    InterfaceID hot_to_be; // we render the button before we call the buttonLogic function and reset the state before every frame; this would mean that we set hot in the logic function but reset it before we draw it with the hot state in mind. therefore we want to set the hot_to_be in the current frame and set it to hot at the start of the next frame. 
};

struct InterfaceLayout {
    int outlinePixels = 3;
    
    Color buttonBase = {1, 0.5, 0.5, 1};
    Color sliderBase = {0.2, 0.2, 0.2, 1};
    BitmapFontInfo* font;

    int textPadX = 5;
    int textPadY = 5;
};

InterfaceState ui;
InterfaceLayout layout;

#define anyActive() (ui.active.id != NULL)
#define isHot(id) (id == ui.hot.id)
#define isActive(id) (id == ui.active.id)

#define ID2(id) ((char*)id + 1)
#define ID3(id) ((char*)id + 2)
#define ID4(id) ((char*)id + 3)

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

// same as above but return true on the left down
bool buttonLogicDown(void *id, bool over) {
    bool result = false;

    if (!anyActive()) {
        if (over) setHot(id);
        if (isHot(id) && ui.leftDown) {
            setActive(id);
            result = true;
        }
    }

    if (isActive(id)) {
        if (over) setHot(id);
        if (ui.leftUp) clearActive();
    }

    return result;
}

bool inRect(int x, int y, int w, int h)
{
    return (ui.mx >= x && ui.mx <= x + w && ui.my >= y && ui.my <= y + h);
}

bool button(char* label, void* id, float x, float y, float w, float h) {
    quad({x, y}, {w, 0}, {0, h}, isHot(id) ? lighten(layout.buttonBase) : layout.buttonBase);
    
    if (label) 
        drawText(layout.font, label, x + layout.textPadX, y + layout.textPadY, 1, {0, 0, 0, 1});

    return buttonLogic(id, inRect(x, y, w, h));
}

bool drag(void* id, float* x, float* y, float w, float h) {
    if (buttonLogicDown(id, inRect(*x, *y, w, h))) {
        // if we press left down, ui.drag will be the offset between the cursor and the rects position
        ui.dragX = *x - ui.mx;
        ui.dragY = *y - ui.my;
    }

    if (isActive(id)) {
        // while the rect is active, move the rects position to the cursor, while considering the offset
        // and if the rect is actually moved, return true
        if (ui.mx + ui.dragX != *x || ui.my + ui.dragY != *y) {
            *x = ui.mx + ui.dragX;
            *y = ui.my + ui.dragY;
            return true;
        }
    }

    return false;
}

bool buttonOutline(char* label, void* id, float x, float y, float w, float h) {
    quadOutline({x, y}, {w, 0}, {0, h}, isHot(id) ? lighten(layout.buttonBase) : layout.buttonBase, darken(layout.buttonBase), layout.outlinePixels);
    return button(label, id, x, y, w, h);
}

bool slider(void* id, float x, float y, float w, float* value, float min, float max) {
    quad({x, y}, {0, 5}, {w, 0}, layout.sliderBase);

    float cursorH = 15;
    float cursorW = 10;

    float slideX = (*value) / (max - min) * w + x; 
    float slideY = y - cursorH / 2.f;

    // draw cursor
    quad({slideX, slideY}, {cursorW, 0}, {0, cursorH}, layout.buttonBase);

    drag(id, &slideX, &slideY, cursorW, cursorH);

    slideX = clamp(slideX, x, x + w);
    float temp = (slideX - x) * (max - min) / w;
    
    // draw value text
    bool with_text = true;
    if (with_text) {
        char* format = "%.2f";
        int len = snprintf(NULL, 0, format, *value);
        char *text = new char[len + 1];
        snprintf(text, len + 1, format, *value);
        int yOffset = 5;
        drawText(layout.font, text, slideX, y + yOffset, 1, {0, 0, 0, 1});
        delete[] text;
    }

    if (temp != *value) {
        *value = temp;
        return true;
    }

    return false;
}

void uiFrameBegin() {
    glDisable(GL_DEPTH_TEST);

    ui.hot.id = ui.hot_to_be.id;

    ui.hot_to_be.id = NULL; 
    ui.hot.parent = NULL;
}

void uiFrameEnd() {
    clearUI();
}