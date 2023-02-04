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
    player_aim.setSize(sf::Vector2f(10, 30));
    player_aim.setOrigin(5, 25);
    player_aim.setFillColor(sf::Color::Red);
    player_aim.setPosition(sf::Vector2f(x, SCREEN_HEIGHT - y));
    aim_angle = 0;
}

void Player::render_player(sf::RenderWindow &window)
{
    shape.setPosition(body->GetPosition().x * PPM, SCREEN_HEIGHT - (body->GetPosition().y * PPM));
    shape.setOrigin(width / 2, height / 2);
    shape.setSize(sf::Vector2f(width, height));
    shape.setRotation(-1 * body->GetAngle() * DEG_PER_RAD);
    shape.setFillColor(color);
    window.draw(shape);
}

void Player::render_player_aim(sf::RenderWindow &window)
{
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

void Player::action_jump_glide()
{

    if (hook_end_attached && !is_hook_impulse_applied)
    {
        std::cout << aim_angle << std::endl;
        b2Vec2 direction(cos(aim_angle_rad), sin(aim_angle_rad));
        direction *= 1000;
        body->ApplyLinearImpulseToCenter(direction, true);
        is_hook_impulse_applied = true;
        destroy_hook();
    }
    else if (player_on_ground)
    {
        player_on_ground = false;
        body->ApplyLinearImpulseToCenter(b2Vec2(0, 250), true);
        // in in_action_jump = true;

        color = sf::Color::Cyan;
    }
    else
    {
        if (body->GetLinearVelocity().y < 3 && !in_action_glide && !hook_end_attached)
        {
            in_action_glide = true;
            body->SetGravityScale(0.2f);
            body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, 0));
            body->ApplyLinearImpulseToCenter(b2Vec2(0, 80), true);
            color = sf::Color::Yellow;
        }
    }
}

void Player::update_player_state(sf::RenderWindow &window)
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

    // player's aim angle and position
    player_aim.setPosition(sf::Vector2f(body->GetPosition().x * PPM, SCREEN_HEIGHT - body->GetPosition().y * PPM));

    int end_x = sf::Mouse::getPosition(window).x;
    int end_y = SCREEN_HEIGHT - sf::Mouse::getPosition(window).y;

    if (hook_end_attached)
    {
        end_x = hook_end.body->GetPosition().x * PPM;
        end_y = hook_end.body->GetPosition().y * PPM;
    }

    b2Vec2 player_pos = body->GetPosition();
    player_pos *= PPM;
    aim_angle_rad = atan2(end_y - player_pos.y, end_x - player_pos.x);
    aim_angle = aim_angle_rad * 180.0f / b2_pi;

    player_aim.setRotation(-aim_angle + 90);
}

// --------------------- hook ----------------------

void Player::use_hook(sf::RenderWindow &window, std::vector<Box> &box_vec)
{
    if (!hook_end_attached && !in_action_glide)
    {
        b2Vec2 playerPos = body->GetPosition();
        playerPos *= PPM;
        int mouseX = sf::Mouse::getPosition(window).x;
        int mouseY = SCREEN_HEIGHT - sf::Mouse::getPosition(window).y;
        aim_angle_rad = atan2(mouseY - playerPos.y, mouseX - playerPos.x);
        aim_angle = aim_angle_rad * 180.0f / b2_pi;

        b2Vec2 direction(cos(aim_angle_rad), sin(aim_angle_rad));
        direction *= 10;
        RayCastClosestCallback callback;
        playerPos.x /= PPM;
        playerPos.y /= PPM;
        world->RayCast(&callback, playerPos, playerPos + direction);

        // if something is captured in the beam
        if (callback.m_closestFixture)
        {

            for (auto b : box_vec)
            {
                if (b.body == callback.m_closestBody)
                {
                    b2Vec2 closestPoint = callback.m_closestPoint;

                    hook_end = create_ground(world, closestPoint.x * PPM, closestPoint.y * PPM, 10, 10, sf::Color::Green, true);

                    // joint
                    hook_joint_def.Initialize(body, hook_end.body, body->GetWorldCenter(), hook_end.body->GetWorldCenter());
                    hook_joint_def.collideConnected = true;
                    hook_joint_def.stiffness = 0.f;
                    hook_joint_def.damping = 0.f;

                    float value = pow(closestPoint.x - body->GetWorldCenter().x, 2) + pow(closestPoint.y - body->GetWorldCenter().y, 2);
                    float distance = sqrt(value);

                    hook_joint = (b2DistanceJoint *)world->CreateJoint(&hook_joint_def);
                    hook_joint->SetMinLength(0.f);
                    hook_joint->SetLength(distance);
                    hook_joint->SetMaxLength(distance);

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
        // render hook box
        sf::RectangleShape hook;
        hook.setPosition(hook_end.body->GetPosition().x * PPM, SCREEN_HEIGHT - (hook_end.body->GetPosition().y * PPM));
        hook.setOrigin(hook_end.width / 2, hook_end.height / 2);
        hook.setSize(sf::Vector2f(hook_end.width, hook_end.height));
        hook.setRotation(-1 * hook_end.body->GetAngle() * DEG_PER_RAD);
        hook.setFillColor(hook_end.color);
        window.draw(hook);

        // render line
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(body->GetPosition().x * PPM, SCREEN_HEIGHT - body->GetPosition().y * PPM);
        line[1].position = sf::Vector2f(hook_end.body->GetPosition().x * PPM, SCREEN_HEIGHT - hook_end.body->GetPosition().y * PPM);
        line[0].color = sf::Color::Red;
        line[1].color = sf::Color::Red;
        window.draw(line);
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
