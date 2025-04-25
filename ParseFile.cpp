#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include "ParseFile.h"
#include <sstream>

ParsedField GetFieldFrom(const std::string& input_path) {
    ParsedField Parfield;

    std::ifstream input_file(input_path);

    if (!input_file) {
        std::cerr << "Error: could not open file"<<' '<<input_path<<' '<<'\n';
        exit(1);
    }


    std::string line;
    bool size_is_read = false;

    uint64_t ships_number = 0;

    while (std::getline(input_file, line)) {
        if (!size_is_read) {
            size_is_read = true;
            continue;
        }
        ships_number++;
    }

    size_is_read = false;

    Ship* ships = new Ship[ships_number];
    uint64_t ships_iterator = 0;

    input_file.clear();
    input_file.seekg(0, std::ios::beg);

    while (std::getline(input_file,line)) {
        std::istringstream main_line(line);

        if (!size_is_read) {
            main_line >> Parfield.width >> Parfield.height;
            size_is_read = true;
            if (main_line.fail()) {
                std::cerr << "Error: could not read fill size from" << ' ' << input_path << ' ' << '\n' <<
                    "the line: " << line;
                exit(1);
            }
            continue;
        }

        main_line >> ships[ships_iterator].length >> ships[ships_iterator].direction >>
                     ships[ships_iterator].x >> ships[ships_iterator].y;
        Parfield.ships_count[ships[ships_iterator].length-1]++;

        if (main_line.fail()) {
            std::cerr << "Error: could not read the line: " << line << '\n';
            exit(1);
        }


        for (int i = 0; i < ships[ships_iterator].length; i++) {
            ships[ships_iterator].hits[i] = true;
        }

        ships_iterator++;
    }


    // For tests, if you need to check is any ship read correctly

    // for (int i = 0; i < ships_iterator; i++) {
    //
    //     std::cout << "Ship " << i + 1 << ":\n";
    //     std::cout << "length: " << ships[i].length << '\n';
    //     std::cout << "Coordinates: (" << ships[i].x << ", " << ships[i].y << ")\n";
    //     std::cout << "Direction: " << ships[i].direction << '\n';
    //
    //     std::cout << "Hits: ";
    //     for (bool hit : ships[i].hits) {
    //         std::cout << (hit ? '1' : '0') << ' ';
    //     }
    //     std::cout << "\n\n";
    // }

    Parfield.ships = ships;


    return Parfield;
}

void SaveFieldTo(const std::string& output_path, const Field& field, const uint64_t ship_number) {
    std::ofstream output_file(output_path);
    const Ship* ships = field.GetShips();
    output_file << field.GetWidth() << ' ' << field.GetHeight() << '\n';
    for (int i = 0; i < ship_number; i++) {
        output_file << ships[i].length << ' ' << ships[i].direction << ' ' <<
                       ships[i].x << ' ' << ships[i].y << '\n';
    }
    std::cout << "File save to " << output_path << '\n';
}

bool ParsedField::IsReadyToCreateField() const {
    if (width != 0 &&
        height != 0) {
        for (int i = 0; i < 4; i++) {
            if (ships_count[i] != 0) {
                return true;
            }
        }
    }
    return false;
}

bool canPlace(const uint64_t k, const uint64_t n, const uint64_t distance) {
    uint64_t count = 1;
    uint64_t last_pos = 0;

    for (uint64_t pos = 1; pos < n; pos++) {
        if (pos - last_pos >= distance) {
            count++;
            last_pos = pos;
            if (count >= k) {
                return true;
            }
        }
    }
    return false;
}

uint64_t FindMaxDistance(const uint64_t k, const uint64_t n) {
    if (k > n) {

        return -1;
    }
    uint64_t low = 1;
    uint64_t high = n - 1;
    uint64_t result = 0;

    while (low <= high) {
        const uint64_t mid = low + (high - low) / 2;

        if (canPlace(k, n, mid)) {
            result = mid;
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return result;
}

bool PlaceShips(ParsedField& field) {
    uint64_t ships_number = 0;

    for (uint16_t i = 0; i < 4; i++) {
        ships_number += field.ships_count[i];
    }

    Ship* ships = new Ship[ships_number];
    uint64_t ships_iterator = 0;


    uint64_t remaining_ships[] = {
        field.ships_count[0],
        field.ships_count[1],
        field.ships_count[2],
        field.ships_count[3]
    };



    uint64_t current_x = 0;
    uint64_t current_y = field.height - 1;
    while (ships_number > 0) {
        if (current_x >= field.width - 1) {
            if (current_y >= 2) {
                current_x = 0;
                current_y -= 2;
            }
            else {
                return false;
            }
        }
        if (remaining_ships[3] != 0) {
            if (current_x + 3 < field.width) {
                ships[ships_iterator] = Ship{current_x, current_y, 'h', 4};
                for (int i = 0; i < ships[ships_iterator].length; i++) {
                    ships[ships_iterator].hits[i] = true;
                }
                ships_iterator++;
                ships_number--;
                remaining_ships[3]--;
                current_x += 5;
                continue;
            }
        }
        if (remaining_ships[2] != 0) {
            if (current_x + 2 < field.width) {
                ships[ships_iterator] = Ship{current_x, current_y, 'h', 3};
                for (int i = 0; i < ships[ships_iterator].length; i++) {
                    ships[ships_iterator].hits[i] = true;
                }
                ships_iterator++;
                ships_number--;
                remaining_ships[2]--;
                current_x += 4;
                continue;
            }
        }
        if (remaining_ships[1] != 0) {
            if (current_x + 1 < field.width) {
                ships[ships_iterator] = Ship{current_x, current_y, 'h', 2};
                for (int i = 0; i < ships[ships_iterator].length; i++) {
                    ships[ships_iterator].hits[i] = true;
                }
                ships_iterator++;
                ships_number--;
                remaining_ships[1]--;
                current_x += 3;
                continue;
            }
        }
        const uint64_t line_length = (current_y / 2) * field.width + (field.width - current_x);
        const uint64_t max_distance = FindMaxDistance(field.ships_count[0], line_length);

        while (remaining_ships[0] != 0) {
            if (current_x >= field.width) {
                while (current_x >= field.width && current_y >= 2) {
                    current_x -= field.width;
                    current_y -= 2;
                }
                if (current_x >= field.width) {
                    return false;
                }
            }
            ships[ships_iterator] = Ship{current_x, current_y, 'h', 1};
            for (int i = 0; i < ships[ships_iterator].length; i++) {
                ships[ships_iterator].hits[i] = true;
            }
            ships_iterator++;
            ships_number--;
            remaining_ships[0]--;
            current_x += max_distance;

        }


    }
    field.ships = ships;
    return true;
}


