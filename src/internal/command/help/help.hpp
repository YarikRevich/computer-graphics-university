#pragma once

#include <args/args.hxx>

/**
 * Represents built-in help command.
*/
class Help {
private:
    args::HelpFlag* command = NULL;

public:
    Help(args::ArgumentParser*);
};