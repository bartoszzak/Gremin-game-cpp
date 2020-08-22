/// Classes related to characters

#ifndef GREMIN_CHARACTERS_HPP
#define GREMIN_CHARACTERS_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


class Entity {
public:
    Entity(float pos_x, float pos_y, float width, float height);

    Entity() = default;

    sf::FloatRect entity_rect;

    [[nodiscard]] float get_pos_x() const { return pos_x_; }

    void set_pos_x(float val) { pos_x_ = val; }

    void add_pos_x(float val) { pos_x_ += val; }

    [[nodiscard]] float get_pos_y() const { return pos_y_; }

    void set_pos_y(float val) { pos_y_ = val; }

    void add_pos_y(float val) { pos_y_ += val; }

    [[nodiscard]] float get_width() const { return width_; }

    [[nodiscard]] float get_height() const { return height_; }

    virtual ~Entity() = default;

private:
    float pos_x_{0};
    float pos_y_{0};
    float width_{0};
    float height_{0};

};


class Character : public Entity {
public:
    inline static float gravity{1};
    inline static std::unordered_map<Character*, sf::Rect<float>> char_rect_dict{0};

    //CONSTRUCTORS
    Character(float pos_x, float pos_y, float width, float height, int health, std::vector<sf::Rect<float>> tiles,
              const sf::Sound& death_sound, float movement_vel = 5.0f, float jump_vel = 20.0f,
              float terminal_vel = 15.0f);

    Character() = default;

    //GETTERS / SETTERS
    void add_health(int difference) { health_ += difference; }

    void set_health(int val) { health_ = val; }

    [[nodiscard]] int get_health() const { return health_; }

    [[nodiscard]] bool get_moving_right() const { return moving_right_; }

    void set_moving_right(bool val) { moving_right_ = val; }

    [[nodiscard]] bool get_moving_left() const { return moving_left_; }

    void set_moving_left(bool val) { moving_left_ = val; }

    [[nodiscard]] bool get_last_movement_right() const { return last_movement_right_; }

    void set_last_movement_right(bool val) { last_movement_right_ = val; }

    void set_jumping(bool val) { jumping_ = val; }

    //MOVEMENT METHODS
    std::vector<sf::Rect<float>> movement_collision_test(); ///Returns a vector of rectangles(tiles)
                                                            ///the character is colliding with

    void move(); ///Character movement, jumping and collisions.

    //DEATH / DAMAGE METHODS
    bool check_if_dead();

    void damage(int val) { add_health(-val); std::cout << get_health() << std::endl; }

    virtual void draw(sf::RenderWindow& display, float camera_pos[2]) = 0;

    virtual void update() { if (!check_if_dead()){ move(); } } //TODO: OVERRIDE IN OTHER CLASSES

    ~Character() override = default;

private:
    int health_{0};
    std::vector<sf::Rect<float>> tiles_;
    sf::Sound death_sound_;
    float movement_vel_{0};
    float jump_vel_{0};
    float terminal_vel_{0};
    bool moving_right_{false};
    bool moving_left_{false};
    bool last_movement_right_{true};
    bool jumping_{false};
    bool can_jump_{false};
    float fall_vel_{0};


};


class Player : public Character {
public:
    Player(float pos_x, float pos_y, float width, float height, int health, const sf::Texture& texture,
           std::vector<sf::Rect<float>> tiles,
           const sf::Sound& death_sound, float movement_vel = 3.0f, float jump_vel = 15.0f, float terminal_vel = 5.0f);

    void set_is_attacking(bool val) { is_attacking_ = val; }

    //TODO: draw function. tutaj albo osobny plik do obslugi tego gowna
    void draw(sf::RenderWindow& display, float camera_pos[2]) override;

    std::vector<Character*> attack_collision_test();

    void attack(); //TODO: ZLE ZROBIONY HITBOX, W OGOLE NIE ZADAJE TEZ OBRAZEN :(

    void respawn();

    void update() override;

private:
    const sf::Texture texture_;
    sf::Sprite sprite_;

    int attack_damage_{0};
    float attack_range_[2]{200.0f, 20.0f};
    sf::Rect<float> attack_rect{0, 0, attack_range_[0], attack_range_[1]};
    bool is_attacking_ = false;
    float respawn_pos_[2]{get_pos_x(), get_pos_y()};
    int respawn_hp{get_health()};
};


class Enemy : public Character {
public:
    Enemy(float pos_x, float pos_y, float width, float height, int health, const sf::Texture& texture, std::vector<sf::Rect<float>> tiles,
          const sf::Sound& death_sound, float movement_vel = 5.0f, float jump_vel = 20.0f,
          float terminal_vel = 15.0f);

    Enemy() = default;

    void draw(sf::RenderWindow& display, float camera_pos[2]) override;

private:
    const sf::Texture texture_;
    sf::Sprite sprite_;
};


class Paleta : public Entity {

};

#endif //GREMIN_CHARACTERS_HPP
