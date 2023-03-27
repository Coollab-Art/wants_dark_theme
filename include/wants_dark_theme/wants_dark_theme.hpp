#pragma once
#include <optional>

namespace Cool {

/// Returns a boolean indicating if the OS preferences are set to a Dark theme (by opposition to a Light theme).
/// Or returns `std::nullopt` if we were not able to read the OS preferences.
auto wants_dark_theme() -> std::optional<bool>;

} // namespace Cool
