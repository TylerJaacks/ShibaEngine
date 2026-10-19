#ifndef SHIBAENGINE_LAYER_H
#define SHIBAENGINE_LAYER_H

#include "Core/Timestep.h"
#include "Events/Event.h"

class Layer {
public:
    Layer();
    virtual ~Layer();

    virtual void OnAttach();
    virtual void OnDetach();

    virtual void OnUpdate(Timestep timestep);
    virtual void OnRender();
    virtual void OnEvent(Event& event);
};

#endif //SHIBAENGINE_LAYER_H