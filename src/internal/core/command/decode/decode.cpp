#include "decode.hpp"

Decode::Decode(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "decode", "Decodes given media");
}

bool Decode::isCalled() {
    return command->Matched();
}