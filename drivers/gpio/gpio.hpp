#pragma once

#include <stdint.h>

namespace drivers::gpio {

enum class Port : uint8_t {
    A = 0U,
    B = 1U,
    C = 2U,
    D = 3U,
    E = 4U,
    F = 5U,
    G = 6U,
    H = 7U,
    I = 8U
};

enum class Level : uint8_t {
    Low = 0U,
    High = 1U
};

enum class Pull : uint8_t {
    None = 0U,
    Up = 1U,
    Down = 2U
};

enum class OutputType : uint8_t {
    PushPull = 0U,
    OpenDrain = 1U
};

struct Pin {
    Port port;
    uint8_t number;
};

class Gpio final {
public:
    explicit constexpr Gpio(Pin pin) : pin_(pin) {}

    void configure_output(Level initial_level) const;
    void configure_input(Pull pull) const;

    void configure_alternate(
        uint8_t alternate_function,
        OutputType output_type = OutputType::PushPull) const;

    void write(Level level) const;
    Level read_input() const;
    Level read_output() const;

    constexpr Pin pin() const
    {
        return pin_;
    }

private:
    Pin pin_;
};

}