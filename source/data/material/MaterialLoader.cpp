#include <radix/data/material/MaterialLoader.hpp>

#include <tinyxml2.h>

#include <radix/data/texture/TextureLoader.hpp>
#include <radix/env/Environment.hpp>
#include <radix/env/Util.hpp>

using namespace tinyxml2;

namespace radix {

std::map<std::string, Material> MaterialLoader::materialCache = {};

const Material MaterialLoader::loadFromXML(const std::string &path) {
  std::string dir = path.substr(0, path.find_last_of("/\\"));
  XMLDocument doc;
<<<<<<< HEAD
  XMLError error = doc.LoadFile((Environment::getDataDir() + "/textures/" + path + ".rmdx").c_str());
=======
  std::string fileName = Environment::getDataDir() + "/textures/" + path + ".gmd";
  XMLError error = doc.LoadFile(fileName.c_str());
>>>>>>> f0cfe90fd45e18e11c7d0b59dbc4c2bb23ef4150

  if (error != 0) {
    Util::Log(Error, "MaterialLoader") << "XML Error " << doc.ErrorID() << ": " <<
      doc.ErrorName() << " in " << path;
  }

  XMLHandle docHandle(&doc);
  XMLElement *root = docHandle.FirstChildElement().ToElement();
  XMLHandle rootH(root);

  std::string name      = root->Attribute("name");
  std::string fancyname = root->Attribute("fancyname");

  Material mat;

  mat.name.swap(name);
  mat.fancyname.swap(fancyname);

  XMLElement *diffE = rootH.FirstChildElement("diffuse").ToElement();
  if (diffE) {
    std::string diffP = diffE->Attribute("path");
    if (diffP.length() > 0) {
      diffP = dir + "/" + diffP;
      Util::Log(Debug, "MaterialLoader") << mat.name << ": load " << diffP;
      mat.diffuse = TextureLoader::getTexture(diffP);
    }
  } else {
    mat.diffuse = TextureLoader::getEmptyDiffuse();
  }

  XMLElement *normE = rootH.FirstChildElement("normal").ToElement();
  if (normE) {
    std::string normP = normE->Attribute("path");
    if (normP.length() > 0) {
      normP = dir + "/" + normP;
      Util::Log(Debug, "MaterialLoader") << mat.name << ": load " << normP;
      mat.normal = TextureLoader::getTexture(normP);
    }
  } else {
    mat.normal = TextureLoader::getEmptyNormal();
  }

  XMLElement *specE = rootH.FirstChildElement("specular").ToElement();
  if (specE) {
    std::string specP = specE->Attribute("path");
    if (specP.length() > 0) {
      specP = dir + "/" + specP;
      Util::Log(Debug, "MaterialLoader") << mat.name << ": load " << specP;
      mat.specular = TextureLoader::getTexture(specP);
    }
    specE->QueryFloatAttribute("shininess", &mat.shininess);
  } else {
    mat.specular = TextureLoader::getEmptySpecular();
  }

  XMLElement *surfaceE = rootH.FirstChildElement("surface").ToElement();
  mat.portalable = false;
  if (surfaceE) {
    surfaceE->QueryBoolAttribute("portalable", &mat.portalable);
  }

  XMLElement *scaleE = rootH.FirstChildElement("scale").ToElement();
  if (scaleE) {
    scaleE->QueryFloatAttribute("u", &mat.scaleU);
    scaleE->QueryFloatAttribute("v", &mat.scaleV);
  }

  XMLElement *kindE = rootH.FirstChildElement("kind").ToElement();
  if (kindE) {
    mat.kind = std::string(kindE->GetText());
  }

  XMLElement *tagsE = rootH.FirstChildElement("tags").ToElement();
  if (tagsE) {
    std::string tagStr(tagsE->GetText());
    size_t start = 0;
    size_t index = tagStr.find(",", start);
    while (index != std::string::npos) {
      mat.tags.push_back(tagStr.substr(start, index - start));
      start = index + 1;
      index = tagStr.find(",", start);
    }
    if (start != std::string::npos) {
      mat.tags.push_back(tagStr.substr(start));
    }
  }

  return mat;
}

const Material &MaterialLoader::getMaterial(const std::string &name) {
  // make sure that texture is not exist in cache
  auto it = materialCache.find(name);
  if (it != materialCache.end()) {
    return it->second;
  }

  Material m = loadFromXML(name);
  auto inserted = materialCache.insert(std::make_pair(m.name, m));

  // Return reference to newly inserted Material
  return inserted.first->second;
}

const Material &MaterialLoader::fromTexture(const std::string &name) {
  // make sure that texture is not exist in cache
  std::string materialName = "rawtex/" + name;
  auto it = materialCache.find(materialName);
  if (it != materialCache.end()) {
    return it->second;
  }

  Material m;
  m.name.swap(materialName);
  m.diffuse   = TextureLoader::getTexture(name);
  m.normal    = TextureLoader::getEmptyNormal();
  m.specular  = TextureLoader::getEmptySpecular();
  m.shininess = 1;

  auto inserted = materialCache.insert(std::make_pair(m.name, m));

  // Return reference to newly inserted Material
  return inserted.first->second;
}

} /* namespace radix */
