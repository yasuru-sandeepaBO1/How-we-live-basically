#pragma once

#include <string>

struct RedBloodCell {
    int id{};
    double lifespanDays{120.0};
    std::string role{"Carries oxygen around the body"};
};

struct WhiteBloodCell {
    int id{};
    double lifespanDays{20.0};
    std::string role{"Supports the immune response"};
};

struct Platelet {
    int id{};
    double lifespanDays{9.0};
    std::string role{"Helps with blood clotting"};
};
