#include <optional>
#include <wants_dark_theme/wants_dark_theme.hpp>

#ifdef _WIN32
#include <windows.h>
namespace Cool {
auto wants_dark_theme() -> std::optional<bool>
{
    // Open the Registry key
    HKEY key;                                                                                                                                     // NOLINT
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &key) != ERROR_SUCCESS) // NOLINT
        return std::nullopt;

    auto  dark_mode  = std::optional<bool>{std::nullopt};
    DWORD value      = 0;
    DWORD value_size = sizeof(DWORD);
    if (RegQueryValueEx(key, "AppsUseLightTheme", nullptr, nullptr, (LPBYTE)&value, &value_size) == ERROR_SUCCESS) // NOLINT
        dark_mode.emplace(value == 0);
    RegCloseKey(key);

    return dark_mode;
}
} // namespace Cool
#endif // _WIN32