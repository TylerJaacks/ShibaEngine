//
// Created by Tyler Jaacks on 9/28/26.
//

#ifndef SHIBAENGINE_LAYERSTACK_H
#define SHIBAENGINE_LAYERSTACK_H

#include <Core/Layer.h>

#include <vector>

class LayerStack {
public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    void PopLayer(const Layer* layer);
    void PopOverlay(const Layer* overlay);

    std::vector<Layer*>::iterator begin();
    std::vector<Layer*>::iterator end();

private:
    std::vector<Layer*> m_layers;
    unsigned int m_layerInsertIndex;
};

#endif //SHIBAENGINE_LAYERSTACK_H