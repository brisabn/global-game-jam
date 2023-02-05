#ifndef PTE_INTRO_STATE_HPP
#define PTE_INTRO_STATE_HPP

#include "engine/include/generic-state.hpp"
#include "states/game-state.hpp"
#include "definitions.hpp"
#include "audio.hpp"

class IntroState : public pte::GenericState
{

protected:
    std::vector<sf::Sprite> intro_strips;
    std::vector<std::string> intro_description;

    std::size_t curr = 0;
    std::size_t itr = 0;

    int a = 0, b = 0, c = 0;

    sf::Text text;
    std::ifstream text_file;
    sf::Font font;
    sf::Sprite background;

    bool zerar;

    std::string phrase;
    sf::Clock write_time;
    Audio write_sound;
    void reset();

    void intro_sequence();

    void text_sequence(std::string myfile);
    void write();
    void set_string(std::string);

    void update_intro_sequence(float delta_time);

public:
    using GenericState::GenericState;

    void init();
    void handle_input();
    void update(float delta_time);
    void draw(float delta_time);
};

#endif