//
// Created by Tyler Jaacks on 9/28/26.
//

#ifndef SHIBAENGINE_APPLICATIONEVENT_H
#define SHIBAENGINE_APPLICATIONEVENT_H

#include "Events/Event.h"

class WindowCloseEvent : public Event {
public:
    WindowCloseEvent();

    [[nodiscard]] EventType GetEventType() const override;
    [[nodiscard]] const char* GetName() const override;
};

class WindowResizeEvent : public Event {
public:
    WindowResizeEvent();

    [[nodiscard]] EventType GetEventType() const override;
    [[nodiscard]] const char* GetName() const override;

private:
    unsigned int m_Width;
    unsigned int m_Height;
};

#endif //SHIBAENGINE_APPLICATIONEVENT_H
