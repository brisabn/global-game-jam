#include "states/animation-demo-state.hpp"

void AnimationDemoState::init()
{
    animation = new pte::Animation("resources/spritesheet.png", 30, 44, 0.3f);
}

void AnimationDemoState::handle_input()
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (sf::Event::Closed == event.type)
        {
            window->close();
        }
    }
}

void AnimationDemoState::update(float delta_time)
{
    animation->update_animation(delta_time);
}

void AnimationDemoState::draw(float delta_time)
{
    window->clear(sf::Color::Black);

    sf::Sprite *spr = animation->get_sprite();
    spr->setPosition(100, 100);
    spr->setScale(2, 2);

    window->draw(*spr);

    window->display();
}
