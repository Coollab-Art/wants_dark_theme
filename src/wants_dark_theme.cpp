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

#ifdef __linux__
#include <fstream>
#include <string>
namespace Cool {
auto wants_dark_theme() -> std::optional<bool>
{
    // Read the contents of the "/etc/os-release" file to find the current linux distribution
    std::ifstream file("/etc/os-release");
    if (!file.is_open())
        return std::nullopt;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.find("ID_LIKE=") == std::string::npos)
            continue;

        // Check if we are using gnome
        if (line.find("gnome") != std::string::npos)
        {
            // Check if the "gtk-application-prefer-dark-theme" key is set to true
            std::ifstream gtk_file("~/.config/gtk-3.0/settings.ini");
            if (!gtk_file.is_open())
                return std::nullopt;
            std::string gtk_line;
            while (std::getline(gtk_file, gtk_line))
            {
                if (gtk_line.find("gtk-application-prefer-dark-theme") == std::string::npos)
                    continue;

                return gtk_line.find("=true") != std::string::npos;
            }
        }
        else
        {
            // Check if the "QT_QPA_PLATFORMTHEME" environment variable is set to "qt5ct"
            const char* qt_env = std::getenv("QT_QPA_PLATFORMTHEME"); // NOLINT (-Wdeprecated-declarations)
            if (qt_env == nullptr || std::string{qt_env} != "qt5ct")
                return std::nullopt;

            // Check if the "style" key is set to "Fusion"
            std::ifstream qt_file("~/.config/qt5ct/qt5ct.conf");
            if (!qt_file.is_open())
                return std::nullopt;
            std::string qt_line;
            while (std::getline(qt_file, qt_line))
            {
                if (qt_line.find("style=") == std::string::npos)
                    continue;
                return qt_line.find("Fusion") != std::string::npos;
            }
        }
    }
    return std::nullopt;
}
} // namespace Cool
#endif // __linux__

#ifdef __APPLE__
#include <array>
#include <cstdlib>
#include <memory>
#include <string>

namespace Cool {

static auto exec(const char* cmd) -> std::string
{
    std::array<char, 128> buffer;
    std::string           result;
    std::unique_ptr<FILE> pipe{popen(cmd, "r"), pclose};
    if (!pipe)
    {
        return "";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}
auto wants_dark_theme() -> std::optional<bool>
{
    return exec("/usr/bin/defaults read -g AppleInterfaceStyle") == "Dark\n";
}
} // namespace Cool
#endif // __APPLE__