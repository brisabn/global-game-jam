#include "game-objects.hpp"

Box create_box(b2World *world, float x, float y, float width, float height, float density, float friction, sf::Color color)
{
    // Body definition
    b2BodyDef boxBodyDef;
    boxBodyDef.position.Set(x / PPM, y / PPM);
    boxBodyDef.type = b2_dynamicBody;

    // Shape definition
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width / 2 / PPM, height / 2 / PPM);

    // Fixture definition
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.shape = &boxShape;

    // Now we have a body for our Box object
    b2Body *boxBody = world->CreateBody(&boxBodyDef);
    // Lastly, assign the fixture
    boxBody->CreateFixture(&fixtureDef);

    return Box{width, height, color, boxBody};
}

Box create_ground(b2World *world, float x, float y, float width, float height, sf::Color color, bool is_sensor)
{
    // Static body definition
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(x / PPM, y / PPM);

    // Shape definition
    b2PolygonShape groundBox;
    groundBox.SetAsBox(width / 2 / PPM, height / 2 / PPM);

    // Now we have a body for our Box object
    b2Body *groundBody = world->CreateBody(&groundBodyDef);
    // For a static body, we don't need a custom fixture definition, this will do:
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &groundBox;
    fixtureDef.density = 0.0f;
    fixtureDef.isSensor = is_sensor;

    groundBody->CreateFixture(&fixtureDef);

    return Box{width, height, color, groundBody};
}

void render_box(sf::RenderWindow &w, Box &box)
{
    sf::RectangleShape rect;

    // For the correct Y coordinate of our drawable rect, we must substract from WINDOW_HEIGHT
    // because SFML uses OpenGL coordinate system where X is right, Y is down
    // while Box2D uses traditional X is right, Y is up
    rect.setPosition(box.body->GetPosition().x * PPM, SCREEN_HEIGHT - (box.body->GetPosition().y * PPM));

    // We also need to set our drawable's origin to its center
    // because in SFML, "position" refers to the upper left corner
    // while in Box2D, "position" refers to the body's center
    rect.setOrigin(box.width / 2, box.height / 2);

    rect.setSize(sf::Vector2f(box.width, box.height));

    // For the rect to be rotated in the crrect direction, we have to multiply by -1
    rect.setRotation(-1 * box.body->GetAngle() * DEG_PER_RAD);

    rect.setFillColor(box.color);
    w.draw(rect);
}

void render_box_vector(sf::RenderWindow &w, std::vector<Box> &boxes)
{
    for (auto &box : boxes)
    {
        render_box(w, box);
    }
}