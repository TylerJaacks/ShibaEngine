//
// Created by Tyler Jaacks on 9/28/26.
//

#ifndef SHIBAENGINE_MOUSEEVENT_H
#define SHIBAENGINE_MOUSEEVENT_H

#include "Events/Event.h"

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float x, float y);

    [[nodiscard]] float GetX() const;
    [[nodiscard]] float GetY() const;

    [[nodiscard]] EventType GetEventType() const override;
    [[nodiscard]] const char* GetName() const override;

private:
    float m_X;
    float m_Y;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float x, float y);

    [[nodiscard]] float GetXOffset() const;
    [[nodiscard]] float GetYOffset() const;

    [[nodiscard]] EventType GetEventType() const override;
    [[nodiscard]] const char* GetName() const override;

private:
    float m_xOffset;
    float m_yOffset;
};

class MouseButtonEvent : public Event {
public:
    [[nodiscard]] int GetMouseButton() const;

protected:
    explicit MouseButtonEvent(int button);

    int m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
    explicit MouseButtonPressedEvent(int button);

    [[nodiscard]] EventType GetEventType() const override;
    [[nodiscard]] const char* GetName() const override;
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    explicit MouseButtonReleasedEvent(int button);

    [[nodiscard]] EventType GetEventType() const override;
    [[nodiscard]] const char* GetName() const override;
};


#endif //SHIBAENGINE_MOUSEEVENT_H