#include "decode.hpp"
#include <iostream>

Decode::Decode(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "decode", "Decodes given media from a certain type");
    args::Group* group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
    this->type = new args::ValueFlag<std::string>(*group, "bmp|jpg|jpeg|png", "Type of the output media", {"type"});
    this->debug = new args::Flag(*group, "true|false(default)", "Enables debug mode", {"debug"});;
    this->to = new args::ValueFlag<std::string>(*group, "path", "Path to the output media", {"to"});
}

bool Decode::isCalled() {
    return command->Matched();
}

int Decode::handle() {
    if (!from->Matched()){
        Validator::throwValueFlagRequiredException("from");
        return EXIT_FAILURE;
    }

    if (!type->Matched()){
        Validator::throwValueFlagRequiredException("type");
        return EXIT_FAILURE;
    }

    if (!to->Matched()){
        Validator::throwValueFlagRequiredException("to");
        return EXIT_FAILURE;
    }

    IO::FILE_TYPES fileType = IO::getFileType(type->Get());
    if (fileType == IO::FILE_TYPES::NONE) {
        Validator::throwValueFlagInvalidException("type");
        return EXIT_FAILURE;
    }

    std::ifstream inputStream(from->Get(), std::ios_base::binary);
    if (!inputStream.is_open()) {
        Validator::throwValueFlagInvalidException("from");
        return EXIT_FAILURE;
    }

    int result = Pipeline::handleDecode(inputStream, debug->Get(), fileType, to->Get());

    inputStream.close();

    if (result != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};