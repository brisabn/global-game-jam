#include "states/final-state.hpp"

void FinalStrip::set_bool()
{
    show = true;
}

void FinalState::init()
{
    final_sequence();
}

void FinalState::final_sequence()
{

    // Texto
    if (!font.loadFromFile("resources/fonts/underwood.ttf"))
        std::cout << "Error loading font\n";

    write_sound.define_sound("resources/audio/typing.wav", 10.f);
    text_sequence("resources/final/final.txt");

    this->text.setCharacterSize(SCREEN_WIDTH / 40);
    this->text.setFont(font);
    this->text.setOrigin(sf::Vector2f(0.f, 0.f));
    this->text.setFillColor(sf::Color::White);
    this->text.setPosition(sf::Vector2f((SCREEN_WIDTH / 12), (SCREEN_HEIGHT / 1.37)));

    // Quadrinhos
    FinalStrip strip1;

    assets->load_texture("background", "resources/intro/cutscene_background.png");
    background.setTexture(assets->get_texture("background"));
    background.setScale(SCREEN_WIDTH / background.getLocalBounds().width, SCREEN_WIDTH / background.getLocalBounds().height);
    background.setPosition(0, 0);

    assets->load_texture("quadro1", "resources/final/cena_final_yuka.png");
    strip1.quadro.setTexture(assets->get_texture("quadro1"));
    strip1.quadro.setScale(0.8, 0.8);
    strip1.quadro.setPosition(sf::Vector2f((SCREEN_WIDTH / 10), (SCREEN_HEIGHT / 5.7)));
    final_strips.push_back(strip1);

    final_strips[0].show = true;
    set_string(final_description[0]);
}

void FinalState::text_sequence(std::string myfile)
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
            final_description.push_back(myline);
        }
    }
}

void FinalState::write()
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

void FinalState::set_string(std::string s)
{
    this->phrase = s;
    zerar = true;
}

void FinalState::reset()
{
    this->write_time.restart();
    this->itr = 0;
};

void FinalState::handle_input()
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
            if ((event.key.code == sf::Keyboard::Right) && (twice == false))
            {
                itr = phrase.size();
                a = 255;
                final_strips[0].quadro.setColor(sf::Color(255, 255, 255, a));
                twice = true;
            }
            else if ((event.key.code == sf::Keyboard::Right) && (twice == true))
            {
                add_state<MainCreditsState>(true);
            }
        }
    }
}

void FinalState::update(float delta_time)
{
}

void FinalState::update_final_sequence()
{
    if (final_strips[0].show)
    {
        if (a < 255)
        {
            final_strips[0].quadro.setColor(sf::Color(255, 255, 255, a));
            a++;
        }

        window->draw(final_strips[0].quadro);

        if (this->final_time.getElapsedTime().asSeconds() > 9.5)
        {
            if (b <= 255 && b > 0)
            {
                final_strips[0].quadro.setColor(sf::Color(255, 255, 255, b));
                this->text.setFillColor(sf::Color(255, 255, 255, b));
                b = b - 2;
            }
        }
        if (this->final_time.getElapsedTime().asSeconds() > 11.6)
            add_state<MainCreditsState>(true);
    }
}

void FinalState::draw(float delta_time)
{
    window->setFramerateLimit(60);
    window->setVerticalSyncEnabled(false);
    window->clear(sf::Color::White);

    window->draw(this->background);
    update_final_sequence();
    write();
    window->draw(this->text);
    if (curr == 0)
        window->draw(this->instruction);

    window->display();
}
