#include "states/game-state.hpp"

void GameState::init()
{
    window->setFramerateLimit(60);

    assets->load_texture("pause_button", PAUSE_BUTTON);
    pause_button.setTexture(assets->get_texture("pause_button"));
    pause_button.setPosition(window->getSize().x - pause_button.getLocalBounds().width - 10, pause_button.getPosition().y + 10);

    // b2d world, gravity: 9 m/s^2
    world = new b2World(b2Vec2(0, -9));

    // b2d ground
    boxes.push_back(createGround(200, 50, 300, 25, sf::Color::White));

    // player
    player = createBox(150, 100, 24, 24, 1.f, 0.7f, sf::Color::Magenta);

    // p1
    p1 = createGround(150, 400, 10, 10, sf::Color::White);
    p2 = createBox(100, 300, 10, 10, 1.f, 0.7f, sf::Color::Cyan);

    // joint p1 p2
    jointDef.Initialize(p1.body, p2.body, p1.body->GetWorldCenter(), p2.body->GetWorldCenter());
    jointDef.collideConnected = true;
    world->CreateJoint(&jointDef);
    // jointDef.SetLength(2.f);

    // // Generate a lot of boxes
    // for (int i = 0; i < 30; i++)
    // {
    // 	// Starting positions are randomly generated: x between 50 and 550, y between 70 and 550
    // 	auto &&box = createBox(50 + (std::rand() % (550 - 50 + 1)), 70 + (std::rand() % (550 - 70 + 1)), 24, 24, 1.f, 0.7f, sf::Color::White);
    // 	boxes.push_back(box);
    // }

    do
    {
        ballAngle = (std::rand() % 360) * 2 * pi / 360;
    } while (std::abs(std::cos(ballAngle)) < 0.7f);
}

void GameState::handle_input()
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (sf::Event::Closed == event.type)
        {
            window->close();
        }

        if (input->is_sprite_clicked(this->pause_button, sf::Mouse::Left, *window))
        {
            // PAUSE
            add_state<PauseState>(false);
        }

        if (sf::Event::MouseButtonPressed == event.type)
        {
            int mouse_x = sf::Mouse::getPosition(*window).x;
            int mouse_y = sf::Mouse::getPosition(*window).y;

            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // create_box(*world, mouse_x, mouse_y);
                auto &&box = createBox(mouse_x, mouse_y, 24, 24, 1.f, 0.7f, sf::Color::White);
                boxes.push_back(box);
            }
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        player.body->ApplyForceToCenter(b2Vec2(10, 1), false);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        player.body->ApplyForceToCenter(b2Vec2(-10, 1), false);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        std::cout << jointDef.length << std::endl;
        std::cout << jointDef.maxLength << std::endl;
        jointDef.maxLength = jointDef.length - 1.f;
    }
}

void GameState::update(float delta_time)
{
    // b2d
    world->Step(delta_time, 6, 2);
}

void GameState::draw(float delta_time)
{
    window->clear(sf::Color(56, 42, 55));

    // ----------------------------------------------------

    render(*window, boxes);

    // player
    sf::RectangleShape rect;
    rect.setPosition(player.body->GetPosition().x * PPM, SCREEN_HEIGHT - (player.body->GetPosition().y * PPM));
    rect.setOrigin(player.width / 2, player.height / 2);
    rect.setSize(sf::Vector2f(player.width, player.height));
    rect.setRotation(-1 * player.body->GetAngle() * DEG_PER_RAD);
    rect.setFillColor(player.color);
    window->draw(rect);

    // p1
    sf::RectangleShape rect2;
    rect2.setPosition(p1.body->GetPosition().x * PPM, SCREEN_HEIGHT - (p1.body->GetPosition().y * PPM));
    rect2.setOrigin(p1.width / 2, p1.height / 2);
    rect2.setSize(sf::Vector2f(p1.width, p1.height));
    rect2.setRotation(-1 * p1.body->GetAngle() * DEG_PER_RAD);
    rect2.setFillColor(p1.color);
    window->draw(rect2);

    // p2
    sf::RectangleShape rect3;
    rect3.setPosition(p2.body->GetPosition().x * PPM, SCREEN_HEIGHT - (p2.body->GetPosition().y * PPM));
    rect3.setOrigin(p2.width / 2, p2.height / 2);
    rect3.setSize(sf::Vector2f(p2.width, p2.height));
    rect3.setRotation(-1 * p2.body->GetAngle() * DEG_PER_RAD);
    rect3.setFillColor(p2.color);
    window->draw(rect3);

    window->draw(this->pause_button);
    window->display();
}

// -------------------------------------------

Box GameState::createBox(float x, float y, float width, float height, float density, float friction, sf::Color color)
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

Box GameState::createGround(float x, float y, float width, float height, sf::Color color)
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
    groundBody->CreateFixture(&groundBox, 0.0f);

    return Box{width, height, color, groundBody};
}

void GameState::render(sf::RenderWindow &w, std::vector<Box> &boxes)
{
    for (const auto &box : boxes)
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
}

// ------------------- b2d -------------------

// void GameState::create_ground(b2World &world, float x, float y)
// {
//     float scale = 30.f;

//     b2BodyDef *body_def = new b2BodyDef();
//     body_def->position.Set(x / 30.f, y / 30.f);
//     // body_def.type = b2_staticBody;
//     b2Body *body = world.CreateBody(body_def);

//     b2PolygonShape shape;
//     shape.SetAsBox((800.f / 2) / scale, (16.f / 2) / scale);
//     body->CreateFixture(&shape, 0.0f);

//     // b2FixtureDef fixture_def;
//     // fixture_def.density = 0.f;
//     // fixture_def.shape = &shape;
//     // body->CreateFixture(&fixture_def);

//     sf::RectangleShape *rect = new sf::RectangleShape();
//     rect->setFillColor(sf::Color::White);

//     Body *new_body = new Body();
//     new_body->b2_body = *body_def;
//     new_body->rect = *rect;
// }

// void GameState::create_box(b2World &world, int mouse_x, int mouse_y)
// {
//     //     float scale = 30.f;

//     //     b2BodyDef body_def;
//     //     body_def.position = b2Vec2(mouse_x / scale, mouse_y / scale);
//     //     body_def.type = b2_dynamicBody;
//     //     b2Body *body = world.CreateBody(&body_def);

//     //     b2PolygonShape shape;
//     //     shape.SetAsBox((32.f / 2) / scale, (32.f / 2) / scale);
//     //     b2FixtureDef fixture_def;
//     //     fixture_def.density = 1.f;
//     //     fixture_def.friction = 0.7f;
//     //     fixture_def.shape = &shape;
//     //     body->CreateFixture(&fixture_def);

//     float scale = 30.f;

//     b2BodyDef body_def;
//     body_def.position.Set(mouse_x / scale, mouse_y / scale);
//     body_def.type = b2_staticBody;
//     b2Body *body = world.CreateBody(&body_def);

//     b2PolygonShape shape;
//     shape.SetAsBox((32.f / 2) / scale, (32.f / 2) / scale);
//     b2FixtureDef fixture_def;
//     fixture_def.density = 1.f;
//     fixture_def.friction = 0.7f;
//     fixture_def.shape = &shape;
//     body->CreateFixture(&fixture_def);
// }