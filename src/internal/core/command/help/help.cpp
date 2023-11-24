#include "help.hpp"

Help::Help(args::ArgumentParser* argumentParser) {
    this->help = new args::HelpFlag(*argumentParser, "help", "Display this help menu", {'h', "help"});
}