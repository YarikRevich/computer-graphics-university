#include "help.hpp"

Help::Help(args::ArgumentParser* argumentParser) {
    this->command = new args::HelpFlag(*argumentParser, "help", "Help menu", {'h', "help"});
}