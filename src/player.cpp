#include "player.hpp"

// -------------------- raycast --------------------

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
        m_closestBody = fixture->GetBody();
    }

    return fraction;
}

// -------------------- player ---------------------

Player::Player(b2World *world, int x, int y, float width, float height, float density, float friction, sf::Color color)
{
    // Body definition
    body_def.position.Set(x / PPM, y / PPM);
    body_def.type = b2_dynamicBody;

    // Shape definition
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width / 2 / PPM, height / 2 / PPM);

    // Fixture definition
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.shape = &boxShape;

    // Now we have a body for our Box object
    body = world->CreateBody(&body_def);
    // Lastly, assign the fixture
    body->CreateFixture(&fixtureDef);

    // rotate is a crime!
    body->SetAngularVelocity(0.0f);
    body->SetFixedRotation(true);

    original_gravity_scale = body->GetGravityScale();
    this->width = width;
    this->height = height;
    this->color = color;
    this->world = world;

    // define bools
    hook_end_attached = false;
    player_on_ground = false;
    in_action_glide = false;
    in_action_jump = false;
    is_hook_impulse_applied = false;

    // player's aim indicator
    // player_aim.setSize(sf::Vector2f(5, 30));
    player_aim.setOrigin(5, 25);
    // player_aim.setFillColor(sf::Color::Red);
    player_aim.setPosition(sf::Vector2f(x, SCREEN_HEIGHT - y));
    aim_angle = 0;

    // player animation (insert the a)
    action = idle;
    previous_action = idle;

    // idle animation (position 0)
    animations.push_back(new pte::Animation("resources/yuca/yuca_idle.png", 500 / 7, 650 / 7, 7, 30, 30, 0.2f));

    // running left animation (position 1)
    animations.push_back(new pte::Animation("resources/yuca/yuca_right.png", 500 / 7, 650 / 7, 12, 30, 30, 0.1f));

    // running right animation (position 2)
    animations.push_back(new pte::Animation("resources/yuca/yuca_left.png", 500 / 7, 650 / 7, 12, 30, 30, 0.05f));

    // jump animation (position 3)
    animations.push_back(new pte::Animation("resources/yuca/yuca_jump.png", 500 / 7, 650 /7, 5, 0, 0, 0.2f));

    // hook animation (position 4)
    animations.push_back(new pte::Animation("resources/yuca/yuca_hook.png", 500 / 7, 650 / 7, 6, 0, 0, 0.25f));

    // glide animation (position 5)
    animations.push_back(new pte::Animation("resources/yuca/yuca_glide.png", 500 / 7, 650 / 7, 8, 0, 0, 0.2f));

    // falling animation (position 6)
    animations.push_back(new pte::Animation("resources/yuca/yuca_falling.png", 500 / 7, 650 / 7, 11, 0, 0, 0.1f));

    // load hook sprite
    if (!hook_texture.loadFromFile("resources/hook.png"))
    {
        std::cerr << "Failed to load hook sprite" << std::endl;
    }

    // load player aim
    if (!player_aim_texture.loadFromFile("resources/yuca/yuca_aim.png"))
    {
        std::cerr << "Failed to load player aim sprite" << std::endl;
    }
}

void Player::render_player(sf::RenderWindow &window)
{

    sf::Sprite *frame_sprite = animations[action]->get_sprite();

    // // For the correct Y coordinate of our drawable sprite, we must substract from WINDOW_HEIGHT
    // // because SFML uses OpenGL coordinate system where X is right, Y is down
    // // while Box2D uses traditional X is right, Y is up
    frame_sprite->setPosition(body->GetPosition().x * PPM, SCREEN_HEIGHT - (body->GetPosition().y * PPM));

    // // We also need to set our drawable's origin to its center
    // // because in SFML, "position" refers to the upper left corner
    // // while in Box2D, "position" refers to the body's center
    // frame_sprite->setOrigin(width / 2, height / 2);

    // sf::RectangleShape shape;
    // shape.setPosition(body->GetPosition().x * PPM, SCREEN_HEIGHT - (body->GetPosition().y * PPM));
    // shape.setOrigin(width / 2, height / 2);
    // shape.setSize(sf::Vector2f(width, height));
    // shape.setRotation(-1 * body->GetAngle() * DEG_PER_RAD);
    // shape.setFillColor(color);
    // window.draw(shape);

    // window->draw(frame_sprite);
    window.draw(*frame_sprite);

    if (hook_end_attached)
    {
        render_hook(window);
        render_player_aim(window);
    }
}

void Player::render_player_aim(sf::RenderWindow &window)
{
    player_aim.setTexture(player_aim_texture);
    sf::Vector2f end_size(player_aim_texture.getSize().x / 2, player_aim_texture.getSize().y / 2);
    sf::Vector2f scale(end_size.x / player_aim_texture.getSize().x, end_size.y / player_aim_texture.getSize().y);
    // player_aim.scale(sf::Vector2f(scale.x, scale.y));
    // player_aim.setScale(0.5, 0,5);
    player_aim.setScale(sf::Vector2f(0.15, 0.15));

    window.draw(player_aim);
}

