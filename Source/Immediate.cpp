#pragma once

#include <GL/gl.h>
#include <Vector.cpp>

void begin(int type) {
    glPushMatrix();
    glOrtho(0, MainWindow.width, MainWindow.height, 0, 0, 1);
    glBegin(type);
}

void flush() {
    glEnd();
    glPopMatrix();
}

void quad(vec2 o, vec2 s0, vec2 s1, Color color) {
    begin(GL_QUADS);
    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(o.x, o.y);
    glVertex2f(o.x + s0.x, o.y + s0.y);
    glVertex2f(o.x + s0.x + s1.x, o.y + s0.y + s1.y);
    glVertex2f(o.x + s1.x, o.y + s1.y);
    flush();
}

void quadOutline(vec2 o, vec2 s0, vec2 s1, Color color, Color outline, int width = 1) {
    begin(GL_QUADS);
    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(o.x, o.y);
    glVertex2f(o.x + s0.x, o.y + s0.y);
    glVertex2f(o.x + s0.x + s1.x, o.y + s0.y + s1.y);
    glVertex2f(o.x + s1.x, o.y + s1.y);
    flush();

    glLineWidth((GLfloat)width);
    begin(GL_LINE_LOOP);
    glColor4f(outline.r, outline.g, outline.b, outline.a);
    glVertex2f(o.x, o.y - 1);
    glVertex2f(o.x + s0.x + 1, o.y + s0.y - 1);
    glVertex2f(o.x + s0.x + s1.x + 1, o.y + s0.y + s1.y);
    glVertex2f(o.x + s1.x, o.y + s1.y);
    glLineWidth(1.0f);
    flush();
}

void quad_tex(vec2 o, vec2 s0, vec2 s1, u32 texture, float scale = 1) {
    s0 = mul(s0, scale);
    s1 = mul(s1, scale);
    glBindTexture(GL_TEXTURE_2D, texture);
    begin(GL_QUADS);
    glColor3f(1, 1, 1);
    
    glTexCoord2f(0, 1);
    glVertex2f(o.x, o.y);

    glTexCoord2f(1, 1);
    glVertex2f(o.x + s0.x, o.y + s0.y);

    glTexCoord2f(1, 0);
    glVertex2f(o.x + s0.x + s1.x, o.y + s0.y + s1.y);

    glTexCoord2f(0, 0);
    glVertex2f(o.x + s1.x, o.y + s1.y);

    flush();
    glBindTexture(GL_TEXTURE_2D, 0);
}

