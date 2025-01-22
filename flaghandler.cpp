#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <yaml-cpp/yaml.h>

/**
 * @file filehandler.cpp
 * @brief Flaghandler class has all the logic for handling flags selected by the user.
 *
 * @details
 * It checks if the input is to be read from a file.
 * It checks if the output is to be written to the console or a file.
 * It checks which session is to be loaded form history.yaml file.
 * It tunes the output according to output options chosen by the user and prints the result in the said formatting.
 *
 * @param flags A string vector containing the list of flags.
 * @param hex Converts the answer obtained into hexadecimal and prints it, if set to true.
 * @param decimal Prints the answer in decimal form if set to true
 * @param msg Prints messages like "Result = " in front of the answers, if set to true.
 * @param preciseness Prints the as many number of decimal points in the answer as it's value is.
 *    
 * @note To implement any further features in terms of flags extend this class.
 */

class Flaghandler {
private:
    std::vector<std::string> flags;
    bool hex = false;
    bool decimal = true;
    bool msg = true;
    int preciseness = 6;
    YAML::Node config;

    void parseFlags() {
        for(const auto& flag : flags) {
            if(flag == "-h" || flag == "--hex") hex = true;
            else if(flag == "-d" || flag == "--decimal") decimal = true;
            else if(flag == "-nm" || flag == "--no-message") msg = false;
            else if(flag.substr(0, 2) == "-p") {
                try {
                    preciseness = std::stoi(flag.substr(2));
                } catch(...) {
                    preciseness = 6;
                }
            }
        }
    }

    void saveToHistory(const std::string& expression, const long double& result) {
        YAML::Node history;
        try {
            history = YAML::LoadFile("history.yaml");
        } catch(...) {
            // File doesn't exist or is corrupted, start fresh
        }

        time_t now = time(0);
        std::string timestamp = ctime(&now);
        timestamp.pop_back(); // Remove newline

        YAML::Node session;
        session["timestamp"] = timestamp;
        session["expression"] = expression;
        session["result"] = result;
        
        history["sessions"].push_back(session);
        
        std::ofstream fout("history.yaml");
        fout << history;
    }

    void logError(const std::string& error) {
        std::ofstream log("calc.log", std::ios::app);
        time_t now = time(0);
        std::string timestamp = ctime(&now);
        log << timestamp << ": " << error << std::endl;
    }

public:
    Flaghandler(const std::vector<std::string>& f) : flags(f) {
        parseFlags();
        try {
            config = YAML::LoadFile("config.yaml");
        } catch(...) {
            // Use defaults if config file doesn't exist
        }
    }

    void print(const long double& result) {
        std::ostringstream oss;
        if(msg) oss << "Result = ";
        
        if(hex) {
            oss << "0x" << std::hex << std::uppercase 
                << static_cast<long long>(result);
        } else if(decimal) {
            oss << std::fixed << std::setprecision(preciseness) << result;
        }

        std::cout << oss.str() << std::endl;
        
        try {
            saveToHistory(expression, result);
        } catch(const std::exception& e) {
            logError("Failed to save to history: " + std::string(e.what()));
        }
    }

    std::string loadSession(int sessionNum) {
        try {
            YAML::Node history = YAML::LoadFile("history.yaml");
            if(history["sessions"] && sessionNum < history["sessions"].size()) {
                return history["sessions"][sessionNum]["expression"].as<std::string>();
            }
        } catch(...) {
            logError("Failed to load session " + std::to_string(sessionNum));
        }
        return "";
    }
};
