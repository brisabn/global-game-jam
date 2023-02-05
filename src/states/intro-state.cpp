#include "states/intro-state.hpp"

void IntroState::intro_sequence()
{
    sf::Sprite strip1, strip2, strip3;
    assets->load_texture("gato1", "resources/gato.jpg");
    assets->load_texture("gato2", "resources/gato2.jpg");
    assets->load_texture("gato3", "resources/gato3.jpg");

    strip1.setTexture(assets->get_texture("gato1"));
    strip1.setPosition(sf::Vector2f((SCREEN_WIDTH) - (strip1.getLocalBounds().width) / 2, (SCREEN_HEIGHT) - (strip1.getLocalBounds().height) / 2));
    intro_strips.push_back(strip1);

    strip2.setTexture(assets->get_texture("gato2"));
    strip2.setPosition(sf::Vector2f((SCREEN_WIDTH) - (strip2.getLocalBounds().width) / 2, (SCREEN_HEIGHT) - (strip2.getLocalBounds().height)));
    intro_strips.push_back(strip2);

    strip3.setTexture(assets->get_texture("gato3"));
    strip3.setPosition(sf::Vector2f((SCREEN_WIDTH) - (strip3.getLocalBounds().width), (SCREEN_HEIGHT) - (strip3.getLocalBounds().height)));
    intro_strips.push_back(strip3);
}

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
        // Switch To Main Menu
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
        window->draw(intro_strips[0]);
    }
    if (this->clock.getElapsedTime().asSeconds() > 1.5)
    {
        if (b < 255)
        {
            intro_strips[1].setColor(sf::Color(255, 255, 255, b));
            b++;
        }
        window->draw(intro_strips[1]);
    }
    if (this->clock.getElapsedTime().asSeconds() > 2.5)
    {
        if (c < 255)
        {
            intro_strips[2].setColor(sf::Color(255, 255, 255, c));
            c++;
        }
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
    update_intro_sequence(delta_time);

    window->display();
}
