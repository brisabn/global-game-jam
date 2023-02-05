#include "animation.hpp"

namespace pte
{
    Animation::Animation(std::string filepath, int width, int height, float frame_interval)
    {
        this->frame_interval = frame_interval;
        
        // load thespritesheet texture
        if (!spritesheet_texture.loadFromFile(filepath))
        {
            std::cerr << "Failed to load sprite sheet: " << filepath << std::endl;
            return;
        }

        // Create the sprites from the texture
        int sprite_count = spritesheet_texture.getSize().x / width;
        for (int i = 0; i < sprite_count; i++)
        {
            sf::Sprite sprite;
            sprite.setTexture(spritesheet_texture);
            sprite.setTextureRect(sf::IntRect(i * width, 0, width, height));
            sprites.push_back(sprite);
        }
    }

    void Animation::update_animation(float delta_time)
    {
        time_since_last_frame += delta_time;

        if (time_since_last_frame >= frame_interval)
        {
            current_sprite = (current_sprite + 1) % sprites.size();
            time_since_last_frame = 0.0f;
        }
    }

    sf::Sprite *Animation::get_sprite()
    {
        return &sprites[current_sprite];
    }

    void Animation::reset()
    {
        current_sprite = 0;
        time_since_last_frame = 0.0f;
    }
}