#include "states/credits-state.hpp"

void MainCreditsState::init()
{
    // background
    assets->load_texture("background", "resources/main-menu/credits.png");
    this->background.setTexture(assets->get_texture("background"));

    // define scale using texture size
    float scale_x = (float)SCREEN_WIDTH / background.getTexture()->getSize().x;
    float scale_y = (float)SCREEN_HEIGHT / background.getTexture()->getSize().y;

    background.setPosition(0, 0);
    background.setScale(scale_x, scale_y);

    // start delta_time
    total_time = 0;
}

void MainCreditsState::handle_input()
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window->close();
        }
        // "press any key to continue"
        else if (event.type == sf::Event::KeyPressed && total_time > 0.5)
        {
            add_state<MainMenuState>(true);
        }
    }
}

void MainCreditsState::update(float delta_time)
{
    if (total_time < 1)
    {
        total_time += delta_time;
    }
}

void MainCreditsState::draw(float delta_time)
{
    window->clear(sf::Color::Blue);

    window->draw(this->background);

    window->display();
}