#include "Core/Layer.h"

Layer::Layer() = default;

Layer::~Layer() = default;

void Layer::OnAttach() {

}

void Layer::OnDetach() {

}

void Layer::OnUpdate(Timestep timestep) {
    (void) timestep;
}

void Layer::OnRender() {

}

void Layer::OnEvent(Event &event) {
    (void) event;
}