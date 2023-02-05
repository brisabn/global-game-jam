#include "states/tutorial-state.hpp"

void TutorialState::init()
{

    // setup play button
    assets->load_texture("tutorial", "resources/tutorial.png");
    this->resume_button.setTexture(assets->get_texture("tutorial"));
    // this->resume_button.setPosition((SCREEN_WIDTH / 2) - (this->resume_button.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 2) + this->resume_button.getGlobalBounds().height);
    this->resume_button.setPosition(0, 0);
    this->resume_button.setScale(0.57, 0.57);

    // // setup quit button
    // assets->load_texture("home_button", PAUSE_MENU_HOME_BUTTON);
    // this->home_button.setTexture(assets->get_texture("home_button"));
    // this->home_button.setPosition((SCREEN_WIDTH / 2) - (this->home_button.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 2) + this->home_button.getGlobalBounds().height * 2.2);
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
        else if (event.type == sf::Event::KeyPressed)
        {
            // std::cout << "aaa" << std::endl;
            add_state<IntroState>(true);
        }
    }

    // if (input->is_sprite_clicked(this->resume_button, sf::Mouse::Left, *window))
    // {
    // 	// Resume Game By Popping Of The Current State (Pause State)
    // 	remove_state();
    // }

    // if (input->is_sprite_clicked(this->home_button, sf::Mouse::Left, *window))
    // {
    // 	// Remove The Pause State Off The Stack
    // 	remove_state();

    // 	// Switch To Main Menu State By Replacing The Game State
    // 	add_state<MainMenuState>(true);
    // }
    // }
}

void TutorialState::update(float delta_time)
{
}

void TutorialState::draw(float delta_time)
{
    window->clear(sf::Color::Red);

    // window->draw(this->title);
    window->draw(this->resume_button);
    // window->draw(this->home_button);

    window->display();
}
