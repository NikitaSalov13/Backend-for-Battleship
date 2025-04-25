#pragma once
#include <cstdint>

struct Ship;
struct ParsedField;


class Field {
private:
    uint64_t width;
    uint64_t height;
    Ship* ships;
    uint64_t ships_number;
    uint64_t alive_ships_number;
    uint16_t ships_count[4]{};


public:
    Field();
    explicit Field(const ParsedField& parsed_field);
    ~Field();

    bool AllSettingsSet() const;
    uint64_t GetWidth() const;
    uint64_t GetHeight() const;
    uint64_t GetShipNumber() const;
    Ship* GetShips() const;
    // 0 = miss; 1 = hit; 2 = kill;
    int Shot(uint64_t x, uint64_t y);
    void SetShipCount(uint16_t ship_height, uint16_t count);
    uint16_t GetShipCount(uint16_t ship_height) const;
    bool IsGameFinished() const;
    void Clear();

};

class Strategy {
protected:
    virtual ~Strategy() = default;
    explicit Strategy(const ParsedField& field);

    uint64_t last_shot_x;
    uint64_t last_shot_y;
    uint64_t width;
    uint64_t height;
    uint64_t user_ships_alive;
    bool is_game_finished;

public:
    virtual void Shot() = 0;
    bool IsGameFinished() const;
    void ReduceAliveShipsCount();
    uint64_t UserShipsAlive() const;
};

class OrderedStrategy final : public Strategy {
public:
    explicit OrderedStrategy(const ParsedField& field);
    void Shot() override;
};

class CustomStrategy final : public Strategy {
public:
    explicit CustomStrategy(const ParsedField& field);
    void Shot() override;
};
