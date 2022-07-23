#pragma once


struct string {
    char* data;
    u32 size;
};

string new_string(u32 size) {
    string s = string{new char[size], size};
    s.data[0] = '\0';
    return s;
}

void free_string(string s) {
    delete[] s.data;
}