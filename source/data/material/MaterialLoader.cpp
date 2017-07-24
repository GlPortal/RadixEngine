#include <radix/data/material/MaterialLoader.hpp>

#include <tinyxml2.h>

#include <radix/data/texture/TextureLoader.hpp>
#include <radix/env/Environment.hpp>
#include <radix/env/Util.hpp>

using namespace tinyxml2;

namespace radix {

std::map<std::string, Material> MaterialLoader::materialCache = {};

constexpr const char *MaterialLoader::typeToString(const MaterialLoader::materialAttrib _type) {
  switch (_type) {
    case materialAttrib::diffuse:
      return "diffuse";
    case materialAttrib::specular:
      return "specular";
    case materialAttrib::normal:
      return "normal";
    case materialAttrib::surface:
      return "surface";
    case materialAttrib::scale:
      return "scale";
    case materialAttrib::kind:
      return "kind";
    case materialAttrib::tags:
      return "tags";
  }

  return "";
}

template <MaterialLoader::materialAttrib _type>
bool MaterialLoader::loadAttrib(XMLHandle *root, const std::string &dir,
                                Material &mat) {
  const auto element = root->FirstChildElement(typeToString(_type)).ToElement();

  if (element) {
    std::string elementPath = element->Attribute("path");

    if (!elementPath.empty()) {
      elementPath = dir + "/" + elementPath;

      Util::Log(Debug, "MaterialLoader") << mat.name << ": load "
                                         << elementPath;

      switch (_type) {
        case materialAttrib::diffuse:
          mat.diffuse = TextureLoader::getTexture(elementPath);
          break;
        case materialAttrib::normal:
          mat.normal = TextureLoader::getTexture(elementPath);
          break;
        case materialAttrib::specular:
          mat.specular = TextureLoader::getTexture(elementPath);
          element->QueryFloatAttribute("shininess", &mat.shininess);
          break;
      }
      return true;
    }
  }

  switch (_type) {
    case materialAttrib::diffuse:
      mat.diffuse = TextureLoader::getEmptyDiffuse();
      break;
    case materialAttrib::normal:
      mat.normal = TextureLoader::getEmptyNormal();
      break;
    case materialAttrib::specular:
      mat.specular = TextureLoader::getEmptySpecular();
      break;
  }
  return false;
}

template <>
bool MaterialLoader::loadAttrib<MaterialLoader::materialAttrib::surface>(
    XMLHandle *root, const std::string &, Material &mat) {
  XMLElement *element = root->FirstChildElement("surface").ToElement();
  mat.portalable = false;
  if (element) {
    element->QueryBoolAttribute("portalable", &mat.portalable);
    return true;
  }
  return false;
}

template <>
bool MaterialLoader::loadAttrib<MaterialLoader::materialAttrib::scale>(
    XMLHandle *root, const std::string &, Material &mat) {
  XMLElement *scaleE = root->FirstChildElement("scale").ToElement();
  if (scaleE) {
    scaleE->QueryFloatAttribute("u", &mat.scaleU);
    scaleE->QueryFloatAttribute("v", &mat.scaleV);
    return true;
  }
  return false;
}

template <>
bool MaterialLoader::loadAttrib<MaterialLoader::materialAttrib::kind>(
    XMLHandle *root, const std::string &, Material &mat) {
  XMLElement *kindE = root->FirstChildElement("kind").ToElement();
  if (kindE) {
    mat.kind = std::string(kindE->GetText());
    return true;
  }
  return false;
}

template <>
bool MaterialLoader::loadAttrib<MaterialLoader::materialAttrib::tags>(
    XMLHandle *root, const std::string &, Material &mat) {
  const XMLElement *tagsE = root->FirstChildElement("tags").ToElement();
  if (tagsE) {
    const std::string tagStr(tagsE->GetText());
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
    return true;
  }
  return false;
}

const Material MaterialLoader::loadFromXML(const std::string &path) {
  const std::string dir = path.substr(0, path.find_last_of("/\\"));
  XMLDocument doc;

  const auto xmlPath = Environment::getDataDir() + "/textures/" + path + ".gmd";
  const XMLError error = doc.LoadFile(xmlPath.c_str());

  if (error != 0) {
    Util::Log(Error, "MaterialLoader") << "XML Error " << doc.ErrorID() << ": "
                                       << doc.ErrorName() << " in " << path;
  }

  XMLHandle docHandle(&doc);
  XMLElement *root = docHandle.FirstChildElement().ToElement();
  XMLHandle rootH(root);

  std::string name = root->Attribute("name");
  std::string fancyname = root->Attribute("fancyname");

  Material mat;

  mat.name = name;
  mat.fancyname = fancyname;

  loadAttrib<materialAttrib::diffuse>(&rootH, dir, mat);

  loadAttrib<materialAttrib::normal>(&rootH, dir, mat);

  loadAttrib<materialAttrib::specular>(&rootH, dir, mat);

  loadAttrib<materialAttrib::surface>(&rootH, dir, mat);

  loadAttrib<materialAttrib::scale>(&rootH, dir, mat);

  loadAttrib<materialAttrib::kind>(&rootH, dir, mat);

  loadAttrib<materialAttrib::tags>(&rootH, dir, mat);

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
  m.diffuse = TextureLoader::getTexture(name);
  m.normal = TextureLoader::getEmptyNormal();
  m.specular = TextureLoader::getEmptySpecular();
  m.shininess = 1;

  auto inserted = materialCache.insert(std::make_pair(m.name, m));

  // Return reference to newly inserted Material
  return inserted.first->second;
}

} /* namespace radix */
