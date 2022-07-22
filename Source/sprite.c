#pragma once

struct Animation {

};

struct Sprite {
    vec2 dim;
    string name;
    Animation* animation;
    u32 texture;
};

void draw_sprite(Sprite* sprite) {

}