void Player::move_player_left()
{
    if (body->GetLinearVelocity().x >= -PLAYER_MAX_LINEAR_VELOCITY)
    {
        body->ApplyLinearImpulseToCenter(b2Vec2(-30, 0), true);
    }
}

void Player::move_player_right()
{
    if (body->GetLinearVelocity().x <= PLAYER_MAX_LINEAR_VELOCITY)
    {
        body->ApplyLinearImpulseToCenter(b2Vec2(30, 0), true);
    }
}

void Player::action_jump()
{
    if (hook_end_attached && !is_hook_impulse_applied)
    {
        // std::cout << aim_angle << std::endl;
        b2Vec2 direction(cos(aim_angle_rad), sin(aim_angle_rad));
        direction *= 600;
        body->ApplyLinearImpulseToCenter(direction, true);
        is_hook_impulse_applied = true;
        destroy_hook();
    }
    else if (player_on_ground)
    {
        player_on_ground = false;
        body->ApplyLinearImpulseToCenter(b2Vec2(0, 200), true); // AQUI É ONDE MUDA MESMO O PULO
        // in_action_jump = true;

        color = sf::Color::Cyan;
    }
}

void Player::action_glide()
{
    if (body->GetLinearVelocity().y < 3 && !in_action_glide && !hook_end_attached && !player_on_ground)
    {
        in_action_glide = true;
        body->SetGravityScale(0.2f);
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, 0));
        body->ApplyLinearImpulseToCenter(b2Vec2(0, 80), true);
        color = sf::Color::Yellow;
    }
}

void Player::update_player_state(sf::RenderWindow &window, sf::View &view)
{
    // movement state
    if (!player_on_ground)
    {
        for (b2ContactEdge *edge = body->GetContactList(); edge; edge = edge->next)
        {
            if (edge->contact->IsTouching())
            {
                b2WorldManifold worldManifold;
                edge->contact->GetWorldManifold(&worldManifold);

                if (worldManifold.normal.y < 0)
                {
                    // The second body is below the first body
                    player_on_ground = true;
                    color = sf::Color::Magenta;

                    if (in_action_glide)
                    {
                        in_action_glide = false;
                        body->SetGravityScale(original_gravity_scale);
                    }

                    break;
                }
                else
                {
                    // The second body is not below the first body
                    player_on_ground = false;
                }
            }
            else
            {
                // The dynamic body is not in contact with another body
                player_on_ground = false;
            }
        }
    }

    // -------------------- angle --------------------
    // player's aim angle and position
    player_aim.setPosition(sf::Vector2f((body->GetPosition().x * PPM) - 10, (SCREEN_HEIGHT - body->GetPosition().y * PPM) + 5));

    // get mouse positions relative to the view
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    sf::Vector2f world_pos = window.mapPixelToCoords(mouse_pos, view);
    int end_x = world_pos.x;
    int end_y = SCREEN_HEIGHT - world_pos.y;

    // use the hook end instead if attached
    if (hook_end_attached)
    {
        end_x = hook_end.body->GetPosition().x * PPM;
        end_y = hook_end.body->GetPosition().y * PPM;
    }

    b2Vec2 player_pos = body->GetPosition();
    player_pos *= PPM;

    float delta_x = end_x - player_pos.x;
    float delta_y = end_y - player_pos.y;
    aim_angle_rad = atan2(delta_y, delta_x);
    aim_angle = aim_angle_rad * 180.0f / b2_pi;

    // set player aim rotation
    player_aim.setRotation(-aim_angle + 90 + 180);
}

// --------------------- hook ----------------------

