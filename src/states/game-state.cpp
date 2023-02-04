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
    player = new Player(world, 150, 130, 34, 44, 45.f, 0.7f, sf::Color::Magenta);

    // all boxes in this level
    init_boxes();

    // configure camera
    view.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    view.setCenter(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
    default_view = view;
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
        player->use_hook(*window, hook_boxes);
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

    // set camera position relative to the player
    int camera_x = SCREEN_WIDTH / 2;
    int camera_y = SCREEN_HEIGHT - player->body->GetPosition().y * PPM;

    if (camera_y > SCREEN_HEIGHT / 2)
    {
        std::cout << camera_y << std::endl;
        camera_y = SCREEN_HEIGHT / 2;
    }

    view.setCenter(camera_x, camera_y);

}

void GameState::draw(float delta_time)
{
    window->clear(sf::Color(56, 42, 55));

    // set camera
    window->setView(view);

    render_box_vector(*window, boxes);
    render_box_vector(*window, hook_boxes);

    player->render_player(*window);
    player->render_player_aim(*window);
    player->render_hook(*window);

    // set default view
    window->setView(default_view);
    window->draw(this->pause_button);
    window->display();
}

// ------------------- boxes -------------------

void GameState::init_boxes()
{
    boxes.push_back(create_ground(world, (SCREEN_WIDTH / 2) - 300, 0 + 100 / 2, 300, 100, sf::Color::White, false));
    boxes.push_back(create_ground(world, (SCREEN_WIDTH / 2) + 300, 0 + 100 / 2, 300, 100, sf::Color::White, false));
    boxes.push_back(create_ground(world, (SCREEN_WIDTH / 2), 350, 300, 80, sf::Color::White, false));

    hook_boxes.push_back(create_ground(world, (SCREEN_WIDTH / 2), 321, 280, 30, sf::Color::Red, true));
    hook_boxes.push_back(create_ground(world, (SCREEN_WIDTH / 2) + 300, 0 + 600 / 2, 40, 40, sf::Color::Red, true));
}