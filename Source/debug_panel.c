#pragma once


enum debug_panel_state {
    OPEN,
    CLOSED,
    OPENING,
    CLOSING
};

debug_panel_state state = CLOSED;

float x = 0;
float open_x = 350;
float toggle_rate = 3000;

void update_openness() {
    if (state == CLOSING) {
        x -= context.dt * toggle_rate;
        if (x <= 0) {
            state = CLOSED;
            x = 0;
        } 
    } else if (state == OPENING) {
        x += context.dt * toggle_rate;
        if (x >= open_x) {
            state = OPEN; 
            x = open_x;
        }
    }
} 

void debug_panel() {
    if (state == OPENING || state == CLOSING) {
        update_openness();
    }

    if (state == CLOSED) return;
 
    quad({0, 0}, {x, 0}, {0, (float)MainWindow.height}, {0.4, 0.6, 0.7, 0.7});
    
    int close_pad = 5;
    int close_width = 15;
    if (button("", &close_pad, x - close_pad - close_width, close_pad, close_width, close_width, {1, 0, 0, 1})) {
        state = CLOSING;
    }


}

void debug_toggle() {
    if (state == OPEN) state = CLOSING;
    else if (state == CLOSED) state = OPENING;
}
