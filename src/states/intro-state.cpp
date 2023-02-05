#include "states/intro-state.hpp"

void IntroState::init()
{
    intro_sequence();
}

void IntroState::intro_sequence()
{

    // Texto
    if (!font.loadFromFile("resources/fonts/underwood.ttf"))
        std::cout << "Error loading font\n";

    write_sound.define_sound("resources/audio/typing.wav", 10.f);
    text_sequence("resources/intro/intro.txt");

    this->text.setCharacterSize(SCREEN_WIDTH / 40);
    this->text.setFont(font);
    this->text.setOrigin(sf::Vector2f(0.f, 0.f));
    this->text.setFillColor(sf::Color::Black);
    this->text.setPosition(sf::Vector2f((SCREEN_WIDTH / 12.5), (SCREEN_HEIGHT / 1.2)));

    // Quadrinhos
    Strip strip1, strip2, strip3, strip4, strip5, strip6, strip7;
    assets->load_texture("background", "resources/intro/intro_background.jpg");

    assets->load_texture("quadro1", "resources/intro/quadro_1.png");
    assets->load_texture("quadro2", "resources/intro/quadro_2.png");
    assets->load_texture("quadro3", "resources/intro/quadro_3.png");
    assets->load_texture("quadro4", "resources/intro/quadro_4.png");
    assets->load_texture("quadro5", "resources/intro/quadro_5.png");
    assets->load_texture("quadro6", "resources/intro/quadro_6.png");
    assets->load_texture("quadro7", "resources/intro/quadro_7.png");

    background.setTexture(assets->get_texture("background"));
    background.setScale(SCREEN_WIDTH / background.getLocalBounds().width, SCREEN_WIDTH / background.getLocalBounds().height);
    background.setPosition(0, 0);

    strip1.quadro.setTexture(assets->get_texture("quadro1"));
    strip1.quadro.setScale(1.35, 1.35);
    strip1.quadro.setPosition(sf::Vector2f((SCREEN_WIDTH / 10), (SCREEN_HEIGHT / 300)));
    intro_strips.push_back(strip1);

    strip2.quadro.setTexture(assets->get_texture("quadro2"));
    strip2.quadro.setScale(1.35, 1.35);
    strip2.quadro.setPosition(sf::Vector2f((SCREEN_WIDTH / 12.5), (SCREEN_HEIGHT / 300)));
    intro_strips.push_back(strip2);

    strip3.quadro.setTexture(assets->get_texture("quadro3"));
    strip3.quadro.setScale(1.35, 1.35);
    strip3.quadro.setPosition(sf::Vector2f((SCREEN_WIDTH / 12.5), (SCREEN_HEIGHT / 300)));
    intro_strips.push_back(strip3);

    strip4.quadro.setTexture(assets->get_texture("quadro4"));
    strip4.quadro.setScale(1.35, 1.35);
    strip4.quadro.setPosition(sf::Vector2f((SCREEN_WIDTH / 30), (SCREEN_HEIGHT / 300)));
    intro_strips.push_back(strip4);

    strip5.quadro.setTexture(assets->get_texture("quadro5"));
    strip5.quadro.setScale(1.35, 1.35);
    strip5.quadro.setPosition(sf::Vector2f((SCREEN_WIDTH / 300), (SCREEN_HEIGHT / 300)));
    intro_strips.push_back(strip5);

    strip6.quadro.setTexture(assets->get_texture("quadro6"));
    strip6.quadro.setScale(1.35, 1.35);
    strip6.quadro.setPosition(sf::Vector2f((SCREEN_WIDTH / 30), (SCREEN_HEIGHT / 2.5)));
    intro_strips.push_back(strip6);

    strip7.quadro.setTexture(assets->get_texture("quadro7"));
    strip7.quadro.setScale(1.35, 1.35);
    strip7.quadro.setPosition(sf::Vector2f((SCREEN_WIDTH / 300), (SCREEN_HEIGHT / 2.5)));
    intro_strips.push_back(strip7);
}

void IntroState::text_sequence(std::string myfile)
{
    text_file = std::ifstream(myfile);
    std::string myline;

    if (text_file.is_open())
    {
        while (text_file)
        {
            std::getline(text_file, myline);

            // identifica quebra de linha no arquivo
            size_t start_pos = 0;
            while ((start_pos = myline.find("\\n", start_pos)) != std::string::npos)
            {
                myline.replace(start_pos, 2, "\n");
                start_pos += 1;
            }
            intro_description.push_back(myline);
        }
    }
}

void IntroState::write()
{
    if (zerar)
    {
        if (this->write_time.getElapsedTime().asSeconds() > .025f)
        {
            this->write_time.restart();
            this->itr++;
            write_sound.play_sound();
        }
        this->text.setString(phrase.substr(0, this->itr));
    }
    if (itr >= phrase.size())
    {
        zerar = false;
        itr = 0;
    }
}

void IntroState::set_string(std::string s)
{
    this->phrase = s;
    zerar = true;
}

void IntroState::reset()
{
    this->write_time.restart();
    this->itr = 0;
};

void Strip::set_bool()
{
    show = true;
}

void IntroState::handle_input()
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (sf::Event::Closed == event.type)
        {
            window->close();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Right)
            {
                if ((curr < intro_description.size() - 1) && (curr <= 3))
                {
                    std::cout << curr << std::endl;
                    set_string(intro_description[curr]);
                    intro_strips[curr].set_bool();
                    reset();
                    curr++;
                }
                if ((curr > 3) && (curr <= 6))
                {
                    intro_strips[0].show = false;
                    intro_strips[1].show = false;
                    intro_strips[2].show = false;

                    set_string(intro_description[curr]);
                    intro_strips[curr].set_bool();
                    reset();
                    curr++;
                }
                if (curr > 6)
                {
                    add_state<GameState>(true);
                }
            }
        }
    }
}

void IntroState::update(float delta_time)
{
}

void IntroState::update_intro_sequence()
{
    if (intro_strips[0].show)
    {
        window->draw(intro_strips[0].quadro);
    }
    if (intro_strips[1].show)
    {
        window->draw(intro_strips[1].quadro);
    }
    if (intro_strips[2].show)
    {
        window->draw(intro_strips[2].quadro);
    }
    if (intro_strips[3].show)
    {
        window->draw(intro_strips[3].quadro);
    }
    if (intro_strips[4].show)
    {
        window->draw(intro_strips[4].quadro);
    }
    if (intro_strips[5].show)
    {
        window->draw(intro_strips[5].quadro);
    }
    if (intro_strips[6].show)
    {
        window->draw(intro_strips[6].quadro);
    }
}

void IntroState::draw(float delta_time)
{
    window->setFramerateLimit(60);
    window->setVerticalSyncEnabled(false);
    window->clear(sf::Color::White);

    window->draw(this->background);
    update_intro_sequence();
    write();
    window->draw(this->text);

    window->display();
}
