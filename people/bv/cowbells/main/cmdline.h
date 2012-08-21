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
};

enum optionIndex { oUNKNOWN, oHELP, oOUTPUT, oGEOMETRY, oUI };
const option::Descriptor usage[] = 
{
    {oUNKNOWN, 0, "", "", Arg::None, 
     "Usage: cowbells [options]\n\nOptions:"},

    {oHELP, 0, "", "help", Arg::None, 
     "  --help\tPrint usages and exit"},

    {oOUTPUT, 0, "o","output", Arg::Required, 
     "  --output, -o <outputfile>\tSet output filename"},

    {oGEOMETRY, 0, "g", "geometry", Arg::Required,
     "  --geometry, -g <geomfile>\tSet the geometry filename"},

    {oUI, 0, "u", "interface", Arg::Required,
     "  --interface, -u <interface>\tSet the user interface"},

    {0,0,0,0,0,0}
};

option::Option* options = 0;

option::Option* parse_args(int argc, char* argv[])
{
    option::Stats stats(usage, argc-1, argv+1);
    options = new option::Option[stats.options_max];
    option::Option* buffer  = new option::Option[stats.buffer_max];
    option::Parser parse(usage, argc-1, argv+1, options, buffer);

    if (parse.error()) {
        std::cerr << "Error parsing commandline arguments" << std::endl;
        option::printUsage(std::cout, usage);
        return 0;
    }

    if (options[oHELP] || argc == 1) {
        option::printUsage(std::cout, usage);
        return 0;
    }

    if (! options[oGEOMETRY].arg) {
        std::cerr << "Must give an input geometry file" << std::endl;
        option::printUsage(std::cout, usage);
        return 0;
    }

    if (! options[oOUTPUT].arg) {
        std::cerr << "Must give an output file" << std::endl;
        option::printUsage(std::cout, usage);
        return 0;
    }

    return options;
}

const char* arg(optionIndex oi)
{
    return options[oi].arg;
}

