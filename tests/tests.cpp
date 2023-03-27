#include <iostream>
#include <wants_dark_theme/wants_dark_theme.hpp>

auto main() -> int
{
    auto const wants_dark_theme = Cool::wants_dark_theme();
    if (!wants_dark_theme.has_value())
        std::cout << "Failed to read preferences.\n";
    else if (*wants_dark_theme)
        std::cout << "Wants DARK theme.\n";
    else
        std::cout << "Wants LIGHT theme.\n";
}