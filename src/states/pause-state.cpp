#include "states/pause-state.hpp"

void PauseState::init()
{
	// setup play button
	assets->load_texture("resume_button", PAUSE_MENU_RESUME_BUTTON);
	this->resume_button.setScale(0.35, 0.35);
	this->resume_button.setTexture(assets->get_texture("resume_button"));
	this->resume_button.setPosition((SCREEN_WIDTH / 2) - (this->resume_button.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 2) + this->resume_button.getGlobalBounds().height);

	// setup background
	background_texture.loadFromFile("resources/pause_background.png");
	background.setTexture(background_texture);
	float scale_x = (float) SCREEN_WIDTH / background.getTexture()->getSize().x;
    float scale_y = (float) SCREEN_HEIGHT / background.getTexture()->getSize().y;
	background.setPosition(sf::Vector2f(0, 0));
	background.setScale(sf::Vector2f(scale_x, scale_y));

	// setup quit button
	assets->load_texture("home_button", CREDITS_MENU_BUTTON);
	this->home_button.setScale(0.35, 0.35);
	this->home_button.setTexture(assets->get_texture("home_button"));
	this->home_button.setPosition((SCREEN_WIDTH / 2) - (this->home_button.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 2) + this->home_button.getGlobalBounds().height * (-1.2));
}

void PauseState::handle_input()
{
	sf::Event event;

	while (window->pollEvent(event))
	{
		if (sf::Event::Closed == event.type)
		{
			window->close();
		}

		if (input->is_sprite_clicked(this->resume_button, sf::Mouse::Left, *window))
		{
			// Resume Game By Popping Of The Current State (Pause State)
			remove_state();
		}

		if (input->is_sprite_clicked(this->home_button, sf::Mouse::Left, *window))
		{
			// Remove The Pause State Off The Stack
			remove_state();
			
			// Switch To Main Menu State By Replacing The Game State
			add_state<MainMenuState>(true);
		}
	}
}

void PauseState::update(float delta_time)
{
}

void PauseState::draw(float delta_time)
{
	window->clear(sf::Color::Red);

	window->draw(background);
	window->draw(this->resume_button);
	window->draw(this->home_button);

	window->display();
}
