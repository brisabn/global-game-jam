#include "states/animation-demo-state.hpp"

void AnimationDemoState::init()
{
    
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
}

void AnimationDemoState::draw(float delta_time)
{
    window->clear(sf::Color::Black);

    window->display();
}
