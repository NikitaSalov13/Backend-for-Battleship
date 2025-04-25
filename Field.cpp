#include "ParseFile.h"
#include "Field.h"
#include <iostream>

Field::Field()
    : width(0)
    ,  height(0)
    ,  ships(nullptr)
    , ships_number(0)
    , alive_ships_number(0) {}

Field::Field(const ParsedField& parsed_field)
    : width(parsed_field.width)
    ,  height(parsed_field.height)
    ,  ships(parsed_field.ships)
    ,  ships_number(0)
    ,  alive_ships_number(0) {
    for (int i = 0; i < 4; ++i) {
        ships_count[i] = parsed_field.ships_count[i];
    }

    for (unsigned short i : ships_count) {
        ships_number += i;
    }
    alive_ships_number = ships_number;
}

Field::~Field() {
    delete[] ships;
}

bool Field::AllSettingsSet() const {
    if (width == 0 || height == 0 || ships == nullptr) {
        return false;
    }
    return true;
}

uint64_t Field::GetWidth() const {
    return width;
}

uint64_t Field::GetHeight() const {
    return height;
}

Ship *Field::GetShips() const {
    return ships;
}

uint64_t Field::GetShipNumber() const {
    return ships_number;
}


bool IsKilled(const Ship* ship) {
    for (int i = 0; i < 4; i++) {
        if (ship->hits[i] == true) {
            return false;
        }
    }
    return true;
}

int Field::Shot(const uint64_t x, const uint64_t y) {
    for (int i = 0; i < ships_number; ++i) {
        Ship* current_ship = &ships[i];
        if (current_ship->is_killed) {
            continue;
        }

        if (current_ship->direction == 'v') {
            if (x == current_ship->x && current_ship->y <= y && y <= current_ship->y + current_ship->length - 1) {
                const auto hit_index = static_cast<uint16_t>(y - current_ship->y);
                current_ship->hits[hit_index] = false;
                if (IsKilled(current_ship)) {
                    current_ship->is_killed = true;
                    alive_ships_number--;
                    return 2;
                }
                return 1;

            }
        } else {
            if (current_ship->y == y && current_ship->x <= x && x <= current_ship->x + current_ship->length - 1) {
                const auto hit_index = static_cast<uint16_t>(x - current_ship->x);
                current_ship->hits[hit_index] = false;
                if (IsKilled(current_ship)) {
                    current_ship->is_killed = true;
                    alive_ships_number--;
                    return 2;
                }
                return 1;

            }
        }
    }
    return 0;
}


void Field::SetShipCount(const uint16_t ship_height, const uint16_t count) {
    if (ship_height < 1 || ship_height > 4) {
        return;
    }

    ships_count[ship_height - 1] = count;
}

uint16_t Field::GetShipCount(const uint16_t ship_height) const {
    if (ship_height < 1 || ship_height > 4) {
        return 0;
    }
    return ships_count[ship_height - 1];
}

bool Field::IsGameFinished() const {
    if (alive_ships_number==0) {
        return true;
    }
    return false;
}

void Field::Clear() {
    delete[] ships;
    width = 0;
    height = 0;
    ships_number = 0;
    alive_ships_number = 0;
    for (int i = 0; i < 4; i++) {

    }
}

Strategy::Strategy(const ParsedField& field)
    : last_shot_x(0)
    , last_shot_y(0)
    , width(field.width)
    , height(field.height)
    , user_ships_alive(0)
    , is_game_finished(false) {

    for (int i = 0; i < 4; ++i) {
        user_ships_alive += field.ships_count[i];
    }
}

OrderedStrategy::OrderedStrategy(const ParsedField& field)
    :
    Strategy(field) {}

CustomStrategy::CustomStrategy(const ParsedField& field)
    :
    Strategy(field) {}

void OrderedStrategy::Shot() {
    if (last_shot_x < width - 1) {
        last_shot_x++;
        std::cout<<last_shot_x<<' '<<last_shot_y;

    }
    else if (last_shot_y < height - 1) {
        last_shot_y++;
        last_shot_x = 0;
        std::cout<<last_shot_x<<' '<<last_shot_y;
    } else {
        is_game_finished = true;
    }
}

void CustomStrategy::Shot() {
    if (last_shot_x < width - 1) {
        last_shot_x++;
        std::cout<<last_shot_x<<' '<<last_shot_y;

    }
    else if (last_shot_y < height - 1) {
        last_shot_y++;
        last_shot_x = 0;
        std::cout<<last_shot_x<<' '<<last_shot_y;
    } else {
        is_game_finished = true;
    }
}

bool Strategy::IsGameFinished() const {
    if (is_game_finished) {
        return true;
    }
    return false;
}

void Strategy::ReduceAliveShipsCount() {
    user_ships_alive--;
}

uint64_t Strategy::UserShipsAlive() const {
    return user_ships_alive;
}

