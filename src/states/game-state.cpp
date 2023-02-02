#include "states/game-state.hpp"

void GameState::init()
{
    window->setFramerateLimit(60);

    assets->load_texture("pause_button", PAUSE_BUTTON);
    pause_button.setTexture(assets->get_texture("pause_button"));
    pause_button.setPosition(window->getSize().x - pause_button.getLocalBounds().width - 10, pause_button.getPosition().y + 10);

    // b2d world, gravity: 9 m/s^2
    world = new b2World(b2Vec2(0, -9));

    // // b2d ground
    // boxes.push_back(createGround(200, 50, 300, 25, sf::Color::White));

    // player
    player = createBox(150, 130, 34, 44, 35.f, 0.7f, sf::Color::Magenta);
    // player.body->GetFixtureList()->SetFriction(5);
    hook_end_attached = false;

    // player's aim indicator
    player_aim.setSize(sf::Vector2f(10, 30));
    player_aim.setOrigin(5, 25);
    player_aim.setFillColor(sf::Color::Red);
    aim_angle = 0;

    // // p1
    // p1 = createGround(150, 400, 10, 10, sf::Color::White);
    // p2 = createBox(100, 300, 10, 10, 1.f, 0.7f, sf::Color::Cyan);

    // // joint p1 p2
    // jointDef.Initialize(p1.body, p2.body, p1.body->GetWorldCenter(), p2.body->GetWorldCenter());
    // jointDef.collideConnected = true;
    // jointDef.stiffness = 0.f;
    // jointDef.damping = 0.f;

    // joint = (b2DistanceJoint *)world->CreateJoint(&jointDef);
    // joint->SetMinLength(0.f);
    // joint->SetMaxLength(10.f);

    // all boxes in this level
    init_boxes();
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
            int mouse_y = SCREEN_HEIGHT - sf::Mouse::getPosition(*window).y;

            std::cout << "mouse: " << mouse_x << " " << mouse_y << std::endl;
        }

        if (hook_end_attached)
        {
            if (event.type == sf::Event::MouseWheelMoved)
            {
                hook_joint->SetMaxLength(hook_joint->GetMaxLength() - (0.3f * event.mouseWheel.delta));
            }
        }
    }

    if (world->GetJointCount())
    {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            hook_joint->SetMaxLength(hook_joint->GetMaxLength() - 0.1f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            hook_joint->SetMaxLength(hook_joint->GetMaxLength() + 0.1f);
        }
        // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        // {
        //     p2.body->ApplyForceToCenter(b2Vec2(10, 0), false);
        // }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        move_player_right();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        move_player_left();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        jump();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        system("clear");
        std::cout << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << std::endl;
        std::cout << player_aim.getPosition().x << " " << player_aim.getPosition().y << std::endl;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        // b2Vec2 playerPos = player.body->GetPosition();
        // playerPos *= PPM;

        if (!hook_end_attached)
        {
            b2Vec2 playerPos = player.body->GetPosition();
            playerPos *= PPM;
            int mouseX = sf::Mouse::getPosition(*window).x;
            int mouseY = SCREEN_HEIGHT - sf::Mouse::getPosition(*window).y;
            float angle_rad = atan2(mouseY - playerPos.y, mouseX - playerPos.x);
            aim_angle = angle_rad * 180.0f / b2_pi;

            // float angle_rad = angle * 180.0f / b2_pi;
            std::cout << "player: " << playerPos.x << " " << playerPos.y << std::endl;
            std::cout << "mouse: " << mouseX << " " << mouseY << std::endl;
            std::cout << "angle: " << aim_angle << std::endl;

            b2Vec2 direction(cos(angle_rad), sin(angle_rad));
            direction *= 8;
            RayCastClosestCallback callback;
            playerPos.x /= PPM;
            playerPos.y /= PPM;
            world->RayCast(&callback, playerPos, playerPos + direction);

            if (callback.m_closestFixture)
            {
                b2Vec2 closestPoint = callback.m_closestPoint;
                std::cout << "closest point: " << closestPoint.x << " " << closestPoint.y << std::endl;

                // Box b = createBox(closestPoint.x * PPM, closestPoint.y * PPM, 10, 10, 1.f, 0.7f, sf::Color::Red);
                hook_end = createGround(closestPoint.x * PPM, closestPoint.y * PPM, 10, 10, sf::Color::Green);
                // boxes.push_back(b);

                // joint
                hook_joint_def.Initialize(player.body, hook_end.body, player.body->GetWorldCenter(), hook_end.body->GetWorldCenter());
                hook_joint_def.collideConnected = true;
                hook_joint_def.stiffness = 0.f;
                hook_joint_def.damping = 0.f;

                float value = pow(closestPoint.x - player.body->GetWorldCenter().x, 2) + pow(closestPoint.y - player.body->GetWorldCenter().y, 2);
                float distance = sqrt(value);

                hook_joint = (b2DistanceJoint *)world->CreateJoint(&hook_joint_def);
                hook_joint->SetMinLength(0.f);
                hook_joint->SetLength(distance);
                hook_joint->SetMaxLength(distance);

                player.body->ApplyForceToCenter(b2Vec2(0, -1), true);

                hook_end_attached = true;
            }
        }
        // std::cout << player.body->GetPosition().x * PPM << " " << player.body->GetPosition().y * PPM << std::endl;
    }

    if (world->GetJointCount())
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            world->DestroyJoint(hook_joint);
            hook_end_attached = false;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            hook_joint->SetMaxLength(hook_joint->GetMaxLength() - 0.1f);
        }
    }
}

