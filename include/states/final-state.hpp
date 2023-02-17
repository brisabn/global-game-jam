#ifndef PTE_FINAL_STATE_HPP
#define PTE_FINAL_STATE_HPP

#include "engine/include/generic-state.hpp"
#include "states/game-state.hpp"
#include "definitions.hpp"
#include "audio.hpp"

class FinalStrip
{
public:
    bool show = false;
    sf::Sprite quadro;

    void set_bool();
};

class FinalState : public pte::GenericState
{

protected:
    std::vector<FinalStrip> final_strips;
    std::vector<std::string> final_description;

    std::size_t curr = 0;
    std::size_t itr = 0;
    int a = 0, b = 255;

    sf::Text text;
    sf::Text instruction;
    std::ifstream text_file;
    sf::Font font;
    sf::Sprite background;
    sf::Clock final_time;

    bool zerar, twice;

    std::string phrase;
    sf::Clock write_time;
    Audio write_sound;
    void reset();

    void final_sequence();

    void text_sequence(std::string myfile);
    void write();
    void set_string(std::string);

    void update_final_sequence();

public:
    using GenericState::GenericState;

    void init();
    void handle_input();
    void update(float delta_time);
    void draw(float delta_time);
};

#endif