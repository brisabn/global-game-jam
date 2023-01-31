/**
 * @file game-state.hpp
 * @date 2023-01-06
 *
 * @brief Example game state. Runs a bouncing ball demo.
 *
 */

#ifndef PTE_GAME_STATE_HPP
#define PTE_GAME_STATE_HPP

#include <cmath>
#include "engine/include/generic-state.hpp"
#include "states/pause-state.hpp"
#include "definitions.hpp"
#include "box2d/box2d.h"
#include "box2d/b2_rope.h"

// Pixels per meter. Box2D uses metric units, so we need to define a conversion
#define PPM 30.0F
// SFML uses degrees for angles while Box2D uses radians
#define DEG_PER_RAD 57.2957795F

// class Body
// {
// public:
//     b2BodyDef b2_body;
//     sf::RectangleShape rect;
// };

// A structure with all we need to render a box
struct Box
{
	float width;
	float height;
	sf::Color color;
	b2Body *body;
};

class GameState : public pte::GenericState
{
private:
    sf::Sprite pause_button;

    // ball
    float pi = 3.14159f;
    float ballRadius = 10.f;
    float ballSpeed = 400.f;
    float ballAngle = 0.f;
    sf::CircleShape ball;

    // b2d
    b2World *world;

    // simulation
    std::vector<Box> boxes;
    Box player;
    Box p1, p2;
    b2DistanceJointDef jointDef;
    

public:
    using GenericState::GenericState;

    void init();
    void handle_input();
    void update(float delta_time);
    void draw(float delta_time);

    // b2d
    Box createBox(float x, float y, float width, float height, float density, float friction, sf::Color color);
    Box createGround(float x, float y, float width, float height, sf::Color color);
    void render(sf::RenderWindow &w, std::vector<Box> &boxes);

};

#endif