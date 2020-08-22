#include <algorithm>
#include <utility>
#include "characters.hpp"


Entity::Entity(float pos_x, float pos_y, float width, float height)
        : entity_rect(pos_x, pos_y, width, height),
          pos_x_(pos_x),
          pos_y_(pos_y),
          width_(width),
          height_(height) {}


Character::Character(float pos_x, float pos_y, float width, float height, int health,
                     std::vector<sf::Rect<float>> tiles, const sf::Sound& death_sound, float movement_vel,
                     float jump_vel, float terminal_vel)
        : Entity(pos_x, pos_y, width, height),
          health_(health),
          tiles_(std::move(tiles)),
          death_sound_(death_sound),
          movement_vel_(movement_vel),
          jump_vel_(jump_vel),
          terminal_vel_(terminal_vel) { char_rect_dict.emplace(this, entity_rect); }

std::vector<sf::FloatRect> Character::movement_collision_test() {
    std::vector<sf::FloatRect> collisions;
    entity_rect.left = get_pos_x();
    entity_rect.top = get_pos_y();

    for (const auto& tile : tiles_) {
        if (tile.intersects(entity_rect)) { collisions.emplace_back(tile); }
    }

    return collisions;
}

void Character::move() {
    std::unordered_map<std::string, bool> collision_types{{"top",    false},
                                                          {"bottom", false},
                                                          {"right",  false},
                                                          {"left",   false}};

    float movement[2] = {0.0f, 0.0f};
    if (moving_right_) { movement[0] += movement_vel_; }
    if (moving_left_) { movement[0] -= movement_vel_; }

    movement[1] += fall_vel_;
    fall_vel_ += gravity;
    if (fall_vel_ > terminal_vel_) { fall_vel_ = terminal_vel_; }

    add_pos_x(movement[0]);
    auto collisions = movement_collision_test();
    for (const auto& tile : collisions) {
        if (movement[0] > 0.0f) {
            set_pos_x(tile.left - get_width());
            collision_types.at("right") = true;
        } else if (movement[0] < 0.0f) {
            set_pos_x(tile.left + tile.width); //TODO: Sprawdzic czy dobra wartosc
            collision_types.at("left") = true;
        }
    }
    add_pos_y(movement[1]);
    collisions = movement_collision_test();
    for (const auto& tile : collisions) {
        if (movement[1] > 0.0f) {
            set_pos_y(tile.top - get_height());
            collision_types["bottom"] = true;
        } else if (movement[1] < 0.0f) {
            set_pos_y(tile.top + tile.height); //TODO: Sprawdzic czy dobra wartosc
            collision_types["top"] = true;
        }
    }
    if (collision_types["bottom"] or collision_types["top"]) { fall_vel_ = 0.0f; }
    if (collision_types["bottom"]) { can_jump_ = true; }
    if (jumping_ and can_jump_) {
        fall_vel_ = -jump_vel_;
        can_jump_ = false;
    }
    jumping_ = false;
}

bool Character::check_if_dead() {
    if (health_ <= 0 or get_pos_y() >= 500) {
        char_rect_dict.erase(this);
        //TODO: PLAY DEATH SOUND

        return true;
    } else { return false; }
}


Player::Player(float pos_x, float pos_y, float width, float height, int health, const sf::Texture& texture,
               std::vector<sf::Rect<float>> tiles, const sf::Sound& death_sound, float movement_vel, float jump_vel,
               float terminal_vel)
        : Character(pos_x, pos_y, width, height, health, std::move(tiles),
                    death_sound, movement_vel, jump_vel, terminal_vel),
          texture_(texture) {
    sprite_.setTexture(texture_);
    sprite_.setOrigin(get_width() / 2, 0); //TODO: sprawdzic czy dziala
}

void Player::draw(sf::RenderWindow& display, float camera_pos[2]) {
    sprite_.setPosition(get_pos_x() + get_width() / 2 - camera_pos[0], get_pos_y() - camera_pos[1]);

    if (get_last_movement_right()) {
        sprite_.setScale(1.0f, 1.0f);
        display.draw(sprite_);
    } else {
        sprite_.setScale(-1.0f, 1.0f);
        display.draw(sprite_);
    }
}

std::vector<Character*> Player::attack_collision_test() {
    std::vector<Character*> collisions;

    if (get_last_movement_right()) {
        attack_rect.left = get_pos_x();
    } else {
        attack_rect.left = get_pos_x() - attack_range_[0] + get_width();
    }
    attack_rect.top = get_pos_y() + (get_height() - attack_range_[1]) / 2;

    for (const auto& character : char_rect_dict) {
        if (attack_rect.intersects(character.second)) {
            collisions.emplace_back(character.first);
        }
    }

    collisions.erase(std::remove(collisions.begin(), collisions.end(), this), collisions.end());
    return collisions;
}

void Player::attack() {
    if (is_attacking_) {
        auto collisions = attack_collision_test();
        for (auto& character : collisions) {
            character->damage(attack_damage_);
        }
        set_is_attacking(false);
    }
}

void Player::respawn() {
    char_rect_dict.emplace(this, entity_rect);
    set_health(respawn_hp);
    set_pos_x(respawn_pos_[0]);
    set_pos_y(respawn_pos_[1]);
}

void Player::update() {
    if (!check_if_dead()) {
        move();
        attack();
    }
}


Enemy::Enemy(float pos_x, float pos_y, float width, float height, int health, const sf::Texture& texture,
             std::vector<sf::Rect<float>> tiles, const sf::Sound& death_sound, float movement_vel, float jump_vel,
             float terminal_vel)
        : Character(pos_x, pos_y, width, height, health, std::move(tiles),
                    death_sound, movement_vel, jump_vel, terminal_vel),
          texture_(texture) {
    sprite_.setTexture(texture_);
    sprite_.setOrigin(get_width() / 2, 0);
}

void Enemy::draw(sf::RenderWindow& display, float* camera_pos) {
    sprite_.setPosition(get_pos_x() + get_width() / 2 - camera_pos[0], get_pos_y() - camera_pos[1]);

    if (get_last_movement_right()) {
        sprite_.setScale(1.0f, 1.0f);
        display.draw(sprite_);
    } else {
        sprite_.setScale(-1.0f, 1.0f);
        display.draw(sprite_);
    }
}
