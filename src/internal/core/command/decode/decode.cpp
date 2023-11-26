#include "decode.hpp"

Decode::Decode(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "decode", "Decodes given media");
    new args::Group(command, "Decoder");
}

bool Decode::isCalled() {
    return command->Matched();
}

int Decode::handle() {
    return -1;
};