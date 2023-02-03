#ifndef PTE_ANIMATION_HPP
#define PTE_ANIMATION_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

namespace pte
{
    class Animation
    {
    private:
        sf::Texture spritesheet_texture;
        std::vector<sf::Sprite> sprites;

    public:
        Animation(std::string filepath);
        sf::Sprite *get_sprite();
        void update_animation(float delta_time, int width, int height);
    };
}

#endif