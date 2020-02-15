#include <ncurses.h>
#include <string.h>
#include "parser.h"
#include "application.h"
#include "args.hxx"
#include <stdio.h>
/* #include <io.h> */


int main(int argc, char* argv[])
{
    bool isInteractive = true;
    if (isatty(fileno(stdin)))
        isInteractive = true;
    else
        isInteractive = false;
    args::ArgumentParser parser("This is a test program.", "This goes after the options.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::ValueFlag<int> timewindow(parser, "timeWindow", "The size of the window for stats", {'t'});
    /* args::ValueFlagList<std::string> characters(parser, "characters", "The character flag", {'c'}); */
    args::ValueFlag<std::string> filename(parser, "filename", "The name of the parsed file", {'f'});
    /* args::PositionalList<double> numbers(parser, "numbers", "The numbers position list"); */
    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (args::ValidationError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    /* if (timeWindow) { std::cout << "t: " << args::get(timeWindow) << std::endl; } */
    /* if (characters) { for (const auto ch: args::get(characters)) { std::cout << "c: " << ch << std::endl; } } */
    /* if (filename) { std::cout << "f: " << args::get(filename) << std::endl; } */
    /* if (numbers) { for (const auto nm: args::get(numbers)) { std::cout << "n: " << nm << std::endl; } } */
    std::string file = "";
    if (filename) { file = args::get(filename); }
    if (timewindow) {
        Application app(file, args::get(timewindow));
        app.run();
    } else {
        Application app(file);
        app.run();
    }
    return(0);
}