void Player::use_hook(sf::RenderWindow &window, std::vector<Box> &box_vec)
{
    if (!hook_end_attached)
    {
        // angle is updated every frame with update_player_state() method

        if (in_action_glide)
        {
            in_action_glide = false;
            body->SetGravityScale(original_gravity_scale);
            color = sf::Color::Cyan;
        }
        int hook_range = 5;

        // get closest point using raycast and the pointing vector
        b2Vec2 direction(cos(aim_angle_rad), sin(aim_angle_rad));
        direction *= hook_range;
        RayCastClosestCallback callback;
        world->RayCast(&callback, body->GetPosition(), body->GetPosition() + direction);

        // if something is captured in the beam
        if (callback.m_closestFixture)
        {
            for (auto b : box_vec)
            {
                if (b.body == callback.m_closestBody)
                {
                    b2Vec2 closestPoint = callback.m_closestPoint;

                    // -------------------- create new hook body --------------------

                    float h_width = 10, h_height = 10;

                    // Static body definition
                    b2BodyDef groundBodyDef;
                    groundBodyDef.position.Set(closestPoint.x, closestPoint.y);

                    // Shape definition
                    b2PolygonShape groundBox;
                    groundBox.SetAsBox(h_width / 2 / PPM, h_height / 2 / PPM);

                    // Now we have a body for our Box object
                    b2Body *groundBody = world->CreateBody(&groundBodyDef);
                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &groundBox;
                    fixtureDef.density = 0.0f;
                    fixtureDef.isSensor = true;

                    groundBody->CreateFixture(&fixtureDef);

                    hook_end = Box{h_width, h_height, sf::Color::Green, groundBody};

                    // --------------------------------------------------------------

                    // joint
                    hook_joint_def.Initialize(body, hook_end.body, body->GetWorldCenter(), hook_end.body->GetWorldCenter());
                    hook_joint_def.collideConnected = true;
                    hook_joint_def.stiffness = 0.f;
                    hook_joint_def.damping = 0.f;

                    float value = pow(closestPoint.x - body->GetWorldCenter().x, 2) + pow(closestPoint.y - body->GetWorldCenter().y, 2);
                    float distance = sqrt(value);

                    hook_joint = (b2DistanceJoint *)world->CreateJoint(&hook_joint_def);
                    hook_joint->SetMinLength(1.f);
                    hook_joint->SetLength(distance);
                    hook_joint->SetMaxLength(distance);
                    if (distance < 3)
                    {
                        hook_joint->SetMaxLength(3.f);
                    }

                    body->ApplyForceToCenter(b2Vec2(0, -1), true);
                    hook_end_attached = true;
                }
            }
        }
    }
}

void Player::render_hook(sf::RenderWindow &window)
{
    if (hook_end_attached)
    {
        // draw hook sprite !!
        sf::Sprite sprite;
        sprite.setTexture(hook_texture);

        sf::Vector2f point_a = sf::Vector2f(body->GetPosition().x * PPM, SCREEN_HEIGHT - body->GetPosition().y * PPM);
        sf::Vector2f point_b = sf::Vector2f(hook_end.body->GetPosition().x * PPM, SCREEN_HEIGHT - hook_end.body->GetPosition().y * PPM);

        point_a.x -= 13;
        point_a.y += 10;

        // Set the position of the sprite based on the two points
        sf::Vector2f topPoint(point_b.x, point_b.y);
        sf::Vector2f bottomPoint(point_a.x, point_a.y);
        // sf::Vector2f centerPoint = (topPoint + bottomPoint) / 2.f;
        sprite.setPosition(sf::Vector2f(topPoint.x, topPoint.y));
        sprite.setOrigin(30, 30);

        // Set the rotation of the sprite
        float angle = std::atan2(bottomPoint.y - topPoint.y, bottomPoint.x - topPoint.x) * 180.f / 3.14159265f;
        sprite.setRotation(angle);

        // Set the scale of the sprite
        float width = hook_texture.getSize().x * 0.3;
        float height = hook_texture.getSize().y * 0.3;
        sprite.setScale(width / hook_texture.getSize().x, height / hook_texture.getSize().y);

        float distance = std::sqrt((topPoint.x - bottomPoint.x) * (topPoint.x - bottomPoint.x) +
                                   (topPoint.y - bottomPoint.y) * (topPoint.y - bottomPoint.y));

        sprite.setTextureRect(sf::IntRect(0, 0, distance * 3.33, 50));

        // Draw the sprite
        window.draw(sprite);
    }
}

void Player::decrease_hook_length()
{
    hook_joint->SetMaxLength(hook_joint->GetMaxLength() - 0.1f);
}

void Player::destroy_hook()
{
    world->DestroyJoint(hook_joint);
    world->DestroyBody(hook_end.body);
    hook_end_attached = false;
    is_hook_impulse_applied = false;
}

// --------------------- player animation ----------------------

void Player::update_player_animation(float delta_time)
{

    // on hook
    if (hook_end_attached)
    {
        previous_action = action;
        action = hook;
    }
    // player glide
    else if (in_action_glide)
    {
        previous_action = action;
        action = glide;
    }
    // player idle
    else if (player_on_ground && body->GetLinearVelocity().x == 0)
    {
        previous_action = action;
        action = idle;
    }
    // player running right
    else if (player_on_ground && body->GetLinearVelocity().x < 0)
    {
        previous_action = action;
        action = running_right;
    }
    // player running left
    else if (player_on_ground && body->GetLinearVelocity().x > 0)
    {
        previous_action = action;
        action = running_left;
    }
    // player jump
    else if (!player_on_ground && !in_action_glide && !hook_end_attached)
    {
        previous_action = action;
        if(body->GetLinearVelocity().y > 1)
        {
            action = jump;
        }
        else
        {
            action = falling;
        }
    }

    if (action != previous_action)
    {
        animations[previous_action]->reset();
    }

    animations[action]->update_animation(delta_time);
}