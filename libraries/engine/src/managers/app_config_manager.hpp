#ifndef APP_CONFIG_MANAGER_HPP
#define APP_CONFIG_MANAGER_HPP

#include "../gui/app_settings.hpp"

#include <string>

struct AppConfig
{
    std::string version;
    DefaultCommunicationMode defaultCommunication = DefaultCommunicationMode::ASK;
    ThemeMode theme = ThemeMode::LIGHT;
    LanguageMode language = LanguageMode::ENGLISH;
};

class AppConfigManager
{
public:
    static bool load(AppConfig &config, std::string &error);
    static bool save(const AppConfig &config, std::string &error);

    static const char *toString(DefaultCommunicationMode mode);
    static const char *toString(ThemeMode mode);
    static const char *toString(LanguageMode mode);

    static DefaultCommunicationMode communicationFromString(const std::string &value);
    static ThemeMode themeFromString(const std::string &value);
    static LanguageMode languageFromString(const std::string &value);
};

#endif // APP_CONFIG_MANAGER_HPP
