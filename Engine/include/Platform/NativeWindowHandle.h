#ifndef SHIBAENGINE_NATIVEWINDOWHANDLE_H
#define SHIBAENGINE_NATIVEWINDOWHANDLE_H

struct NativeWindowHandle {
    void* window = nullptr;
    void* view = nullptr;
    void* layer = nullptr;
};

#endif //SHIBAENGINE_NATIVEWINDOWHANDLE_H