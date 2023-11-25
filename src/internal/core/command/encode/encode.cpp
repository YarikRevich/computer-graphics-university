#include "encode.hpp"

Encode::Encode(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "encode", "Encodes given media");
    // args::ValueFlag<std::string> gitdir(*command, "path", "", {"media"});
}

bool Encode::isCalled() {
    return command->Matched();
}

int Encode::handle() {
    return -1;
}