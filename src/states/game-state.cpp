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
    player = new Player(world, 400, 1600, 23, 50, 45.f, 0.7f, sf::Color::Magenta);
    // player = new Player(world, 200, -624, 34, 44, 45.f, 0.7f, sf::Color::Magenta);
    // 200 180

    // all boxes in this level
    init_boxes();

    // configure camera
    view.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    view.setCenter(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
    default_view = view;

    // background
    assets->load_texture("background", "resources/background/Ato_1.png");
    background.setTexture(assets->get_texture("background"));
    background.setScale(1.1f, 1.1f);
    background.setPosition((SCREEN_WIDTH - background.getGlobalBounds().width) / 2, (SCREEN_HEIGHT - background.getGlobalBounds().height) / 2);

    assets->load_texture("tree1", "resources/background/Background_1.png");
    tree1.setTexture(assets->get_texture("tree1"));
    tree1.setPosition((SCREEN_WIDTH - tree1.getGlobalBounds().width) / 2, (SCREEN_HEIGHT - tree1.getGlobalBounds().height) / 2);

    assets->load_texture("tree2", "resources/background/Background_2.png");
    tree2.setTexture(assets->get_texture("tree2"));
    tree2.setPosition((SCREEN_WIDTH - tree2.getGlobalBounds().width) / 2, (SCREEN_HEIGHT - 3 * tree2.getGlobalBounds().height) / 2);
    // window->setSize(sf::Vector2u(SCREEN_WIDTH * 1.5, SCREEN_HEIGHT * 1.5));

    // load texture
    if (!box_texture.loadFromFile("resources/block_texture.png"))
    {
        std::cerr << "error loading texture" << std::endl;
    }
    if (!roots_texture.loadFromFile("resources/small_roots_texture.png"))
    {
        std::cerr << "error loading texture" << std::endl;
    }
    if (!end_texture.loadFromFile("resources/end_level.png"))
    {
        std::cerr << "error loading texture" << std::endl;
    }

    // goal position
    end_sprite.setTexture(end_texture);
    end_sprite.setPosition(sf::Vector2f(650, -1050));
    end_sprite.setScale(sf::Vector2f(0.15, 0.15));
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

    // player movement (jump have priority)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        player->action_jump();
    }
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
    player->update_player_animation(delta_time);

    // set camera position relative to the player
    int camera_x = SCREEN_WIDTH / 2;
    int camera_y = SCREEN_HEIGHT - player->body->GetPosition().y * PPM;

    if (camera_y > SCREEN_HEIGHT / 2)
    {
        camera_y = SCREEN_HEIGHT / 2;
    }

    // verify goal
    if (player->animations[player->action]->get_sprite()->getGlobalBounds().intersects(end_sprite.getGlobalBounds()))
    {
        add_state<Level2>(true);
    }

    view.setCenter(camera_x, camera_y);
}

void GameState::draw(float delta_time)
{
    window->clear(sf::Color(84, 69, 65));
    // window->clear(sf::Color::White);

    // set camera
    window->setView(view);

    window->draw(this->background);
    window->draw(this->tree1);
    window->draw(this->tree2);

    render_box_vector(*window, boxes, box_texture);
    render_box_vector(*window, hook_boxes, roots_texture);

    // player->render_hook(*window);
    player->render_player(*window);
    // player->render_player_aim(*window);

    window->draw(end_sprite);

    // set default view
    window->setView(default_view);
    window->draw(this->pause_button);
    window->display();
}

// ------------------- boxes -------------------

void GameState::init_boxes()
{
    // teto
    boxes.push_back(create_ground(world, 162, 480, 700, 120, sf::Color::White, false));

    // objetos
    boxes.push_back(create_ground(world, 290, 430, 100, 50, sf::Color::White, false));
    boxes.push_back(create_ground(world, 610, 410, 210, 70, sf::Color::White, false));
    boxes.push_back(create_ground(world, 420, 205, 192, 59, sf::Color::White, false)); // do meio no inicio
    boxes.push_back(create_ground(world, 212, 107, 63, 60, sf::Color::White, false));
    boxes.push_back(create_ground(world, 287, -150, 93, 41, sf::Color::White, false));

    boxes.push_back(create_ground(world, 430, -100, 55, 150, sf::Color::White, false));

    boxes.push_back(create_ground(world, 536, 48, 170, 55, sf::Color::White, false));
    boxes.push_back(create_ground(world, 536, -7, 60, 55, sf::Color::White, false));

    boxes.push_back(create_ground(world, 500, -353, 106, 42, sf::Color::White, false));
    boxes.push_back(create_ground(world, 564, -353, 42, 150, sf::Color::White, false));

    boxes.push_back(create_ground(world, 372, -556, 37, 75, sf::Color::White, false));
    boxes.push_back(create_ground(world, 372, -556, 99, 37, sf::Color::White, false));

    boxes.push_back(create_ground(world, 655, -470, 170, 37, sf::Color::White, false));

    boxes.push_back(create_ground(world, 500, -515, 57, 37, sf::Color::White, false));

    boxes.push_back(create_ground(world, 212, -319, 56, 59, sf::Color::White, false));

    boxes.push_back(create_ground(world, 655, -814, 42, 260, sf::Color::Black, false));

    boxes.push_back(create_ground(world, 342, -940, 500, 42, sf::Color::Black, false));

    // hooks
    hook_boxes.push_back(create_ground(world, 426, 254, 180, 15, sf::Color::White, true)); // do meio no inicio

    hook_boxes.push_back(create_ground(world, 690, 230, 40, 40, sf::Color::Yellow, true)); // pŕimeira

    hook_boxes.push_back(create_ground(world, 350, 70, 40, 40, sf::Color::Yellow, true)); // segunda

    hook_boxes.push_back(create_ground(world, 223, -49, 40, 40, sf::Color::Yellow, true)); // vertical esquerda

    hook_boxes.push_back(create_ground(world, 223, -217, 40, 40, sf::Color::Yellow, true)); // vertical esquerda

    hook_boxes.push_back(create_ground(world, 700, -120, 40, 40, sf::Color::Yellow, true));

    hook_boxes.push_back(create_ground(world, 700, -315, 40, 40, sf::Color::Yellow, true));

    // hook_boxes.push_back(create_ground(world, 647, -420, 40, 40, sf::Color::Green, true));

    hook_boxes.push_back(create_ground(world, 430, -457, 40, 40, sf::Color::Yellow, true));

    hook_boxes.push_back(create_ground(world, 223, -440, 40, 40, sf::Color::Yellow, true));

    // hook_boxes.push_back(create_ground(world, 728, -550, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 732, -653, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 732, -814, 40, 40, sf::Color::Yellow, true));

    // hook_boxes.push_back(create_ground(world, 527, -854, 40, 40, sf::Color::Magenta, true));
    // hook_boxes.push_back(create_ground(world, 382, -854, 40, 40, sf::Color::Magenta, true));

    hook_boxes.push_back(create_ground(world, 382, -907, 230, 15, sf::Color::Magenta, true));

    hook_boxes.push_back(create_ground(world, 257, -879, 40, 40, sf::Color::Magenta, true));

    // paredes
    boxes.push_back(create_ground(world, 812, -1400, 50, 2000, sf::Color::White, false));
    boxes.push_back(create_ground(world, 162, -1440, 50, 1800, sf::Color::White, false));
}