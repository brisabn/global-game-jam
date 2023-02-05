/**
 * @file credits-state.hpp
 * @date 2023-01-06
 *
 * @brief Example game credits state.
 *
 */

#ifndef PTE_CREDITS_STATE_HPP
#define PTE_CREDITS_STATE_HPP

#include "engine/include/generic-state.hpp"
#include "states/menu-state.hpp"
#include "definitions.hpp"

class MainCreditsState : public pte::GenericState
{
private:
    sf::Sprite background;
    float total_time;

public:
    using GenericState::GenericState;

    void init();
    void handle_input();
    void update(float delta_time);
    void draw(float delta_time);
};

#endif