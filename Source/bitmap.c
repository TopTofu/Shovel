
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

    LONG width; LONG height;
    
    WORD planes;
    WORD bit_per_pixel;
    
    DWORD compression;
    DWORD image_size;
    
    DWORD x_pixel_per_meter; DWORD y_pixel_per_meter;
    
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

    fseek(file, file_header.data_offset, SEEK_SET);

    data = new unsigned char[header.image_size];

    fread(data, header.image_size, 1, file);

    if (!data) {
        fclose(file);
        return 0;
    }

    // swap red and blue channels (bitmaps are BGR)
    unsigned char temp;
    for (int i = 0; i < header.image_size; i+=3) {
        temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }

    fclose(file);

    bitmap* res = new bitmap;
    res->data = data;
    res->header = header;
    return res;
}