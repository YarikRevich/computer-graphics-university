#include <args/args.hxx>

class Encode {
private:
    args::Command* command = NULL;
    // args::ArgumentParser command("This is a git-like program", "Valid commands are init and add");

public:
    Encode(args::ArgumentParser*);

    bool isCalled();
};