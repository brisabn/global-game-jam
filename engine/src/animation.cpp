#include "animation.hpp"

namespace pte
{
    Animation::Animation(std::string filepath)
    {
        // load spritesheet texture
        spritesheet_texture.loadFromFile(filepath);
    }
}