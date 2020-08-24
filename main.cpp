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

    //PLAYER TEXTURE
    sf::Image p_img;
    p_img.loadFromFile("sprites/player.png");
    p_img.createMaskFromColor(sf::Color(255, 255, 255));
    sf::Texture player_image;
    player_image.loadFromImage(p_img);


    //ENEMY TEXTURE
    sf::Image e_img;
    e_img.loadFromFile("sprites/STROJ-PASTUSZEK-kostium-ludowy-GORAL-baca-122-128.png");
    sf::Texture enemy_image;
    enemy_image.loadFromImage(e_img);

    sf::SoundBuffer buffer;
    buffer.loadFromFile("sounds/bruh.wav");
    sf::Sound bruh(buffer);

    Player gremin(50, 0, 16, 40, 100, player_image, tiles, bruh, 3.0f);
    Enemy enemy(100, 0, 40, 40, 100, enemy_image, tiles, bruh);

    while (run) {
        window.clear(sf::Color(51, 153, 255));

        ///EVENT HANDLING
        handle_events(gremin, window, event, run);

        for (auto& elem : Character::char_rect_dict) {
            elem.first->update();
            elem.first->draw(window, camera_pos);
        }

        draw_map(map01, TILE_SIZE, window, camera_pos, dirt_img, grass_img);
        gremin.draw_attack_hitbox(window, camera_pos); //TODO: WYJEBAC

        camera_pos[0] = gremin.get_pos_x() - VIEW_SIZE[0] / 2;
        camera_pos[1] = gremin.get_pos_y() - VIEW_SIZE[1] / 2;

        window.display();

    }

    return EXIT_SUCCESS;
}

