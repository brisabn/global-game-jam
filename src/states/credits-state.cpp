#include "states/credits-state.hpp"

void MainCreditsState::init()
{

    // setup menu button
    assets->load_texture("menu_button", CREDITS_MENU_BUTTON);
    this->menu_button.setTexture(assets->get_texture("menu_button"));
    this->menu_button.setScale(0.35, 0.35);
    this->menu_button.setPosition((SCREEN_WIDTH / 2) - (this->menu_button.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 2) + this->menu_button.getGlobalBounds().height * (-1.0));

    // set quit button
    assets->load_texture("quit_button", MAIN_MENU_QUIT_BUTTON);
    this->quit_button.setTexture(assets->get_texture("quit_button"));
    this->quit_button.setScale(0.35, 0.35);
    this->quit_button.setPosition((SCREEN_WIDTH / 2) + (this->quit_button.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 2) + this->quit_button.getGlobalBounds().height * (-2.f));

    // background
    assets->load_texture("background", "resources/main-menu/credits.png");
    this->background.setTexture(assets->get_texture("background"));
    this->background.setScale(0.6f, 0.6f);
    this->background.setPosition((SCREEN_WIDTH / 2) - (this->background.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 2) - this->background.getGlobalBounds().height / 2);
}

void MainCreditsState::handle_input()
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (input->is_sprite_clicked(this->menu_button, sf::Mouse::Left, *window))
        {
            // Remove The Pause State Off The Stack
            remove_state();

            // Switch To Main Menu State By Replacing The Game State
            add_state<MainMenuState>(true);
        }
    }
}

void MainCreditsState::update(float delta_time)
{
}

void MainCreditsState::draw(float delta_time)
{
    window->clear(sf::Color::Blue);
    window->draw(this->background);
    window->draw(this->menu_button);
    window->draw(this->quit_button);

    window->display();
}