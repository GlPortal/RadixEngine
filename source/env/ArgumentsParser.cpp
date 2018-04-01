#include <radix/env/ArgumentsParser.hpp>

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <iostream>
#include <radix/util/SimpleOpt.h>

#include <radix/core/file/Path.hpp>
#include <radix/env/Config.hpp>
#include <radix/env/Environment.hpp>

namespace radix {
const int ArgumentsParser::NO_ARGUMENT = -1;
std::string ArgumentsParser::mapName = "";
std::string ArgumentsParser::mapPath = "";
bool ArgumentsParser::showCursor = false;
bool ArgumentsParser::ignoreGlVersion = false;
bool ArgumentsParser::debugMode = false;
bool ArgumentsParser::consoleEnabled = false;
bool ArgumentsParser::profilerEnabled = false;
int ArgumentsParser::screen = 0;


// option identifiers
enum {
     OPT_VERSION,
     OPT_HELP,
     OPT_SHOWCURSOR,
     OPT_IGNOREGLVERSION,
     OPT_DEBUGMODE,
     OPT_DATADIR,
     OPT_MAP,
     OPT_MAPFROMPATH,    
     OPT_CONSOLE,        
     OPT_PROFILER,         
     OPT_SCREEN       
};

// option array
CSimpleOpt::SOption cmdline_options[] = {
    { OPT_VERSION,          ("-version"),         SO_NONE },
    { OPT_HELP,             ("-help"),            SO_NONE },
    { OPT_SHOWCURSOR,       ("-showcursor"),      SO_NONE },
    { OPT_IGNOREGLVERSION,  ("-ignoreGlVersion"), SO_NONE },
    { OPT_DEBUGMODE,        ("-debugMode"),       SO_NONE },
    { OPT_DATADIR,          ("-datadir"),         SO_REQ_SEP },
    { OPT_MAP,              ("-map"),             SO_REQ_SEP },
    { OPT_MAPFROMPATH,      ("-mapfrompath"),     SO_REQ_SEP },
    { OPT_CONSOLE,          ("-console"),         SO_NONE },
    { OPT_PROFILER,         ("-profiler"),        SO_NONE },
    { OPT_SCREEN,           ("-screen"),          SO_REQ_SEP },
    SO_END_OF_OPTIONS
};

void ArgumentsParser::showUsage(char **argv) {
  std::cout << "Usage: " << argv[0]  << " [options]" << std::endl << std::endl;

  std::cout << "Options:" << std::endl;
  std::cout << "  -h, --help               Show this help message and exit" << std::endl;
  std::cout << "  -v, --version            Display GlPortal version" << std::endl;
  std::cout << "  -d, --datadir DIR        Set the data directory" << std::endl;
  std::cout << "  -m, --map NAME           Specify map name to load" << std::endl;
  std::cout << "  -M, --mapfrompath FILE   Load the specified map file" << std::endl;
  std::cout << "  -c, --showcursor         Forces to draw os mouse cursor" << std::endl;
  std::cout << "  -G, --ignoreGlVersion    Disable OpenGl version check" << std::endl;
  std::cout << "  -D, --debugMode          Run game in debug mode" << std::endl;
  std::cout << "  -a, --console            Run game interactively" << std::endl;
  std::cout << "  -p, --profiler           Enable profiler" << std::endl;
  std::cout << "  -s, --screen             Use alternate display" << std::endl;
}

void ArgumentsParser::setEnvironmentFromArgs (const int argc, char **argv) {

    CSimpleOpt args (argc, argv, cmdline_options);

    while (args.Next ())
    {
        if (args.LastError () != SO_SUCCESS)
        {
            showUsage (argv);
            exit (1);
        }
        switch (args.OptionId ()) {
        case OPT_VERSION:
            showVersion ();
            exit (0);
        case OPT_DATADIR:
            Environment::setDataDir (args.OptionArg ());
            break;
        case OPT_HELP:
            showUsage (argv);
            exit (0);
        case OPT_MAP:
            mapName = args.OptionArg ();
            break;
        case OPT_MAPFROMPATH:
            mapPath = args.OptionArg ();
            break;
        case OPT_SHOWCURSOR:
            showCursor = true;
            break;
        case OPT_IGNOREGLVERSION:
            ignoreGlVersion = true;
            break;
        case OPT_DEBUGMODE:
            debugMode = true;
            showCursor = true;
            ignoreGlVersion = true;
            break;
        case OPT_CONSOLE:
            consoleEnabled = true;
            break;
        case OPT_PROFILER:
            profilerEnabled = true;
        case OPT_SCREEN:
            screen = atoi (args.OptionArg ());
            break;
        default:
            showUsage (argv);
            exit (0);
            break;
        }
    }
}


void ArgumentsParser::populateConfig(radix::Config &config) {
  if ( !mapName.empty()) {
    config.map = mapName;
  }

  if ( !mapPath.empty()) {
    config.mapPath = mapPath;
  }

  if (showCursor) {
    config.cursorVisibility = showCursor;
  }

  if (ignoreGlVersion) {
    config.ignoreGlVersion = ignoreGlVersion;
  }

  if (debugMode) {
    config.width = 800;
    config.height = 600;
    config.fullscreen = false;
  }

  if (consoleEnabled) {
    config.consoleEnabled = true;
  }

  if (profilerEnabled) {
    config.profilerEnabled = true;
  }

  config.screen = screen;

}

void ArgumentsParser::showVersion(){
  std::cout << "GlPortal Version 0.1\n";
}

} /* namespace radix */
