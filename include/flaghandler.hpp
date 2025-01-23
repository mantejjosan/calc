#ifndef FLAGHANDLER_HPP
#define FLAGHANDLER_HPP
#include <vector>
#include <string>
#include <yaml-cpp/yaml.h>

class Flaghandler {
private:
    std::vector<std::string> flags;
    bool hex;
    bool decimal;
    bool msg;
    int preciseness;
    YAML::Node config;
    void parseFlags();
    void saveToHistory(const std::string& expression, const long double& result);
    void logError(const std::string& error);
public:
    Flaghandler(const std::vector<std::string>& f);
    void print(std::string& expression, const long double& result);
    std::string loadSession(int sessionNum);
};
#endif
