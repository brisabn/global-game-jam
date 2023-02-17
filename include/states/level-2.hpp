/**
 * @file game-state.hpp
 * @date 2023-01-06
 *
 * @brief Example game state. Runs a bouncing ball demo.
 *
 */

#ifndef LVL_2_HPP
#define LVL_2_HPP

#include <cmath>
#include "engine/include/generic-state.hpp"
#include "states/pause-state.hpp"
#include "states/final-state.hpp"
#include "definitions.hpp"
#include "box2d/box2d.h"
#include "audio.hpp"

#include "game-objects.hpp"
#include "player.hpp"

#define PLAYER_STEP 5

class Level2 : public pte::GenericState
{
private:
    sf::Sprite pause_button;

    // b2d
    b2World *world;

    // simulation
    std::vector<Box> boxes;
    std::vector<Box> hook_boxes;
    Player *player;

    // camera
    sf::View view;
    sf::View default_view;

    Audio *music;
    bool music_on;

    // background
    sf::Sprite tree1, tree2, background;

    // textures
    sf::Texture box_texture;
    sf::Texture roots_texture;
    sf::Texture end_texture;
    sf::Sprite end_sprite;

    sf::Texture door_texture;
    sf::Sprite door_sprite;

public:
    using GenericState::GenericState;

    void init();
    void handle_input();
    void update(float delta_time);
    void draw(float delta_time);

    // boxes
    void init_boxes();
};

#endif