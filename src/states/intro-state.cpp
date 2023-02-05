#include "states/intro-state.hpp"

void IntroState::intro_sequence()
{
    // Texto
    if (!font.loadFromFile("resources/fonts/VT323.ttf"))
        std::cout << "Error loading font\n";

    write_sound.define_sound("resources/audio/typing.wav", 10.f);
    text_sequence("resources/intro/intro.txt");

    this->text.setCharacterSize(30);
    this->text.setFont(font);
    this->text.setOrigin(sf::Vector2f(0.f, 0.f));
    this->text.setFillColor(sf::Color::Black);
    this->text.setPosition(110, 400);

    // Quadrinhos
    sf::Sprite strip1, strip2, strip3;
    assets->load_texture("background", "resources/intro/intro_background.jpg");

    assets->load_texture("quadro1", "resources/intro/quadro_1.png");
    assets->load_texture("quadro2", "resources/intro/quadro_2.png");
    assets->load_texture("quadro3", "resources/intro/quadro_3.png");

    background.setTexture(assets->get_texture("background"));
    background.setScale(SCREEN_WIDTH / background.getLocalBounds().width, SCREEN_WIDTH / background.getLocalBounds().height);
    background.setPosition(0, 0);

    strip1.setTexture(assets->get_texture("quadro1"));
    strip1.setScale(1.2, 1.2);
    strip1.setPosition(sf::Vector2f((SCREEN_WIDTH / 80), (SCREEN_HEIGHT / 80)));
    intro_strips.push_back(strip1);

    strip2.setTexture(assets->get_texture("quadro2"));
    strip2.setScale(1.5, 1.5);
    strip2.setPosition(sf::Vector2f((SCREEN_WIDTH / 80), (SCREEN_HEIGHT / 80)));
    intro_strips.push_back(strip2);

    strip3.setTexture(assets->get_texture("quadro3"));
    strip3.setScale(1.5, 1.5);
    strip3.setPosition(sf::Vector2f((SCREEN_WIDTH / 80), (SCREEN_HEIGHT / 80)));
    intro_strips.push_back(strip3);
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
        text.setString(phrase);
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

void IntroState::init()
{
    intro_sequence();
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
    }
}

void IntroState::update(float delta_time)
{
    if (this->clock.getElapsedTime().asSeconds() > 10)
    {
        // Switch To Game
        add_state<GameState>(true);
    }
}

void IntroState::update_intro_sequence(float delta_time)
{
    if (this->clock.getElapsedTime().asSeconds() > 0.5)
    {
        if (a < 255)
        {
            intro_strips[0].setColor(sf::Color(255, 255, 255, a));
            a++;
        }
        set_string(intro_description[0]);
        window->draw(intro_strips[0]);
    }
    if (this->clock.getElapsedTime().asSeconds() > 1.5)
    {
        if (b < 255)
        {
            intro_strips[1].setColor(sf::Color(255, 255, 255, b));
            b++;
        }
        set_string(intro_description[1]);
        window->draw(intro_strips[1]);
    }
    if (this->clock.getElapsedTime().asSeconds() > 2.5)
    {
        if (c < 255)
        {
            intro_strips[2].setColor(sf::Color(255, 255, 255, c));
            c++;
        }
        set_string(intro_description[2]);
        window->draw(intro_strips[2]);
    }
    if (this->clock.getElapsedTime().asSeconds() > 3.5)
    {

        // add_state<GameState>(true);
    }
}

void IntroState::draw(float delta_time)
{
    window->setFramerateLimit(60);
    window->setVerticalSyncEnabled(false);
    window->clear(sf::Color::White);

    window->draw(this->background);

    update_intro_sequence(delta_time);
    write();
    window->draw(this->text);

    window->display();
}
