
#pragma pack(push , 1)
struct bitmap_file_header {
    WORD file_type;
    DWORD file_size;
    WORD reserved1;
    WORD reserved2;
    DWORD data_offset; // offset in bytes from the header to the data
};
#pragma pack(pop)

#pragma pack(push , 1)
struct bitmap_header {
    DWORD header_size;

    LONG width; 
    LONG height;
    
    WORD planes;
    WORD bit_per_pixel;
    DWORD compression;
    DWORD image_size;
    
    DWORD x_pixel_per_meter; 
    DWORD y_pixel_per_meter;
    
    DWORD colors_used;
    DWORD colors_important;
};
#pragma pack(pop)

struct bitmap {
    bitmap_header header;
    u8* data;
};

bitmap* load_bmp(char* path) {
    FILE *file;
    unsigned char* data;
    bitmap_file_header file_header;
    bitmap_header header;

    file = fopen(path, "rb");
    if (!file) 
        return 0;

    fread(&file_header, sizeof(bitmap_file_header), 1, file);

    if (file_header.file_type != 0x4D42) {
        fclose(file);
        return 0;
    }

    fread(&header, sizeof(bitmap_header), 1, file);

    if (header.colors_used > 0) {
        u8* colors = new u8[header.colors_used * 4];
        fread(colors, sizeof(u8), header.colors_used * 4, file);
        assert(header.bit_per_pixel == 8);
        data = new u8[header.image_size * 4]; // @NOTE this is only correct if header.bit_per_pixel == 8
        for (int i = 0; i < header.image_size; i++) {
            u8 pixel_index = i * 4;
            u8 color_index = fgetc(file);

            // swap red and blue
            data[pixel_index]     = colors[color_index + 2];
            data[pixel_index + 1] = colors[color_index + 1];
            data[pixel_index + 2] = colors[color_index];
            data[pixel_index + 3] = colors[color_index + 3];
        }
    } else {
        data = new unsigned char[header.image_size];
        fseek(file, file_header.data_offset, SEEK_SET);
        fread(data, header.image_size, 1, file);

        // swap red and blue channels (bitmaps are BGR)
        unsigned char temp;
        for (int i = 0; i < header.image_size; i+=3) {
            temp = data[i];
            data[i] = data[i + 2];
            data[i + 2] = temp;
        }
    }

    if (!data) {
        fclose(file);
        return 0;
    }


    fclose(file);

    bitmap* res = new bitmap;
    res->data = data;
    res->header = header;
    return res;
}

u32 bitmap_to_texture(bitmap* bmp) {
    u32 tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp->header.width, bmp->header.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp->data);

    // glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}