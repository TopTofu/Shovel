#pragma once

char* buffer = 0;
int buffer_size = 0;
int* buffer_index = 0;

void listen_to_text_input(char* text, int max_size, int* current_i) {
    buffer = text;
    buffer_size = max_size;
    buffer_index = current_i;
}

bool type_char(char c) {
    if (!buffer || *buffer_index - 1 >= buffer_size) return false;
    
    buffer[*buffer_index] = c;
    buffer[*buffer_index + 1] = '\0'; 
    (*buffer_index)++;

    return true;
}

void release_buffer(char* text) {
    // give pointer to the buffer to not accidentally release a different buffer
    if (text == buffer) buffer = 0;
}

char remove_last(char* text) {
    if (text != buffer || *buffer_index <= 0) return 0;
    char c = buffer[*buffer_index];
    buffer[*buffer_index] = '\0';
    *buffer_index--;
    return c;
}