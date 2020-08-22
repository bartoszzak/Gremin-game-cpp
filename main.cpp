#include <cstdlib>
#include "characters.hpp"
#include "event.hpp"
#include "map.hpp"
#include "camera.hpp"

int main() {
    const float TILE_SIZE = 16.0f;
    const unsigned int WINDOW_SIZE[2] = {1440, 900};
    const float VIEW_SIZE[2] = {700, 400};

    sf::RenderWindow window{sf::VideoMode{WINDOW_SIZE[0], WINDOW_SIZE[1]}, "Gremin: the Game"};
    sf::View view{sf::FloatRect(0, 0, VIEW_SIZE[0], VIEW_SIZE[1])};
    window.setFramerateLimit(60);
    window.setView(view);

    bool run = true;
    sf::Event event; //has to be uninitialized apparently


    auto map01 = load_map("maps/map01");
    sf::Texture grass_img;
    grass_img.loadFromFile("sprites/grass.png");
    sf::Texture dirt_img;
    dirt_img.loadFromFile("sprites/dirt.png");

    float camera_pos[2] = {0.0f, 0.0f};

    auto tiles = get_tiles(map01, TILE_SIZE);

    sf::Texture player_image;
    player_image.loadFromFile("sprites/player.png");
    sf::Sound sample_sound;
    Player gremin(50, 0, 16, 40, 100, player_image, tiles, sample_sound, 5.0f);

    while (run) {
        window.clear(sf::Color::Black);

        ///EVENT HANDLING
        handle_events(gremin, window, event, run);

        gremin.update();
        gremin.draw(window, camera_pos);
        draw_map(map01, TILE_SIZE, window, camera_pos, dirt_img, grass_img);
        window.display();

    }

    return EXIT_SUCCESS;
}

