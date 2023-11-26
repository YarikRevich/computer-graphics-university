#include "validator.hpp"

Validator::Validator(args::ArgumentParser* argumentParser) {
    this->argumentParser = argumentParser;
}

void Validator::handleArgumentParser(std::exception_ptr exception) {
    try {
        std::rethrow_exception(exception);
    } catch (const args::Completion& e)
    {
        std::cout << e.what();
    }
    catch (const args::Help&)
    {
        std::cout << *argumentParser;
    }
    catch (const args::ValidationError& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << *argumentParser;
    }
}

void Validator::throwValueFlagException(std::string name) {
    std::printf("Value flag '%s' is required\n", name.c_str());
}