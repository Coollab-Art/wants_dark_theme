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

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
auto wants_dark_theme() -> std::optional<bool>
{
    // Get the "AppleInterfaceStyle" key from the user preferences
    CFStringRef pref = CFSTR("AppleInterfaceStyle");
    CFPreferencesAppSynchronize(CFSTR("com.apple.systempreferences"));
    CFPropertyListRef value = CFPreferencesCopyValue(pref, CFSTR("com.apple.systempreferences"), kCFPreferencesCurrentUser, kCFPreferencesAnyHost);

    // Check if the value exists and is "Dark"
    if (value == nullptr)
        return std::nullopt;

    CFStringRef dark_mode_str = CFSTR("Dark");
    const bool  dark_mode     = CFStringCompare((CFStringRef)value, dark_mode_str, kCFCompareCaseInsensitive) == kCFCompareEqualTo;
    CFRelease(value);

    return dark_mode;
}
#endif // __APPLE__