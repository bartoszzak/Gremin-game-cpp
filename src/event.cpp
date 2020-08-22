#include <SFML-2.5.1/include/SFML/Window/Event.hpp>
#include "event.hpp"

void handle_events(Player& player, sf::RenderWindow& window, sf::Event& event, bool& run) {
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                run = false;
                break;

            case sf::Event::KeyPressed:
                switch (event.key.code) {
                    case sf::Keyboard::Right:
                        player.set_moving_right(true);
                        player.set_last_movement_right(true);
                        break;

                    case sf::Keyboard::Left:
                        player.set_moving_left(true);
                        player.set_last_movement_right(false);
                        break;

                    case sf::Keyboard::Up:
                        player.set_jumping(true);
                        break;

                    case sf::Keyboard::Space:
                        player.set_is_attacking(true);
                        //player.draw_attack_hitbox(display);
                        break;

                    case sf::Keyboard::R:
                        player.set_pos_x(50.0f);
                        player.set_pos_y(0.0f);

                    default:
                        break;
                }
                break;

            case sf::Event::KeyReleased:
                switch (event.key.code) {
                    case sf::Keyboard::Right:
                        player.set_moving_right(false);
                        break;

                    case sf::Keyboard::Left:
                        player.set_moving_left(false);
                        break;

                    default:
                        break;
                }

            default:
                break;
        }
    }

}