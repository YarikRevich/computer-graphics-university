#include "validator.hpp"

Validator::Validator(args::ArgumentParser* argumentParser) {
    this->argumentParser = argumentParser;
}

int Validator::handleArgumentParser(std::exception_ptr exception) {
    try {
        std::rethrow_exception(exception);
    } catch (const args::Completion& e)
    {
        std::cout << e.what();
        return EXIT_FAILURE;
    }
    catch (const args::Help&)
    {
        std::cout << *argumentParser;
        return EXIT_FAILURE;
    }
    catch (const args::ValidationError& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << *argumentParser;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void Validator::throwValueFlagRequiredException(std::string name) {
    std::printf("Flag '%s' is required\n", name.c_str());
}

void Validator::throwValueFlagInvalidException(std::string name) {
    std::printf("Value of the flag '%s' is invalid\n", name.c_str());
}