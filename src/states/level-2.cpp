#include "states/level-2.hpp"

void Level2::init()
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

void Level2::handle_input()
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        player->action_glide();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
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

void Level2::update(float delta_time)
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

void Level2::draw(float delta_time)
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

void Level2::init_boxes()
{
    // paredes
    boxes.push_back(create_ground(world, 812, -900, 50, 1500, sf::Color::White, false));
    boxes.push_back(create_ground(world, 162, -940, 50, 1300, sf::Color::White, false));

    // chão
    boxes.push_back(create_ground(world, 162, 480, 700, 120, sf::Color::White, false));

    // objetos
    boxes.push_back(create_ground(world, 365, 350, 40, 130, sf::Color::White, false));
    boxes.push_back(create_ground(world, 574, 400, 238, 80, sf::Color::White, false));
    boxes.push_back(create_ground(world, 573, 230, 139, 55, sf::Color::White, false));
    boxes.push_back(create_ground(world, 475, 106, 40, 203, sf::Color::White, false));
    boxes.push_back(create_ground(world, 309, -191, 93, 41, sf::Color::White, false));
    boxes.push_back(create_ground(world, 531, -445, 42, 175, sf::Color::Black, false));

    // hooks
    hook_boxes.push_back(create_ground(world, 365, 310, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 752, 265, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 602, 122, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 602, -76, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 428, -76, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 235, -127, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 464, -268, 40, 40, sf::Color::Green, true));
}