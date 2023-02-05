/**
 * @file player.hpp
 * @author mateuskrause
 *
 * @brief
 *
 */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include "box2d/box2d.h"
#include "game-objects.hpp"

#define PLAYER_MAX_LINEAR_VELOCITY 4

// raycast callback. check for closest object
struct RayCastClosestCallback : public b2RayCastCallback
{
    b2Fixture *m_closestFixture = nullptr;
    b2Vec2 m_closestPoint;
    float m_closestFraction = 1.0f;
    b2Body *m_closestBody = nullptr;

    float ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction);
};

// everything related to player
class Player
{
private:
    Box hook_end;
    sf::RectangleShape player_aim;
    float aim_angle;
    float aim_angle_rad;

    // hook
    b2DistanceJointDef hook_joint_def;
    b2DistanceJoint *hook_joint;

    bool hook_end_attached;
    bool player_on_ground;
    bool in_action_jump;
    bool in_action_glide;
    bool is_hook_impulse_applied;

    float original_gravity_scale;

    b2World *world;

public:
    float width;
    float height;
    b2BodyDef body_def;
    b2Body *body;

    sf::RectangleShape shape;
    sf::Color color;

    Player(b2World *world, int x, int y, float width, float height, float density, float friction, sf::Color color);

    void update_player_state(sf::RenderWindow &window, sf::View &view);
    void move_player_right();
    void move_player_left();
    void action_jump_glide();
    void action_jump();
    void action_glide();
    void use_hook(sf::RenderWindow &window, std::vector<Box> &box_vec);
    void decrease_hook_length();
    void destroy_hook();

    void render_player(sf::RenderWindow &window);
    void render_player_aim(sf::RenderWindow &window);
    void render_hook(sf::RenderWindow &window);
};

#endif