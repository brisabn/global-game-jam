#include "states/tutorial-state.hpp"

void TutorialState::init()
{
    // setup background
    assets->load_texture("tutorial", "resources/tutorial.png");
    this->background.setTexture(assets->get_texture("tutorial"));

    // define scale using texture size
    float scale_x = (float) SCREEN_WIDTH / background.getTexture()->getSize().x;
    float scale_y = (float) SCREEN_HEIGHT / background.getTexture()->getSize().y;

    // define propeties
    background.setPosition(0, 0);
    background.setScale(scale_x, scale_y);
}

void TutorialState::handle_input()
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window->close();
        }
        // "press any key to continue"
        else if (event.type == sf::Event::KeyPressed)
        {
            add_state<IntroState>(true);
        }
    }
}

void TutorialState::update(float delta_time)
{
}

void TutorialState::draw(float delta_time)
{
    window->clear(sf::Color::Red);

    window->draw(this->background);

    window->display();
}
