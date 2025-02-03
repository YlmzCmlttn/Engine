#pragma once

#include "Core/Layer.h"



class ExampleLayer : public Engine::Layer {
public:
    ExampleLayer();
    ~ExampleLayer() override = default;

    void onAttach() override;
    void onDetach() override;

    void onUpdate([[maybe_unused]] Engine::Timestep ts) override;
    void onEvent([[maybe_unused]] Engine::Event& event) override;
    void onImGuiRender() override;
};