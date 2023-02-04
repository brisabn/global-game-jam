/**
 * @file animation-demo-state.hpp
 * @date 2023-02-03
 * 
 * @brief Animations examples
 * 
 */

#ifndef ANIMATION_DEMO_STATE_HPP
#define ANIMATION_DEMO_STATE_HPP

#include "engine/include/generic-state.hpp"
#include "engine/include/animation.hpp"
#include "states/game-state.hpp"
#include "definitions.hpp"

class AnimationDemoState : public pte::GenericState
{
private:
    pte::Animation *animation;

public:
    using GenericState::GenericState;

    void init();
    void handle_input();
    void update(float delta_time);
    void draw(float delta_time);
};

#endif