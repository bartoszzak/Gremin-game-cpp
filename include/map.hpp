#ifndef GREMIN_MAP_HPP
#define GREMIN_MAP_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>

std::vector<std::string> load_map(const std::string& path);

std::vector<sf::FloatRect> get_tiles(const std::vector<std::string>& game_map, float tile_size);

void draw_map(const std::vector<std::string>& game_map, float tile_size, sf::RenderWindow& display, float camera_pos[2],
              const sf::Texture& texture_1, const sf::Texture& texture_2);

#endif //GREMIN_MAP_HPP
