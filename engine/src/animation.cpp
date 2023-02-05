#include "animation.hpp"

namespace pte
{
    Animation::Animation(std::string filepath, int width, int height, int frames_qnt, int off_x, int off_y, float frame_interval)
    {
        this->frame_interval = frame_interval;
        
        // load thespritesheet texture
        if (!spritesheet_texture.loadFromFile(filepath))
        {
            std::cerr << "Failed to load sprite sheet: " << filepath << std::endl;
            return;
        }

        // width /= 7;
        // height /= 7;

        // float scale_x = width / spritesheet_texture.getSize().x ;
        // float scale_y = height/ spritesheet_texture.getSize().y ;
        
        sf::Vector2f scale ((float) width / (spritesheet_texture.getSize().x / frames_qnt), (float) height / spritesheet_texture.getSize().y);

        // std::cout << "width: " << width << " " << height << std::endl;
        // std::cout << "size: " << spritesheet_texture.getSize().x << " " << spritesheet_texture.getSize().y << std::endl;
        // std::cout << "scale: " << scale.x << " " << scale.y << std::endl;

        float real_width = spritesheet_texture.getSize().x / frames_qnt;

        // Create the sprites from the texture
        int sprite_count = frames_qnt;
        for (int i = 0; i < sprite_count; i++)
        {
            sf::Sprite sprite;
            sprite.setTexture(spritesheet_texture);
            sprite.setTextureRect(sf::IntRect(i * real_width, 0, real_width, spritesheet_texture.getSize().y));
            sprite.setScale(scale.x, scale.y);
            sprite.setOrigin((real_width / 2) - off_x, (spritesheet_texture.getSize().y / 2) + off_y);
            sprites.push_back(sprite);
        }
    }

    void Animation::update_animation(float delta_time)
    {
        if (!status)
        {
            return;
        }
        
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
    void Animation::play()
    {
        status = false;
    }

    void Animation::pause()
    {
        status = true;
    }
}