#pragma once

#include <Vector.cpp>
#include <Immediate.cpp>

struct Id {
    int index;
};

struct InterfaceState {
    Id active;
    Id hovered;
};

bool uiButton(void* id, Bool hover) {
    
}