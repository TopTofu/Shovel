#pragma once

struct vec2 {
    float x; float y;
};

struct vec3 {
    float x; float y; float z;
};

vec2 add(vec2 a, vec2 b) {
    return {a.x + b.x, a.y + b.y};
}

vec2 sub(vec2 a, vec2 b) {
    return {a.x - b.x, a.y - b.y};
}

vec2 mul(vec2 a, float b) {
    return {a.x * b, a.y * b};
}

vec2 mul(vec2 a, vec2 b) {
    return {a.x * b.x, a.y * b.y};
}

vec3 add(vec3 a, vec3 b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3 sub(vec3 a, vec3 b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}



struct Color {
    float r; float g; float b; float a;
};

inline float clamp(float x, float min, float max) {
    return min > x ? min : max < x ? max : x;
}

inline Color clamp(Color c, float min, float max) {
    return {clamp(c.r, min, max), clamp(c.g, min, max), clamp(c.b, min, max), clamp(c.a, min, max)};
}

inline Color darken(Color c) {
    return {c.r * 0.65f, c.g * 0.65f, c.b * 0.65f, c.a};
}

inline Color lighten(Color c) {
    float scale = 0.5;
    return clamp({1 - (scale * (1 - c.r)), 1 - (scale * (1 - c.g)), 1 - (scale * (1 - c.b)), c.a}, 0, 1);
}