#include <Core/LayerStack.h>

#include <algorithm>

LayerStack::LayerStack() : m_layerInsertIndex(0) {
}

LayerStack::~LayerStack() {
    for (Layer* layer : m_layers) {
        if (layer != nullptr) {
            layer->OnDetach();
            delete layer;
        }
    }

    m_layers.clear();
    m_layerInsertIndex = 0;
}

void LayerStack::PushLayer(Layer *layer) {
    if (layer == nullptr) {
        return;
    }

    m_layers.emplace(m_layers.begin() + m_layerInsertIndex++, layer);
    ++m_layerInsertIndex;

    layer->OnAttach();
}

void LayerStack::PushOverlay(Layer *overlay) {
    if (overlay == nullptr) {
        return;
    }

    m_layers.emplace(m_layers.begin() + m_layerInsertIndex, overlay);
    ++m_layerInsertIndex;

    overlay->OnAttach();
}

void LayerStack::PopLayer(const Layer *layer) {
    const auto beginIt = m_layers.begin();
    const auto endIt = m_layers.end();

    const auto it = std::find(beginIt, endIt, layer);

    if (it == endIt) {
        return;
    }

    (*it)->OnDetach();
    m_layers.erase(it);
    --m_layerInsertIndex;
}

void LayerStack::PopOverlay(const Layer *overlay) {
    const auto beginIt = m_layers.begin() + m_layerInsertIndex;
    const auto endIt = m_layers.end();

    const auto it = std::find(beginIt, endIt, overlay);

    if (it == endIt) {
        return;
    }

    (*it)->OnDetach();
    m_layers.erase(it);
}

std::vector<Layer *>::iterator LayerStack::begin() {
    return m_layers.begin();
}

std::vector<Layer *>::iterator LayerStack::end() {
    return m_layers.end();
}
