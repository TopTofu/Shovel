#pragma once

char* buffer = 0;
int buffer_size = 0;
int* buffer_index = 0; // the next index to write at

void listen_to_text_input(char* text, int max_size, int* current_i) {
    buffer = text;
    buffer_size = max_size;
    buffer_index = current_i;
}

void release_buffer(char* text) {
    // give pointer to the buffer to not accidentally release a different buffer
    if (text == buffer) buffer = 0;
}

char remove_last(char* text) {
    int i = *buffer_index - 1; // this is the character we want to delete

    if (text != buffer || i < 0) 
        return 0;

    char c = buffer[i];
    buffer[i] = '\0';
    
    (*buffer_index)--;

    return c;
}

bool type_char(char c) {
    if (c == KEY_BACK) {
        remove_last(buffer);
    }

    if (!buffer || *buffer_index - 1 >= buffer_size) 
        return false;

    if (c >= 32 && c <= 126) {
        buffer[*buffer_index] = c;
        buffer[*buffer_index + 1] = '\0'; 
        (*buffer_index)++;
        return true;
    }

    return true;
}
