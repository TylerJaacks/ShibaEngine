//
// Created by Tyler Jaacks on 9/28/26.
//

#ifndef SHIBAENGINE_EVENT_H
#define SHIBAENGINE_EVENT_H

enum class EventType {
    None = 0,

    WindowClose,
    WindowResize,

    KeyPressed,
    KeyReleased,

    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled,
};

class Event {
public:
    virtual ~Event();

    [[nodiscard]] virtual EventType GetEventType() const = 0;
    [[nodiscard]] virtual const char* GetName() const = 0;

    bool Handled = false;
};

#endif //SHIBAENGINE_EVENT_H