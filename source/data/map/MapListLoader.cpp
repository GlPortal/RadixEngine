#include <radix/data/map/MapListLoader.hpp>
#include <radix/env/Environment.hpp>
#include <fstream>
#include <iostream>

using namespace std;

#include <ciso646>

namespace radix {

vector<string> MapListLoader::getMapList() {
  vector<string> mapList;
  std::string path = Environment::getDataDir() + "/maps/maplist";
  ifstream file(path);

  if (not file.is_open()) {
    throw runtime_error("Could not find file: " + path);
  }

  string line;

  while (std::getline(file, line)) {
    mapList.push_back(line);
  }

  return mapList;
}

} /* namespace radix */
