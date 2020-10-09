#include <radix/data/map/MapListLoader.hpp>
#include <radix/env/LegacyEnvironment.hpp>
#include <fstream>
#include <iostream>

using namespace std;

namespace radix {

vector<string> MapListLoader::getMapList() {
  vector<string> mapList;
  std::string path = LegacyEnvironment::getDataDir() + "/maps/maplist";
  ifstream file(path);

  if (!file.is_open()) {
    throw runtime_error("Could not find file: " + path);
  }

  string line;

  while (std::getline(file, line)) {
    mapList.push_back(line);
  }

  return mapList;
}

} /* namespace radix */
