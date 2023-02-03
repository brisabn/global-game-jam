#include "states/game-state.hpp"

void GameState::init()
{
    window->setFramerateLimit(60);

    assets->load_texture("pause_button", PAUSE_BUTTON);
    pause_button.setTexture(assets->get_texture("pause_button"));
    pause_button.setPosition(window->getSize().x - pause_button.getLocalBounds().width - 10, pause_button.getPosition().y + 10);

    // b2d world, gravity: 9 m/s^2
    world = new b2World(b2Vec2(0, -9));

    // create player
    player = new Player(world, 200, 180, 34, 44, 45.f, 0.7f, sf::Color::Magenta);

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
    }

    // player movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        player->move_player_left();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        player->move_player_right();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        player->action_jump_glide();
    }

    // grappling hook
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        player->use_hook(*window);
    }

    // if exists some joint, it's definitely the hook joint! (gambiarra)
    if (world->GetJointCount())
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            player->destroy_hook();
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            player->decrease_hook_length();
        }
    }
}

void GameState::update(float delta_time)
{
    // b2d
    world->Step(delta_time * 2, 6, 2);

    // update player (used in movement states)
    player->update_player_state(*window);
}

void GameState::draw(float delta_time)
{
    window->clear(sf::Color(56, 42, 55));

    render_box_vector(*window, boxes);

    player->render_player(*window);
    player->render_player_aim(*window);
    player->render_hook(*window);

    window->draw(this->pause_button);
    window->display();
}

// ------------------- boxes -------------------

void GameState::init_boxes()
{
    boxes.push_back(create_ground(world, 162, 480, 700, 120, sf::Color::White));
    boxes.push_back(create_ground(world, 812, -900, 50, 1500, sf::Color::White));
    boxes.push_back(create_ground(world, 162, -940, 50, 1300, sf::Color::White));

    // objetos
    boxes.push_back(create_ground(world, 350, 430, 100, 50, sf::Color::White));
    boxes.push_back(create_ground(world, 583, 410, 150, 70, sf::Color::White));
}