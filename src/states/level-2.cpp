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
    player = new Player(world, 410, 1980, 23, 50, 45.f, 0.7f, sf::Color::Magenta);
    // player = new Player(world, 200, -624, 34, 44, 45.f, 0.7f, sf::Color::Magenta);

    // all boxes in this level
    init_boxes();

    // configure camera
    view.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    view.setCenter(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
    default_view = view;

    // background
    assets->load_texture("background", "resources/background/Ato_2.png");
    background.setTexture(assets->get_texture("background"));
    background.setScale(1.1f, 1.1f);
    background.setPosition((SCREEN_WIDTH - background.getGlobalBounds().width) / 2, (SCREEN_HEIGHT - background.getGlobalBounds().height) / 2);

    assets->load_texture("tree1", "resources/background/Background_1.png");
    tree1.setTexture(assets->get_texture("tree1"));
    tree1.setPosition((SCREEN_WIDTH - tree1.getGlobalBounds().width) / 2, (SCREEN_HEIGHT - tree1.getGlobalBounds().height) / 2);

    assets->load_texture("tree2", "resources/background/Background_2.png");
    tree2.setTexture(assets->get_texture("tree2"));
    tree2.setPosition((SCREEN_WIDTH - tree2.getGlobalBounds().width) / 2, (SCREEN_HEIGHT - 3 * tree2.getGlobalBounds().height) / 2);

    // load texture
    if (!box_texture.loadFromFile("resources/block_texture.png"))
    {
        std::cerr << "error loading texture" << std::endl;
    }
    if (!roots_texture.loadFromFile("resources/small_roots_texture.png"))
    {
        std::cerr << "error loading texture" << std::endl;
    }
    if (!end_texture.loadFromFile("resources/door.png"))
    {
        std::cerr << "error loading texture" << std::endl;
    }
    if (!door_texture.loadFromFile("resources/door.png"))
    {
        std::cerr << "error loading texture" << std::endl;
    }

    // goal position
    end_sprite.setTexture(end_texture);
    end_sprite.setPosition(sf::Vector2f(650, -1295));
    end_sprite.setScale(sf::Vector2f(0.3, 0.5));

    // door
    door_sprite.setTexture(door_texture);
    door_sprite.setPosition(sf::Vector2f(180, 383));
    door_sprite.setScale(sf::Vector2f(0.3, 0.5));

    // audio
    music = new Audio();
    music->define_sound("resources/music/soundtrack/ato_3_novo.ogg", 100);
    music->play_sound();
    music_on = true;
}

void Level2::handle_input()
{
    // check for music and pause
    if (music_on == false)
    {
        std::cout << "false" << std::endl;
        music->music.play();
        music_on = true;
    }

    sf::Event event;

    while (window->pollEvent(event))
    {
        if (sf::Event::Closed == event.type)
        {
            window->close();
        }

        if (input->is_sprite_clicked(this->pause_button, sf::Mouse::Left, *window))
        {
            // pause game
            music->music.pause();
            
            music_on = false;
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

void Level2::update(float delta_time)
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
        delete music;
        add_state<MainCreditsState>(true);
    }

    view.setCenter(camera_x, camera_y);
}

void Level2::draw(float delta_time)
{
    window->clear(sf::Color(56, 42, 55));

    // set camera
    window->setView(view);
    window->draw(this->background);
    window->draw(this->tree1);
    window->draw(this->tree2);

    render_box_vector(*window, boxes, box_texture);
    render_box_vector(*window, hook_boxes, roots_texture);

    // doors
    window->draw(end_sprite);
    window->draw(door_sprite);

    player->render_player(*window);

    window->draw(end_sprite);

    // set default view
    window->setView(default_view);
    window->draw(this->pause_button);
    window->display();
}

// ------------------- boxes -------------------

void Level2::init_boxes()
{
    // paredes
    boxes.push_back(create_ground(world, 812, -1695, 50, 2595, sf::Color::White, false));
    boxes.push_back(create_ground(world, 162, -1695, 50, 2355, sf::Color::White, false));

    // chão
    boxes.push_back(create_ground(world, 162, 480, 700, 120, sf::Color::White, false));

    // objetos
    boxes.push_back(create_ground(world, 365, 350, 40, 130, sf::Color::White, false));
    boxes.push_back(create_ground(world, 574, 400, 238, 80, sf::Color::White, false));
    boxes.push_back(create_ground(world, 573, 230, 139, 55, sf::Color::White, false));
    boxes.push_back(create_ground(world, 475, 106, 40, 203, sf::Color::White, false));
    boxes.push_back(create_ground(world, 212, 17, 63, 60, sf::Color::White, false));
    boxes.push_back(create_ground(world, 309, -191, 93, 41, sf::Color::White, false));
    boxes.push_back(create_ground(world, 531, -445, 42, 175, sf::Color::White, false));
    boxes.push_back(create_ground(world, 611, -198, 81, 41, sf::Color::White, false));
    boxes.push_back(create_ground(world, 416, -615, 42, 118, sf::Color::White, false));
    boxes.push_back(create_ground(world, 212, -645, 70, 42, sf::Color::White, false));
    boxes.push_back(create_ground(world, 356, -812, 119, 40, sf::Color::White, false));
    boxes.push_back(create_ground(world, 438, -1011, 70, 42, sf::Color::White, false));
    boxes.push_back(create_ground(world, 313, -1061, 42, 148, sf::Color::White, false));

    // teto
    boxes.push_back(create_ground(world, 314, -1195, 497, 42, sf::Color::White, false));

    // hooks
    hook_boxes.push_back(create_ground(world, 365, 310, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 752, 265, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 602, 122, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 602, -76, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 428, -76, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 235, -147, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 464, -325, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 622, -366, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 543, -576, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 347, -698, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 316, -812, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 523, -902, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 591, -1010, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 395, -1152, 40, 40, sf::Color::Yellow, true));
    hook_boxes.push_back(create_ground(world, 242, -1061, 40, 40, sf::Color::Yellow, true));
}