#pragma once


struct string {
    char* data;
    int size;
};

string new_string(int size) {
    string s = string{new char[size], size};
    s.data[0] = '\0';
    return s;
}