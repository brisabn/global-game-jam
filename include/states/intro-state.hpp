#ifndef PTE_INTRO_STATE_HPP
#define PTE_INTRO_STATE_HPP

#include "engine/include/generic-state.hpp"
#include "states/game-state.hpp"
#include "definitions.hpp"

class IntroState : public pte::GenericState
{

protected:
    std::vector<sf::Sprite> intro_strips;
    std::size_t curr = 0;
    int a = 0, b = 0, c = 0;

    void intro_sequence();
    void update_intro_sequence(float delta_time);

public:
    using GenericState::GenericState;

    void init();
    void handle_input();
    void update(float delta_time);
    void draw(float delta_time);
};

#endif