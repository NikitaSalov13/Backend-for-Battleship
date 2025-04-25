#pragma once
#include <string>

#include "Field.h"


struct Ship {
    uint64_t x{};
    uint64_t y{};
    char direction{};
    uint16_t length{};
    bool hits[4]{};
    bool is_killed = false;

    Ship() = default;

    Ship(const uint64_t x, const uint64_t y, const char direction, const uint16_t length)
        :  x(x)
        ,  y(y)
        ,  direction(direction)
        ,  length(length) {
    }
};

struct ParsedField {
    Ship* ships = nullptr;
    uint64_t width{};
    uint64_t height{};
    uint64_t ships_count[4]{};

    bool IsReadyToCreateField() const;

};



ParsedField GetFieldFrom(const std::string& input_path);
bool PlaceShips(ParsedField& field);
void SaveFieldTo(const std::string& output_path, const Field& field, const uint64_t ship_number);