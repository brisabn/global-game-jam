#include "audio.hpp"

Audio::Audio()
{
}

void Audio::define_music(std::string myfile, float volume)
{
    audio_file = std::ifstream(myfile);
    this->music.openFromFile(myfile);
    this->music.setVolume(volume);
}

void Audio::play_music()
{
    if (music.getStatus() == sf::Music::Stopped)
    {
        music.play();
    }
}

void Audio::define_sound(std::string myfile, float volume)
{
    audio_file = std::ifstream(myfile);
    this->buffer.loadFromFile(myfile);
    this->sound.setBuffer(buffer);
    this->sound.setVolume(volume);
}

void Audio::play_sound()
{
    if (sound.getStatus() == sf::Sound::Stopped)
    {
        sound.play();
    }
}