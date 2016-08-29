#include <radix/map/XmlMapLoader.hpp>

#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <vector>

#include <radix/map/XmlHelper.hpp>
#include <radix/env/Environment.hpp>
#include <radix/core/math/Vector3f.hpp>

#include <radix/component/Transform.hpp>
#include <radix/component/MeshDrawable.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/component/SoundSource.hpp>
#include <radix/component/LightSource.hpp>
#include <radix/component/RigidBody.hpp>
#include <radix/component/Player.hpp>

#include <radix/World.hpp>
#include <radix/model/Mesh.hpp>
#include <radix/texture/Texture.hpp>
#include <radix/model/MeshLoader.hpp>
#include <radix/texture/TextureLoader.hpp>
#include <radix/material/MaterialLoader.hpp>

using namespace std;
using namespace tinyxml2;

namespace radix {

XmlMapLoader::XmlMapLoader(World &w) :
  MapLoader(w),
  rootHandle(nullptr) {
}

/**
 * Get a scene from a map file in XML format.
 */
void XmlMapLoader::load(const std::string &path) {
  XMLDocument doc;
  XMLError error = doc.LoadFile(path.c_str());

  if (error == XML_NO_ERROR) {
    XMLHandle docHandle(&doc);
    XMLElement *element = docHandle.FirstChildElement().ToElement();
    rootHandle = XMLHandle(element);

    extractMaterials();
    extractSpawn();
    extractDoor();
    extractModels();
    extractLights();
    extractWalls();
    extractAcids();
    extractTriggers();
    Util::Log(Info, "XmlMapLoader") << "Map " << path << " loaded";
  } else {
    Util::Log(Error, "XmlMapLoader") << "Failed to load map " << Environment::getDataDir()
                       << "/" << path << ".xml";
  }
}

void XmlMapLoader::extractMaterials() {
  XMLElement *matIdxElm = rootHandle.FirstChildElement("materials").ToElement();

  if (matIdxElm) {
    XMLElement *matElm = matIdxElm->FirstChildElement("material");
    if (matElm) {
      do {
        int mid = -1;
        matElm->QueryIntAttribute("id", &mid);
        if (mid == -1) {
          Util::Log(Error) << "Invalid Material ID in map.";
          continue;
        }
        std::string name = matElm->Attribute("name");
        if (name.length() > 0) {
          world.materials[mid] = MaterialLoader::getMaterial(name);
        } else {
          Util::Log(Error) << "Name is mandatory for mat tag.";
          continue;
        }
      } while ((matElm = matElm->NextSiblingElement("material")) != nullptr);
    }
  }
}

/**
 * Extract a spawn element containing its rotation and position elements
 */
void XmlMapLoader::extractSpawn() {
  XMLElement *spawnElement = rootHandle.FirstChildElement("spawn").ToElement();

  if (spawnElement) {
    Entity &start = world.entities.create();
    Transform &t = start.addComponent<Transform>();
    Vector3f position;
    XmlHelper::extractPosition(spawnElement, position);
    t.setPosition(position);
    Player &p = world.getPlayer().getComponent<Player>();
    XmlHelper::extractRotation(spawnElement, p.headAngle);
    Transform &pt = world.getPlayer().getComponent<Transform>();
    pt.setPosition(position);
  } else {
    throw std::runtime_error("No spawn position defined.");
  }
}

/**
 * Extract a light elements containing position (x, y, z) and colour (r, g, b) attributes
 */
void XmlMapLoader::extractLights() {
  Vector3f lightPos;
  Vector3f lightColor;
  float distance, energy, specular;
  XMLElement* lightElement = rootHandle.FirstChildElement("light").ToElement();

  if (lightElement){
  do {
    XmlHelper::extractPosition(lightElement, lightPos);
    XmlHelper::extractColor(lightElement, lightColor);

    lightElement->QueryFloatAttribute("distance", &distance);
    lightElement->QueryFloatAttribute("energy", &energy);
    if (lightElement->QueryFloatAttribute("specular", &specular) == XML_NO_ATTRIBUTE) {
      specular = 0;
    }

    Entity &light = world.entities.create();
    Transform &t = light.addComponent<Transform>();
    t.setPosition(lightPos);
    LightSource &ls = light.addComponent<LightSource>();
    ls.color = lightColor;
    ls.distance = distance;
    ls.energy = energy;
    ls.specular = specular;
  } while ((lightElement = lightElement->NextSiblingElement("light")) != nullptr);
  }
}

void XmlMapLoader::extractDoor() {
  XMLElement *endElement = rootHandle.FirstChildElement("end").ToElement();

  if (endElement) {
    Entity &door = world.entities.create();
    Transform &t = door.addComponent<Transform>();
    Vector3f position;
    XmlHelper::extractPosition(endElement, position);
    t.setPosition(position);
    Vector3f angles;
    XmlHelper::extractRotation(endElement, angles);
    t.setOrientation(Quaternion().fromAero(angles));
    MeshDrawable &m = door.addComponent<MeshDrawable>();
    m.material = MaterialLoader::loadFromXML("door/door");
    m.mesh = MeshLoader::getMesh("Door.obj");
  }
}

void XmlMapLoader::extractWalls() {
  XMLElement *wallBoxElement = rootHandle.FirstChildElement("wall").ToElement();

  if (wallBoxElement) {
    do {
      Entity &wall = world.entities.create();

      Transform &t = wall.addComponent<Transform>();
      Vector3f position;
      XmlHelper::extractPosition(wallBoxElement, position);
      t.setPosition(position);
      Vector3f angles;
      XmlHelper::extractRotation(wallBoxElement, angles);
      t.setOrientation(Quaternion().fromAero(angles));
      Vector3f scale;
      XmlHelper::extractScale(wallBoxElement, scale);
      t.setScale(scale);

      int mid = -1;
      wallBoxElement->QueryIntAttribute("material", &mid);
      MeshDrawable &m = wall.addComponent<MeshDrawable>();
      m.material = world.materials[mid];
      m.material.scaleU = m.material.scaleV = 2.f;
      m.mesh = MeshLoader::getPortalBox(wall);
      wall.addComponent<RigidBody>
        (0, std::make_shared<btBoxShape>(btVector3(t.getScale().x/2,
                                                   t.getScale().y/2, t.getScale().z/2)));
    } while ((wallBoxElement = wallBoxElement->NextSiblingElement("wall")) != nullptr);
  }
}

void XmlMapLoader::extractAcids() {
  XMLElement *acidElement = rootHandle.FirstChildElement("acid").ToElement();
  
  if (acidElement) {
    do {
      Entity &acid = world.entities.create();

      Transform &t = acid.addComponent<Transform>();
      Vector3f position;
      XmlHelper::extractPosition(acidElement, position);
      t.setPosition(position);
      Vector3f scale;
      XmlHelper::extractScale(acidElement, scale);
      t.setScale(scale);

      MeshDrawable &m = acid.addComponent<MeshDrawable>();
      m.material = MaterialLoader::loadFromXML("fluid/acid00");
      m.mesh = MeshLoader::getPortalBox(acid);
    } while ((acidElement = acidElement->NextSiblingElement("acid")) != nullptr);
  }
}

void XmlMapLoader::extractTriggers() {
  XMLElement *triggerElement = rootHandle.FirstChildElement("trigger").ToElement();

  if (triggerElement) {
    do {
      Entity &trigger = world.entities.create();

      Transform &t = trigger.addComponent<Transform>();
      Vector3f position;
      XmlHelper::extractPosition(triggerElement, position);
      t.setPosition(position);
      Vector3f scale;
      XmlHelper::extractScale(triggerElement, scale);
      t.setScale(scale);
      
      Trigger &tgr = trigger.addComponent<Trigger>();
      tgr.type = triggerElement->Attribute("type");
    } while ((triggerElement = triggerElement->NextSiblingElement("trigger")) != nullptr);
  }
}

void XmlMapLoader::extractModels() {
  int mid = -1;
  string mesh("none");
  XMLElement *modelElement = rootHandle.FirstChildElement("model").ToElement();
  if (modelElement){
    do {
      modelElement->QueryIntAttribute("material", &mid);
      mesh = modelElement->Attribute("mesh");

      Entity &model = world.entities.create();
      Transform &t = model.addComponent<Transform>();
      Vector3f position;
      XmlHelper::extractPosition(modelElement, position);
      t.setPosition(position);
      Vector3f angles;
      XmlHelper::extractRotation(modelElement, angles);
      t.setOrientation(Quaternion().fromAero(angles));
      MeshDrawable &m = model.addComponent<MeshDrawable>();
      m.material = world.materials[mid];
      m.mesh = MeshLoader::getMesh(mesh);
    } while ((modelElement = modelElement->NextSiblingElement("object")) != nullptr);
  }
}
} /* namespace radix */
