#pragma once

#include <string>

/**
 * Represents common logger for the application(besides the ones in external libs).
*/
class Logger {
private:
    static std::string message;
public:
    /**
     * Checks if latest logger message exist.
     * @return result of logger message existance check.
    */
    static bool IsErrorExist();

    /**
     * Retrieves latest logger message.
     * @return latest logger message.
    */
    static std::string GetError();

    /**
     * Sets latest logger message.
     * @param message - latest logger message.
    */
    static void SetError(std::string message);
};