#include "states/menu-state.hpp"

void MainMenuState::init()
{
    // setup title
    assets->load_font("default_font", DEFAULT_FONT_PATH);
    this->title.setFont(assets->get_font("default_font"));
    this->title.setString("MAIN MENU");
    this->title.setCharacterSize(55);
    this->title.setFillColor(sf::Color::White);
    this->title.setPosition((SCREEN_WIDTH / 2) - (this->title.getGlobalBounds().width / 2), SCREEN_HEIGHT * 0.2);

    // setup play button
    assets->load_texture("play_button", MAIN_MENU_PLAY_BUTTON);
    this->play_button.setTexture(assets->get_texture("play_button"));
    this->play_button.setPosition((SCREEN_WIDTH / 2) - (this->play_button.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 2) + this->play_button.getGlobalBounds().height);

    // setup quit button
    assets->load_texture("quit_button", MAIN_MENU_QUIT_BUTTON);
    this->quit_button.setTexture(assets->get_texture("quit_button"));
    this->quit_button.setPosition((SCREEN_WIDTH / 2) - (this->quit_button.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 2) + this->quit_button.getGlobalBounds().height * 2.2);

    // window->setSize(sf::Vector2u(SCREEN_WIDTH * 1.5, SCREEN_HEIGHT * 1.5));
    // credits button
    assets->load_texture("credits_button", MAIN_MENU_PLAY_BUTTON);
    this->credits_button.setTexture(assets->get_texture("play_button"));
    this->credits_button.setPosition((SCREEN_WIDTH / 2) - (this->credits_button.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 2) + this->credits_button.getGlobalBounds().height * 3.4);


    // audio
    menu_music = new Audio();
    menu_music->define_sound("resources/music/soundtrack/ato_1_novo.ogg", 100);
    menu_music->play_sound();
}

void MainMenuState::handle_input()
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (sf::Event::Closed == event.type || input->is_sprite_clicked(this->quit_button, sf::Mouse::Left, *window))
        {
            window->close();
            
        }

        // if (sf::Event::KeyPressed)

        if (input->is_sprite_clicked(this->play_button, sf::Mouse::Left, *window))
        {
            // Switch to game state

            // menu_music->music.pause();
            menu_music->sound.pause();

            delete menu_music;
            add_state<TutorialState>(true);

        }
        if (input->is_sprite_clicked(this->credits_button, sf::Mouse::Left, *window))
        {
            // Remove The Menu State Off The Stack
            remove_state();

            // Switch To Main Credits State By Replacing The Game State
            add_state<MainCreditsState>(true);
        }
    }
}

void MainMenuState::update(float delta_time)
{
}

void MainMenuState::draw(float delta_time)
{
    window->clear(sf::Color(65, 11, 95));

    window->draw(this->title);
    window->draw(this->play_button);
    window->draw(this->quit_button);
    window->draw(this->credits_button);

    window->display();
}
