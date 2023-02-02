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

// Pixels per meter. Box2D uses metric units, so we need to define a conversion
#define PPM 30.0F
// SFML uses degrees for angles while Box2D uses radians
#define DEG_PER_RAD 57.2957795F

#define PLAYER_STEP 5

// A structure with all we need to render a box
struct Box
{
	float width;
	float height;
	sf::Color color;
	b2Body *body;
};

struct RayCastClosestCallback : public b2RayCastCallback {
  b2Fixture *m_closestFixture = nullptr;
  b2Vec2 m_closestPoint;
  float m_closestFraction = 1.0f;

  float ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction);
};

class GameState : public pte::GenericState
{
private:
    sf::Sprite pause_button;

    // b2d
    b2World *world;

    // simulation
    std::vector<Box> boxes;
    Box player;
    Box hook_end;
    bool hook_end_attached;
    sf::RectangleShape player_aim;
    float aim_angle;

    b2DistanceJointDef hook_joint_def;
    b2DistanceJoint* hook_joint;
    

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

    // boxes
    void init_boxes();

    // player
    void move_player_right();
    void move_player_left();
    void jump();

};

#endif