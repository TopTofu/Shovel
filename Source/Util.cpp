#pragma once

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