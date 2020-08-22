#include "map.hpp"


std::vector<std::string> load_map(const std::string& path) {
    std::vector<std::string> game_map;
    std::ifstream f;
    f.open(path + ".txt", std::ios::out);
    std::string line;
    while (std::getline(f, line)) {
        game_map.push_back(line);
    }
    f.close();
    return game_map;
}

std::vector<sf::FloatRect> get_tiles(const std::vector<std::string>& game_map, float tile_size) {
    std::vector<sf::FloatRect> tiles;
    for (std::size_t y = 0; y < game_map.size(); y++) {
        for (std::size_t x = 0; x < game_map[0].size(); x++) {
            if (game_map[y][x] != '0') {
                tiles.emplace_back(x * tile_size, y * tile_size, tile_size, tile_size);
            }
        }
    }
    return tiles;
}

void draw_map(const std::vector<std::string>& game_map, float tile_size, sf::RenderWindow& display, float* camera_pos,
              const sf::Texture& texture_1, const sf::Texture& texture_2) {
    for (std::size_t y = 0; y < game_map.size(); y++) {
        for (std::size_t x = 0; x < game_map[0].size(); x++) {
            if (game_map[y][x] == '1') {
                sf::Sprite sprite(texture_1);
                sprite.setPosition(x * tile_size - camera_pos[0], y * tile_size - camera_pos[1]);
                display.draw(sprite);
            } else if (game_map[y][x] == '2') {
                sf::Sprite sprite(texture_2);
                sprite.setPosition(x * tile_size - camera_pos[0], y * tile_size - camera_pos[1]);
                display.draw(sprite);
            }
        }
    }
}
