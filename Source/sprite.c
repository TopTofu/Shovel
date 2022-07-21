#pragma once

struct Animation {

};

struct Sprite {
    vec2 dim;
    string name;
    Animation* animation;
    uint texture;
};



void draw_sprite(Sprite* sprite) {
    
}