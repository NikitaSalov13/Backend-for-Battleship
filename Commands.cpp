#include "Commands.h"

#include <cstdint>
#include <iostream>
#include <sstream>

#include "ParseFile.h"

const std::string default_setting_path = "../MasterSettings";


bool IsANumber(const std::string &str) {
    if (str.empty()) {
        return false;
    }
    for (const char c: str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

void CreateMaster() {
    ParsedField parfield = GetFieldFrom(default_setting_path);
    std::string path_to_load;

    bool need_ordered_strategy = false;
    while (true) {
        bool is_game_active = false;
        std::string cmd;
        std::getline(std::cin, cmd);

        if (cmd == "exit") {
            exit(0);
        }
        if (cmd == "stop") {
            std::cout << "ok" << '\n';
            return;
        }
        if (cmd == "ping") {
            std::cout << "pong" << std::endl;
            continue;
        }
        if (cmd == "get width") {
            std::cout << parfield.width << '\n';
            continue;
        }
        if (cmd == "get height") {
            std::cout << parfield.height << '\n';
            continue;
        }
        if (cmd == "set strategy ordered") {
            std::cout << "ok" << '\n';
            need_ordered_strategy = true;
            continue;
        }
        if (cmd == "set strategy custom") {
            std::cout << "ok" << '\n';
            need_ordered_strategy = false;
            continue;
        }
        if (cmd.find("get count") == 0) {
            const int ship_height = std::stoi(cmd.substr(10));
            std::cout << parfield.ships_count[ship_height-1] << '\n';
            continue;
        }
        if (cmd == "start") {
            is_game_active = true;
            std::cout << "ok" << '\n';
        }
        if (cmd.find("load") == 0) {
            std::istringstream line(cmd);
            std::string temp;
            line >> temp >> path_to_load;
            parfield = GetFieldFrom(path_to_load);
            continue;
        }
        if (is_game_active) {
            bool is_program_turn = false;
            Field battle_field(parfield);
            if (need_ordered_strategy) {
                OrderedStrategy program_shots_ordered(parfield);
                Game(program_shots_ordered, battle_field, is_program_turn);
                return;
            }
            CustomStrategy program_shots_custom(parfield);
            Game(program_shots_custom, battle_field, is_program_turn);
            return;
        }
    }
}

void CreateSlave() {
    ParsedField parfield;
    std::string path_to_load;

    while (true) {
        bool is_game_active = false;
        bool need_ordered_strategy = false;

        std::string cmd;
        std::getline(std::cin, cmd);

        if (cmd == "exit") {
            exit(0);
        }

        if (cmd == "stop") {
            std::cout << "ok" << '\n';
            return;
        }

        if (cmd == "ping") {
            std::cout << "pong" << std::endl;
        } else if (cmd.find("set width") == 0 && cmd.size() != 9 && IsANumber(cmd.substr(10))) {
            std::istringstream line(cmd);
            std::string temp;
            line >> temp >> temp >> parfield.width;
            if (line.fail()) {
                std::cout << "failed" << '\n';
                continue;
            }
            std::cout << "ok" << '\n';
        } else if (cmd.find("set height") == 0 && cmd.size() != 10 && IsANumber(cmd.substr(11))) {
            std::istringstream line(cmd);
            std::string temp;
            line >> temp >> temp >> parfield.height;
            if (line.fail()) {
                std::cout << "failed" << '\n';
                continue;
            }
            std::cout << "ok" << '\n';
        } else if (cmd.find("set count") == 0) {
            std::istringstream line(cmd);
            std::string temp;
            uint16_t ship_length;
            uint64_t count;

            line >> temp >> temp;
            line >> ship_length >> count;

            parfield.ships_count[ship_length - 1] = count;
            std::cout << "ok" << '\n';
        } else if (cmd == "set strategy ordered") {
            std::cout << "ok" << '\n';
            need_ordered_strategy = true;
        } else if (cmd == "set strategy custom") {
            std::cout << "ok" << '\n';
            need_ordered_strategy = false;
        }
        else if (cmd.find("load") == 0) {
            std::istringstream line(cmd);
            std::string temp;
            line >> temp >> path_to_load;
            parfield = GetFieldFrom(path_to_load);
            continue;
        }


        if (cmd == "start") {
            if (path_to_load.empty()) {
                if (PlaceShips(parfield)) {
                    is_game_active = true;
                    std::cout << "ok" << '\n';
                }
            } else {
                is_game_active = true;
                std::cout << "ok" << '\n';
            }
        }

        if (is_game_active) {
            bool is_program_turn = true;
            Field battle_field(parfield);
            if (need_ordered_strategy) {
                OrderedStrategy program_shots_ordered(parfield);
                Game(program_shots_ordered, battle_field, is_program_turn);
                return;
            }
            CustomStrategy program_shots_custom(parfield);
            Game(program_shots_custom, battle_field, is_program_turn);
            return;
        }
    }
}

void Game(Strategy &program_shots, Field &battle_field, bool &is_program_turn) {
    bool is_program_won = false;
    bool is_game_active = true;
    uint64_t users_x;
    uint64_t users_y;
    std::string cmd;


    while (is_game_active) {
        std::getline(std::cin, cmd);
        if (cmd == "exit") {
            exit(0);
        }
        if (cmd == "finished") {
            std::cout << "no" << '\n';
            continue;
        }
        if (cmd == "stop") {
            std::cout << "ok" << '\n';
            return;
        }
        if (cmd.find("dump") == 0) {
            std::istringstream line(cmd);
            std::string temp;
            std::string path;
            line >> temp >> path;
            SaveFieldTo(path, battle_field, battle_field.GetShipNumber());
        }
        if (cmd.substr(0, 4) == "shot" && cmd.size() > 5 && !is_program_turn) {
            std::istringstream coords(cmd.substr(5));
            coords >> users_x >> users_y;
            if (coords.fail()) {
                continue;
            }
            const uint16_t result = battle_field.Shot(users_x, users_y);
            if (result == 0) {
                std::cout << "miss" << '\n';
            } else if (result == 1) {
                std::cout << "hit" << '\n';
                continue;
            } else {
                std::cout << "kill" << '\n';
                if (battle_field.IsGameFinished() == true) {
                    is_game_active = false;
                }
                continue;
            }
            is_program_turn = true;
            continue;
        }
        if (is_program_turn) {
            if (cmd == "shot") {
                program_shots.Shot();
                if (program_shots.IsGameFinished()) {
                    is_game_active = false;
                    is_program_won = true;
                }
            }
            if (cmd == "set result miss") {
                std::cout << "ok" << '\n';
                is_program_turn = false;
            } else if (cmd == "set result hit") {
                std::cout << "ok" << '\n';
            } else if (cmd == "set result kill") {
                std::cout << "ok" << '\n';
                program_shots.ReduceAliveShipsCount();
                if (program_shots.UserShipsAlive() == 0) {
                    is_game_active = false;
                    is_program_won = true;
                }
            }
        }
    }
    while (true) {
        std::getline(std::cin, cmd);
        if (cmd == "exit") {
            exit(0);
        }
        if (cmd == "finished") {
            std::cout << "yes" << '\n';
        }
        if (cmd == "win") {
            if (is_program_won) {
                std::cout << "yes" << '\n';
            } else {
                std::cout << "no" << '\n';
            }
        }
        if (cmd == "lose") {
            if (is_program_won) {
                std::cout << "no" << '\n';
            } else {
                std::cout << "yes" << '\n';
            }
        }
    }
}
