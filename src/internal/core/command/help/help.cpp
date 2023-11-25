#include "help.hpp"

Help::Help(args::ArgumentParser* argumentParser) {
    this->command = new args::HelpFlag(*argumentParser, "help", "Display this help menu", {'h', "help"});
}