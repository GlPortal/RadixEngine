#include <radix/environment/Arguments.hpp>

namespace radix {
  Arguments::Arguments(int argumentCount, char **argumentVector) {
    executableName = argumentVector[0];
    std::cout << "Have " << argumentCount << " arguments:" << std::endl;
    for (int i = 0; i < argumentCount; ++i) {
        std::cout << argumentVector[i] << std::endl;
    }
    exit(0);

    cxxopts::Options options("Radix", "A free game engine");
        options.add_options()
      ("d, datadir", "Set Data Dir", cxxopts::value<std::string>());
        //          auto result = options.parse(argumentCount, const_cast<char**>(argumentVector));
        auto result = options.parse(argumentCount, argumentVector);
     try {
       executableName = result["d"].as<std::string>();
     } catch(const cxxopts::OptionException& exception) {
       std::cout << "Argument Parse Error: " << exception.what() << std::endl;
     }
  }

  std::string Arguments::getExecutableName(){
    return executableName;
  }

  std::string Arguments::getDataPath(){
    return dataPath;
  }
} /* namespace radix */
