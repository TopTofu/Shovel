#pragma once

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long

void log(char* format, ...) {
    size_t len = strlen(format);
    char* buffer = new char[len + 2];
    strcpy(buffer, format);
    buffer[len] = '\n'; // @INFO auto add new line char because i am lazy
    buffer[len+1] = '\0';

#ifdef DEV
    char* args;
    va_start(args, format);
    vprintf(buffer, args);
    va_end(args);
#else
    // @TODO might wanna do something else here
#endif

    delete[] buffer;
}

struct FileReadResult {
    unsigned int size;
    void* content;
};

FileReadResult readEntireFile(char* path) {
    FileReadResult result = {};

    FILE *file = fopen(path, "r");
    if (file == 0) {
        log("Could not open file [%s]", path);
        return result;
    }
    
    fseek(file, 0, SEEK_END);
    unsigned long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // @NOTE: anything calling must delete this pointer when done
    char* content = new char[size];
    unsigned int numRead = fread(content, 1, size, file);
    
    result.content = content;
    // @FIX: find out why size seems to always be bigger than the actual file contents
    result.size = numRead;
    
    fclose(file);

    return result;
}

char* getFileNameFromPath(char* path) {
    // "../dir/otherDir/fileName.ext" -> "fileName"

    size_t length = strlen(path);
    char *i = path + length;
    
    u32 nameEnd = 0;
    u32 nameBegin = 0;
    
    // find first '.' from behind
    for (u32 i = length; i > 0; i--) {
        if (path[i] == '.') {
            nameEnd = i;
            break;
        }
    }

    // find first '/' from behind
    for (u32 i = nameEnd; i > 0; i--){
        if (path[i] == '/') {
            // +1 to ommit the '/'
            nameBegin = i + 1;
            break;
        }
    }    

    // @NOTE: anyone calling this must delete this
    u32 nameLength = nameEnd - nameBegin;
    char* result = new char[nameLength + 1];
    result[nameLength]= '\0';
    strncpy(result, path + nameBegin, nameLength);

    return result;
}
