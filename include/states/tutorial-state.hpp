/**
 * @file splash-state.hpp
 * @date 2023-01-06
 * 
 * @brief Example splash state
 * 
 */

#ifndef PTE_TUTORIAL_STATE_HPP
#define PTE_TUTORIAL_STATE_HPP

#include "engine/include/generic-state.hpp"
#include "states/menu-state.hpp"
#include "states/game-state.hpp"
#include "states/intro-state.hpp"
#include "definitions.hpp"

class TutorialState : public pte::GenericState
{

public:
    using GenericState::GenericState;

    sf::Sprite resume_button;

    void init();
    void handle_input();
    void update(float delta_time);
    void draw(float delta_time);
};

#endif