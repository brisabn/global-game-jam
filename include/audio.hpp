
#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>

#include <iostream>
#include <fstream>

#include <ctime>
#include <cstdlib>
#include <stack>

class Audio
{
    // usar singleton? como?
private:
public:
    sf::Music music;

    sf::SoundBuffer buffer;
    sf::Sound sound;

    std::ifstream audio_file;
    
    Audio();
    void define_music(std::string myfile, float volume);
    void play_music();
    void define_sound(std::string myfile, float volume);
    void play_sound();
};

#endif