void GameState::update(float delta_time)
{
    // b2d
    world->Step(delta_time * 2, 6, 2);

    // player's aim position
    player_aim.setPosition(player.body->GetPosition().x * PPM, SCREEN_HEIGHT - player.body->GetPosition().y * PPM);

    // player's aim angle
    // int mouse_x = sf::Mouse::getPosition(*window).x;
    // int mouse_y = sf::Mouse::getPosition(*window).y;
    // int aim_x = player_aim.getPosition().x;
    // int aim_y = player_aim.getPosition().y;
    // aim_angle = atan2(mouse_y - aim_y, mouse_x - aim_x) * DEG_PER_RAD + 90;
    player_aim.setRotation(90 - aim_angle);
}

void GameState::draw(float delta_time)
{
    window->clear(sf::Color(56, 42, 55));

    // ----------------------------------------------------

    render(*window, boxes);

    // // p1
    // sf::RectangleShape rect2;
    // rect2.setPosition(p1.body->GetPosition().x * PPM, SCREEN_HEIGHT - (p1.body->GetPosition().y * PPM));
    // rect2.setOrigin(p1.width / 2, p1.height / 2);
    // rect2.setSize(sf::Vector2f(p1.width, p1.height));
    // rect2.setRotation(-1 * p1.body->GetAngle() * DEG_PER_RAD);
    // rect2.setFillColor(p1.color);
    // window->draw(rect2);

    // // p2
    // sf::RectangleShape rect3;
    // rect3.setPosition(p2.body->GetPosition().x * PPM, SCREEN_HEIGHT - (p2.body->GetPosition().y * PPM));
    // rect3.setOrigin(p2.width / 2, p2.height / 2);
    // rect3.setSize(sf::Vector2f(p2.width, p2.height));
    // rect3.setRotation(-1 * p2.body->GetAngle() * DEG_PER_RAD);
    // rect3.setFillColor(p2.color);
    // window->draw(rect3);

    // // draw boxes
    // for (int i = 0; i < boxes.size(); i++)
    // {
    //     Box *box = &boxes[i];
    //     box->shape->setPosition(box->body->GetPosition().x * PPM, SCREEN_HEIGHT - (box->body->GetPosition().y * PPM));
    //     box->shape->setOrigin(box->width / 2, box->height / 2);
    //     box->shape->setSize(sf::Vector2f(box->width, box->height));
    //     box->shape->setRotation(-1 * box->body->GetAngle() * DEG_PER_RAD);
    //     box->shape->setFillColor(box->color);
    //     window->draw(*box->shape);
    // }

    // draw hook
    if (hook_end_attached)
    {
        sf::RectangleShape hook;
        hook.setPosition(hook_end.body->GetPosition().x * PPM, SCREEN_HEIGHT - (hook_end.body->GetPosition().y * PPM));
        hook.setOrigin(hook_end.width / 2, hook_end.height / 2);
        hook.setSize(sf::Vector2f(hook_end.width, hook_end.height));
        hook.setRotation(-1 * hook_end.body->GetAngle() * DEG_PER_RAD);
        hook.setFillColor(hook_end.color);
        window->draw(hook);
    }

    // draw player
    sf::RectangleShape rect_player;
    rect_player.setPosition(player.body->GetPosition().x * PPM, SCREEN_HEIGHT - (player.body->GetPosition().y * PPM));
    rect_player.setOrigin(player.width / 2, player.height / 2);
    rect_player.setSize(sf::Vector2f(player.width, player.height));
    rect_player.setRotation(-1 * player.body->GetAngle() * DEG_PER_RAD);
    rect_player.setFillColor(player.color);
    window->draw(rect_player);

    // draw player's aim
    window->draw(player_aim);

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

    // // Create SFML shape
    // sf::RectangleShape *shape = new sf::RectangleShape(sf::Vector2f(width, height));

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

// ------------------- boxes -------------------

void GameState::init_boxes()
{
    Box b1 = createGround((SCREEN_WIDTH / 2) - 300, 0 + 100 / 2, 300, 100, sf::Color::White);
    boxes.push_back(b1);

    Box b2 = createGround((SCREEN_WIDTH / 2) + 300, 0 + 100 / 2, 300, 100, sf::Color::White);
    boxes.push_back(b2);

    Box b3 = createGround((SCREEN_WIDTH / 2), 350, 300, 80, sf::Color::White);
    boxes.push_back(b3);
}

// -------------- player movement --------------

void GameState::move_player_right()
{
    if (player.body->GetLinearVelocity().x <= 7)
    {
        player.body->ApplyLinearImpulseToCenter(b2Vec2(30, 0), true);
        // player.body->ApplyLinearImpulse(b2Vec2(0.5, 0), b2Vec2(player.body->GetPosition().x, player.body->GetPosition().y - player.height / 2 / PPM), true);
    }
}

void GameState::move_player_left()
{
    // std::cout << player.body->GetLinearVelocity().x << std::endl;
    if (player.body->GetLinearVelocity().x >= -7)
    {
        player.body->ApplyLinearImpulseToCenter(b2Vec2(-30, 0), true);
    }
}

void GameState::jump()
{
    player.body->ApplyLinearImpulseToCenter(b2Vec2(0, 50), true);
}

// raycast

float RayCastClosestCallback::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction)
{
    if (fixture->GetShape()->GetType() == b2Shape::e_chain)
    {
        return -1.0f;
    }

    if (fraction < m_closestFraction)
    {
        m_closestFraction = fraction;
        m_closestPoint = point;
        m_closestFixture = fixture;
    }

    return fraction;
}