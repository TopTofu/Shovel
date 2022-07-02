#pragma once

#include <GL/gl.h>
#include <Util.cpp>
#include <Window.cpp>

enum ColorMode {
    GRAYSCALE = 8,
    RGB = 24,
    RGBA = 32,
};

struct BitmapFontInfo {
    u16 width;
    u16 height;

    u32 cellWidth;
    u32 cellHeight;

    u32 glyphsPerRow;
    u32 glyphsPerCollumn;

    char firstCharOffset;

    ColorMode colorMode;

    char* glyphWidths;
    char* bitmap;

    char* name;

    GLuint texture;
};

#define MAX_FONTS 10

BitmapFontInfo** fonts;
int fontCount;

#define CHAR_DATA_OFFSET 276 // in BFF files this is 20 byte header + 256 byte character width info

bool loadBFF(BitmapFontInfo* font, char* path) {
    FileReadResult file = readEntireFile(path);
    
    font->name = getFileNameFromPath(path);

    u8* reader = (u8*)file.content;

    if ((int)reader[0] != 0xBF || (int)reader[1] !=0xF2) {
        log("Bitmap font file [%s] starts with wrong version number", path);
        delete[] file.content;
        return false;
    }

    memcpy(&font->width, &reader[2], sizeof(int));
    memcpy(&font->height, &reader[6], sizeof(int));
    memcpy(&font->cellWidth, &reader[10], sizeof(int));
    memcpy(&font->cellHeight, &reader[14], sizeof(int));

    font->colorMode = ColorMode(reader[18]);
    font->firstCharOffset = reader[19];

    u32 bitmapSize = font->width * font->height * font->colorMode / 8.f;
    if (file.size != bitmapSize + CHAR_DATA_OFFSET){
        log("Bitmap font file [%s] has wrong size", path);
        delete[] file.content;
        return false;
    }

    font->glyphsPerRow = font->width / font->cellWidth;
    font->glyphsPerCollumn = font->height / font->cellHeight;

    //@OPTIMIZE: we don't need all of these
    font->glyphWidths = new char[256];
    memcpy(font->glyphWidths, &reader[20], 256);

    font->bitmap = new char[bitmapSize];
    memcpy(font->bitmap, &reader[CHAR_DATA_OFFSET], bitmapSize);

    delete[] file.content;

    glGenTextures(1, &font->texture);
    glBindTexture(GL_TEXTURE_2D, font->texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    switch(font->colorMode) {
        case 32: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, font->width, font->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, font->bitmap);
        } break;

        default: {
            log("Bitmap font file [%s] had unsupported color mode (%i)", path, font->colorMode);
            glDeleteTextures(1, &font->texture);
            delete[] font->bitmap;
            return false;
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] font->bitmap;

    return true;
}

int findAllFilesInDirectory(char* directory, char** fileNames, int maxCount, char* fileFormat);

void initFonts(char* fontDirectory) {
    char** fileNames = new char*[MAX_FONTS];
    int count = findAllFilesInDirectory(fontDirectory, fileNames, MAX_FONTS, "*.bff");
    
    fonts = new BitmapFontInfo*[count];
    fontCount = 0;

    for (int i = 0; i < count; i++) {
        fonts[i] = new BitmapFontInfo;
        *fonts[i] = {};

        char* path = new char[strlen(fontDirectory) + strlen(fileNames[i]) + 1];
        strcpy(path, fontDirectory);
        strcpy(path + strlen(fontDirectory), fileNames[i]);

        if(loadBFF(fonts[i], path)) {
            fontCount++;
        }

        delete[] path;
        delete[] fileNames[i];
    }

    delete[] fileNames;
    // log("%i fonts loaded", fontCount);
}

BitmapFontInfo* getFont(char* name) {
    for (int i = 0; i < fontCount; i++) {
        if (strcmp(name, fonts[i]->name) == 0) {
            return fonts[i];
        }
    }
    return 0;
}

void drawText(BitmapFontInfo* font, char* text, u32 x, u32 y, float size, Color color) {
    int len = strlen(text);

    glPushMatrix();
    // @NOTE: BIG this lets us draw in pixel space
    glOrtho(0, MainWindow.width, MainWindow.height, 0, 0, 1);
    glBindTexture(GL_TEXTURE_2D, font->texture);

    glBegin(GL_QUADS);
    glColor4f(color.r, color.g, color.b, color.a);

    float colFactor = (float)font->cellWidth / (float)font->width;
    float rowFactor = (float)font->cellHeight / (float)font->height;

    float scaledWidth = font->cellWidth * size;
    float scaledHeight = font->cellHeight * size;
    
    for (int i = 0; i < len; i++) {
        int position = text[i] - font->firstCharOffset;

        float row = position / font->glyphsPerRow;
        float col = position - row * font->glyphsPerRow;

        float u0 = col * colFactor;
        float v0 = row * rowFactor;
        float u1 = u0 + colFactor;
        float v1 = v0 + rowFactor;

        glTexCoord2f(u0, v0); glVertex2f(x, y);
        glTexCoord2f(u1, v0); glVertex2f(x + scaledWidth, y);
        glTexCoord2f(u1, v1); glVertex2f(x + scaledWidth, y + scaledHeight);
        glTexCoord2f(u0, v1); glVertex2f(x,  y + scaledHeight);

        x += font->glyphWidths[text[i]] * size;
    }

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}


u32 stringLengthInPixels(BitmapFontInfo* font, char* text, float size) {
    u32 len = strlen(text);
    u32 result = 0;
    for (int i = 0; i < len; i++) {
        result += font->glyphWidths[text[i]] * size;
    }
    return result;
}

int getStringIndexAtPixelLength(BitmapFontInfo* font, char* text, float size, int pixel) {
    u32 len = strlen(text);
    u32 currentPixel = 0;
    for (u32 i = 0; i < len; i++) {
        currentPixel += font->glyphWidths[text[i]] * size;
        if (currentPixel > pixel) return i;
    }
    return len;
}