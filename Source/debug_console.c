#pragma once


enum debug_console_state {
    OPEN,
    CLOSED,
    OPENING,
    CLOSING
};

struct Console_layout {
    BitmapFontInfo* font = getFont("Bahnschrift_Regular");

    Color background_color = {0.7, 0.4, 0.4, 0.7};
    Color input_color = {0.5, 0.25, 0.25, 1};
    
    int text_pad_x = 0;
    int text_pad_y = 0;
};

debug_console_state state = CLOSED;

float y = 0;
float open_y = 150;
float toggle_rate = 3000;

Console_layout con;

u32 history_size = 300;
int history_index = -1;
int input_index = 0;
u32 entry_size = 128;
// the last entry in the history is the string inside the input area 
string** console_history = new string*[history_size];

void update_openness() {
    if (state == CLOSING) {
        y -= context.dt * toggle_rate;
        if (y <= 0) {
            state = CLOSED;
            y = 0;
        } 
    } else if (state == OPENING) {
        y += context.dt * toggle_rate;
        if (y >= open_y) {
            state = OPEN; 
            y = open_y;
        }
    }
}

void debug_console() {
    if (state == OPENING || state == CLOSING) {
        update_openness();
    }

    if (state == CLOSED) return;
 
    quad({0, 0}, {(float)MainWindow.width, 0}, {0, y}, con.background_color);
    quad({0, y}, {(float)MainWindow.width, 0}, {0, 15}, con.input_color);
    
    if (input_index > 0) {
        string* buffer = console_history[history_index];
        char* input_string = new char[input_index + 1];

        strncpy(input_string, buffer->data, input_index);
        input_string[input_index] = '\0';

        drawText(con.font, input_string, con.text_pad_x, y + con.text_pad_y, 0.85, {0, 0, 0, 1});
    }
}

void init_history() {
    string* s = new string;
    s->data = new char[entry_size];
    s->size = entry_size;
    console_history[0] = s;
    history_index = 0;
}

void enable_console_typing() {
    if (history_index == -1) {
        init_history();
    }
    string* buffer = console_history[history_index];
    listen_to_text_input(buffer->data, buffer->size, &input_index);
}

void disable_console_typing() {
    string* buffer = console_history[history_index];
    release_buffer(buffer->data);
}

void debug_toggle() {
    if (state == OPEN) {
        state = CLOSING;
        disable_console_typing();
    } else if (state == CLOSED) {
        state = OPENING;
        enable_console_typing();
    } 
        
}
