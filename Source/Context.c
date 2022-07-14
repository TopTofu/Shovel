#pragma once


struct context_info {
    u32 currentT;
    u32 lastT;
    double dt;
    u32 frameCount;
};

context_info context;

void update_context() {
    FILETIME time;
    GetSystemTimeAsFileTime(&time);

    context.lastT = context.currentT;
    context.currentT = time.dwLowDateTime; 

    context.dt = (context.currentT - context.lastT) / 10000000.f;

    context.frameCount++;
};