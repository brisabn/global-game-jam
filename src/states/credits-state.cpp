#include "states/credits-state.hpp"

void MainCreditsState::init()
{
    // setup title
    assets->load_font("default_font", DEFAULT_FONT_PATH);
    this->title.setFont(assets->get_font("default_font"));
    this->title.setString("CREDITS");
    this->title.setCharacterSize(55);
    this->title.setFillColor(sf::Color::White);
    this->title.setPosition((SCREEN_WIDTH / 2) - (this->title.getGlobalBounds().width / 2), SCREEN_HEIGHT * 0.2);

   // setup quit button
	assets->load_texture("menu_button", CREDITS_MENU_BUTTON);
	this->menu_button.setTexture(assets->get_texture("menu_button"));
    this->menu_button.setScale(0.35, 0.35);
	this->menu_button.setPosition((SCREEN_WIDTH / 2) - (this->menu_button.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 2) + this->menu_button.getGlobalBounds().height * (-1.0));
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

    window->draw(this->title);
    
    window->draw(this->menu_button);

    window->display();
}