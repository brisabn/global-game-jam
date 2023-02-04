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
    // player = new Player(world, 200, -624, 34, 44, 45.f, 0.7f, sf::Color::Magenta);

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
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        player->action_glide();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        player->action_jump();
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
    player->update_player_state(*window, view);

    // set camera position relative to the player
    int camera_x = SCREEN_WIDTH / 2;
    int camera_y = SCREEN_HEIGHT - player->body->GetPosition().y * PPM;

    if (camera_y > SCREEN_HEIGHT / 2)
    {
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
    // paredes
    boxes.push_back(create_ground(world, 812, -900, 50, 1500, sf::Color::White, false));
    boxes.push_back(create_ground(world, 162, -940, 50, 1300, sf::Color::White, false));

    // teto
    boxes.push_back(create_ground(world, 162, 480, 700, 120, sf::Color::White, false));

    // objetos
    boxes.push_back(create_ground(world, 264, 430, 100, 50, sf::Color::White, false));
    boxes.push_back(create_ground(world, 583, 410, 230, 70, sf::Color::White, false));
    boxes.push_back(create_ground(world, 420, 235, 192, 59, sf::Color::White, false));
    boxes.push_back(create_ground(world, 212, 117, 63, 60, sf::Color::White, false));
    boxes.push_back(create_ground(world, 287, -168, 93, 41, sf::Color::White, false));

    boxes.push_back(create_ground(world, 536, 48, 170, 55, sf::Color::White, false));
    boxes.push_back(create_ground(world, 536, -7, 60, 55, sf::Color::White, false));

    boxes.push_back(create_ground(world, 500, -353, 106, 42, sf::Color::White, false));
    boxes.push_back(create_ground(world, 564, -353, 42, 175, sf::Color::White, false));

    boxes.push_back(create_ground(world, 372, -556, 37, 75, sf::Color::White, false));
    boxes.push_back(create_ground(world, 372, -556, 99, 37, sf::Color::White, false));

    boxes.push_back(create_ground(world, 212, -319, 56, 59, sf::Color::White, false));

    boxes.push_back(create_ground(world, 655, -814, 42, 200, sf::Color::Black, false));

    boxes.push_back(create_ground(world, 342, -940, 313, 42, sf::Color::Black, false));

    // hooks
    hook_boxes.push_back(create_ground(world, 720, 275, 40, 40, sf::Color::Yellow, true));

    hook_boxes.push_back(create_ground(world, 330, 137, 40, 40, sf::Color::Yellow, true));

    hook_boxes.push_back(create_ground(world, 223, -49, 40, 40, sf::Color::Yellow, true));

    hook_boxes.push_back(create_ground(world, 223, -217, 40, 40, sf::Color::Yellow, true));

    hook_boxes.push_back(create_ground(world, 665, -95, 40, 40, sf::Color::Yellow, true));

    hook_boxes.push_back(create_ground(world, 665, -315, 40, 40, sf::Color::Yellow, true));

    // hook_boxes.push_back(create_ground(world, 647, -420, 40, 40, sf::Color::Green, true));

    hook_boxes.push_back(create_ground(world, 430, -457, 40, 40, sf::Color::Yellow, true));

    hook_boxes.push_back(create_ground(world, 223, -440, 40, 40, sf::Color::Yellow, true));

    hook_boxes.push_back(create_ground(world, 728, -550, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 728, -676, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 728, -814, 40, 40, sf::Color::Yellow, true));

    hook_boxes.push_back(create_ground(world, 527, -854, 40, 40, sf::Color::Magenta, true));
    hook_boxes.push_back(create_ground(world, 382, -854, 40, 40, sf::Color::Magenta, true));

    hook_boxes.push_back(create_ground(world, 257, -879, 40, 40, sf::Color::Magenta, true));
}