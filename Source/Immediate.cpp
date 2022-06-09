#pragma once

#include <GL/gl.h>
#include <Vector.cpp>

void beginQuads() {
    glPushMatrix();
    glOrtho(0, MainWindow.width, MainWindow.height, 0, 0, 1);
    glBegin(GL_QUADS);
}

void flush() {
    glEnd();
    glPopMatrix();
}

void immediateQuad(vec2 o, vec2 s0, vec2 s1, Color color) {
    beginQuads();
    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(o.x, o.y);
    glVertex2f(o.x + s0.x, o.y + s0.y);
    glVertex2f(o.x + s0.x + s1.x, o.y + s0.y + s1.y);
    glVertex2f(o.x + s1.x, o.y + s1.y);
    flush();
}