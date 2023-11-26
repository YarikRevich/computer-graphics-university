#pragma once

#include <args/args.hxx>

class Help {
private:
    args::HelpFlag* command = NULL;

public:
    Help(args::ArgumentParser*);
};