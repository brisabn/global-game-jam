#include "states/menu-state.hpp"

void MainMenuState::init()
{

    // setup play button
    assets->load_texture("play_button", MAIN_MENU_PLAY_BUTTON);
    this->play_button.setTexture(assets->get_texture("play_button"));
    this->play_button.setScale(0.35, 0.35);
    this->play_button.setPosition((SCREEN_WIDTH / 2) + (this->play_button.getGlobalBounds().width / 2) * 2.5f, (SCREEN_HEIGHT / 2) + this->play_button.getGlobalBounds().height * (1.0));

    // setup quit button
    assets->load_texture("quit_button", MAIN_MENU_QUIT_BUTTON);
    this->quit_button.setTexture(assets->get_texture("quit_button"));
    this->quit_button.setScale(0.35, 0.35);
    this->quit_button.setPosition((SCREEN_WIDTH / 2) + (this->quit_button.getGlobalBounds().width / 2) * 2.5f, (SCREEN_HEIGHT / 2) + this->quit_button.getGlobalBounds().height * (3.f));

    // credits button
    assets->load_texture("credits_button", MAIN_MENU_CREDITS_BUTTON);
    this->credits_button.setTexture(assets->get_texture("credits_button"));
    this->credits_button.setScale(0.35, 0.35);
    this->credits_button.setPosition((SCREEN_WIDTH / 2) + (this->credits_button.getGlobalBounds().width / 2) * 2.5f, (SCREEN_HEIGHT / 2) + this->credits_button.getGlobalBounds().height * (2.f));

    // background
    assets->load_texture("background", MAIN_MENU_BACKGROUND);
    this->background.setTexture(assets->get_texture("background"));
    this->background.setScale(0.6f, 0.6f);
    this->background.setPosition((SCREEN_WIDTH / 2) - (this->background.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 2) - (this->background.getGlobalBounds().height / 2));

    // audio
    menu_music = new Audio();
    menu_music->define_music("resources/music/soundtrack/ato_1_novo.ogg", 100);
    menu_music->play_music();
    music_on = true;
}

void MainMenuState::handle_input()
{
    // check for music and pause
    if (music_on == false)
    {
        std::cout << "false" << std::endl;
        menu_music->music.play();
        music_on = true;
    }

    sf::Event event;

    while (window->pollEvent(event))
    {
        if (sf::Event::Closed == event.type || input->is_sprite_clicked(this->quit_button, sf::Mouse::Left, *window))
        {
            window->close();
        }

        if (input->is_sprite_clicked(this->play_button, sf::Mouse::Left, *window))
        {
            // Stop music
            menu_music->music.stop();
            delete menu_music;

            // Switch to tutorial state
            add_state<TutorialState>(true);
        }
        if (input->is_sprite_clicked(this->credits_button, sf::Mouse::Left, *window))
        {
            // Switch to main credits
            menu_music->music.pause();

            music_on = false;
            add_state<MainCreditsState>(false);
        }
    }
}

void MainMenuState::update(float delta_time)
{
}

void MainMenuState::draw(float delta_time)
{
    window->clear(sf::Color(65, 11, 95));
    window->draw(this->background);
    window->draw(this->title);
    window->draw(this->play_button);
    window->draw(this->quit_button);
    window->draw(this->credits_button);

    window->display();
}
