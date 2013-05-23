/**
 *  command line parsing
 */ 


// http://optionparser.sourceforge.net/
#include "optionparser.h"

struct Arg: public option::Arg
{
    static option::ArgStatus Required(const option::Option& option, bool) {
        return option.arg == 0 ? option::ARG_ILLEGAL : option::ARG_OK;
    }
    static option::ArgStatus Empty(const option::Option& option, bool) {
        return (option.arg == 0 || option.arg[0] == 0) ? option::ARG_OK : option::ARG_IGNORE;
    }

    static option::ArgStatus Numeric(const option::Option& option, bool) {
        char* endptr = 0;
        if (option.arg != 0 && strtol(option.arg, &endptr, 10)){};
        if (endptr != option.arg && *endptr == 0) {
            return option::ARG_OK;
        }
        return option::ARG_ILLEGAL;
    }


};

enum optionIndex { oUNKNOWN, oHELP, oOUTPUT, oMODULES, oUI, oKIN, oPHYS, oNEVENTS, oSEED };
const option::Descriptor usage[] = 
{
    {oUNKNOWN, 0, "", "", Arg::None, 
     "Usage: cowbells [options]\n\nOptions:"},

    {oHELP, 0, "", "help", Arg::None, 
     "  --help\tPrint usages and exit"},

    {oOUTPUT, 0, "o","output", Arg::Required, 
     "  --output, -o <outputfile>\tSet output filename"},

    {oMODULES, 0, "m","modules", Arg::Required, 
     "  --modulies, -m <modules>\tSet output modules as comma separated list"},

    {oUI, 0, "u", "interface", Arg::Required,
     "  --interface, -u <interface>\tSet the user interface"},

    {oKIN, 0, "k", "kinematics", Arg::Required,
     "  --kinematics, -k <kindesc>\tSet the kinematics descriptor"},

    {oPHYS, 0, "p", "physics", Arg::Required,
     "  --physics, -p <physics,list>\tSet the physics list"},

    {oNEVENTS, 0, "n", "nevents", Arg::Required,
     "  --nevents, -n <#events>\tSet the number of events to generate"},

    {oSEED, 0, "s", "seed", Arg::Required,
     "  --seed, -s <seed>\tSeed the random number generator"},

    {0,0,0,0,0,0}
};

option::Option* options = 0;
option::Parser* parser = 0;

option::Option* parse_args(int argc, char* argv[])
{
    option::Stats stats(usage, argc-1, argv+1);
    options = new option::Option[stats.options_max];
    option::Option* buffer  = new option::Option[stats.buffer_max];
    parser = new option::Parser(usage, argc-1, argv+1, options, buffer);

    option::Parser& parse = *parser;
    if (parse.error()) {
        std::cerr << "Error parsing commandline arguments" << std::endl;
        option::printUsage(std::cout, usage);
        return 0;
    }

    if (options[oHELP] || argc == 1) {
        option::printUsage(std::cout, usage);
        return 0;
    }

    optionIndex required[] = { oOUTPUT, oUNKNOWN };
    for (int ind=0; required[ind]; ++ind) {
        optionIndex oi = required[ind];
        option::Option& opt = options[oi];
        if (! opt.arg) {
            std::cerr << "Option \"" 
                      << usage[oi].longopt
                      << "\" is required" << std::endl;
            option::printUsage(std::cout, usage);
            return 0;
        }
    }

    return options;
}

const char* opt(optionIndex oi)
{
    return options[oi].arg;
}

int nargs()
{
    return parser->nonOptionsCount();
}

const char* arg(int ind)
{
    if (ind<0 || ind >+ nargs()) { return 0; }
    return parser->nonOption(ind);
}
