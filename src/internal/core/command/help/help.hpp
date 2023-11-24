#include <args/args.hxx>

class Help {
private:
    args::HelpFlag* help = NULL;

public:
    Help(args::ArgumentParser*);
};