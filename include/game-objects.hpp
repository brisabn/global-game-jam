/**
 * @file game-objects.hpp
 * @author mateuskrause
 * 
 * @brief 
 * 
 */

#ifndef GAME_OBJECTS_HPP
#define GAME_OBJECTS_HPP

#include <iostream>
#include "SFML/Graphics.hpp"
#include "box2d/box2d.h"
#include "definitions.hpp"

// Pixels per meter. Box2D uses metric units, so we need to define a conversion
#define PPM 30.0F
// SFML uses degrees for angles while Box2D uses radians
#define DEG_PER_RAD 57.2957795F

// A structure with all we need to render a box
struct Box
{
	float width;
	float height;
	sf::Color color;
	b2Body *body;
};

Box create_box(b2World *world, float x, float y, float width, float height, float density, float friction, sf::Color color);
Box create_ground(b2World *world, float x, float y, float width, float height, sf::Color color, bool is_sensor);

void render_box(sf::RenderWindow &w, Box &boxes, sf::Texture &texture);
void render_box_vector(sf::RenderWindow &w, std::vector<Box> &boxes, sf::Texture &texture);

#endif