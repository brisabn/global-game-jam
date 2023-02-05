/**
 * @file menu-state.hpp
 * @date 2023-01-06
 *
 * @brief Game menu state.
 *
 */

#ifndef PTE_MENU_STATE_HPP
#define PTE_MENU_STATE_HPP

#include "engine/include/generic-state.hpp"
#include "states/credits-state.hpp"
#include "states/game-state.hpp"
#include "states/intro-state.hpp"
#include "states/tutorial-state.hpp"
#include "audio.hpp"

#include "definitions.hpp"

class MainMenuState : public pte::GenericState
{
private:
    sf::Sprite play_button;
    sf::Sprite quit_button;
    sf::Sprite credits_button;
    sf::Text title;
    sf::Sprite background;
    Audio *menu_music;
    bool music_on;

public:
    using GenericState::GenericState;

    void init();
    void handle_input();
    void update(float delta_time);
    void draw(float delta_time);
};

#endif