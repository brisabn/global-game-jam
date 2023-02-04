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
    int b2d_x = x + width / 2;
    int b2d_y = SCREEN_HEIGHT - y - height / 2;

    // Static body definition
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(b2d_x / PPM, b2d_y / PPM);

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

void render_box(sf::RenderWindow &w, Box &box, sf::Texture &texture)
{
    // set texture to repeat
    texture.setRepeated(true);

    // Create a sprite from the texture
    sf::Sprite sprite;
    sprite.setTexture(texture);

    // For the correct Y coordinate of our drawable sprite, we must substract from WINDOW_HEIGHT
    // because SFML uses OpenGL coordinate system where X is right, Y is down
    // while Box2D uses traditional X is right, Y is up
    sprite.setPosition(box.body->GetPosition().x * PPM, SCREEN_HEIGHT - (box.body->GetPosition().y * PPM));

    // We also need to set our drawable's origin to its center
    // because in SFML, "position" refers to the upper left corner
    // while in Box2D, "position" refers to the body's center
    sprite.setOrigin(box.width / 2, box.height / 2);

    // Set the size of the sprite's texture rectangle
    sprite.setTextureRect(sf::IntRect(0, 0, box.width, box.height));

    // For the sprite to be rotated in the correct direction, we have to multiply by -1
    sprite.setRotation(-1 * box.body->GetAngle() * DEG_PER_RAD);

    w.draw(sprite);
}

void render_box_vector(sf::RenderWindow &w, std::vector<Box> &boxes, sf::Texture &texture)
{
    for (auto &box : boxes)
    {
        render_box(w, box, texture);
    }
}