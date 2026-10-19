//
// Created by Tyler Jaacks on 9/28/26.
//

#ifndef SHIBAENGINE_KEYEVENT_H
#define SHIBAENGINE_KEYEVENT_H

#include "Events/Event.h"

class KeyEvent : public Event {
public:
    [[nodiscard]] int GetKeyCode() const;

protected:
    explicit KeyEvent(int keyCode);

    int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int keyCode, bool repeat);

    [[nodiscard]] bool IsRepeat() const;

    [[nodiscard]] EventType GetEventType() const override;
    [[nodiscard]] const char* GetName() const override;
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int keyCode, bool repeat);

    [[nodiscard]] EventType GetEventType() const override;
    [[nodiscard]] const char* GetName() const override;
};

#endif //SHIBAENGINE_KEYEVENT_H