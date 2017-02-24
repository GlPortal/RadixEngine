#include <radix/data/map/XmlMapLoader.hpp>

#include <iostream>
#include <vector>

#include <radix/data/map/XmlHelper.hpp>
#include <radix/data/map/XmlTriggerHelper.hpp>
#include <radix/env/Environment.hpp>

#include <radix/component/Transform.hpp>
#include <radix/component/MeshDrawable.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/component/LightSource.hpp>
#include <radix/component/RigidBody.hpp>
#include <radix/component/Player.hpp>

#include <radix/World.hpp>
#include <radix/data/model/MeshLoader.hpp>
#include <radix/data/material/MaterialLoader.hpp>

using namespace std;

namespace radix {

XmlMapLoader::XmlMapLoader(World &w) :
  MapLoader(w),
  rootHandle(nullptr) {
}

void XmlMapLoader::load(const std::string &path) {
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError error = doc.LoadFile(path.c_str());

  if (error == 0) {
    tinyxml2::XMLHandle docHandle(&doc);
    tinyxml2::XMLElement *element = docHandle.FirstChildElement().ToElement();
    rootHandle = tinyxml2::XMLHandle(element);

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
  tinyxml2::XMLElement *matIdxElm = rootHandle.FirstChildElement("materials").ToElement();

  if (matIdxElm) {
    tinyxml2::XMLElement *matElm = matIdxElm->FirstChildElement("material");
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
          Util::Log(Error) << "Name is mandatory for material tag.";
          continue;
        }
      } while ((matElm = matElm->NextSiblingElement("material")) != nullptr);
    }
  }
}

void XmlMapLoader::extractSpawn() {tinyxml2::XMLElement *spawnElement = rootHandle.FirstChildElement("spawn").ToElement();

  if (spawnElement) {
    Entity &start = world.entityManager.create();
    Transform &transform = start.addComponent<Transform>();
    Vector3f position;
    XmlHelper::extractPosition(spawnElement, position);
    transform.setPosition(position);
    Player &player = world.getPlayer().getComponent<Player>();
    XmlHelper::extractRotation(spawnElement, player.headAngle);
    Transform &playerTransform = world.getPlayer().getComponent<Transform>();
    playerTransform.setPosition(position);
  } else {
    throw std::runtime_error("No spawn position defined.");
  }
}

/**
 * Extract light element containing position (x, y, z) and colour (r, g, b) attributes
 */
void XmlMapLoader::extractLights() {
  Vector3f lightPos;
  Vector3f lightColor;
  float distance, energy, specular;
  tinyxml2::XMLElement* lightElement = rootHandle.FirstChildElement("light").ToElement();

  if (lightElement){
    do {
      XmlHelper::extractPosition(lightElement, lightPos);
      XmlHelper::extractColor(lightElement, lightColor);

      lightElement->QueryFloatAttribute("distance", &distance);
      lightElement->QueryFloatAttribute("energy", &energy);
      if (lightElement->QueryFloatAttribute("specular", &specular) == tinyxml2::XML_NO_ATTRIBUTE) {
        specular = 0;
      }

      Entity &light = world.entityManager.create();
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
  tinyxml2::XMLElement *endElement = rootHandle.FirstChildElement("end").ToElement();

  if (endElement) {
    Entity &door = world.entityManager.create();
    Transform &transform = door.addComponent<Transform>();
    Vector3f position;
    XmlHelper::extractPosition(endElement, position);
    transform.setPosition(position);
    Vector3f angles;
    XmlHelper::extractRotation(endElement, angles);
    transform.setOrientation(Quaternion().fromAero(angles));
    MeshDrawable &mesh = door.addComponent<MeshDrawable>();
    mesh.material = MaterialLoader::loadFromXML("door/door");
    mesh.mesh = MeshLoader::getMesh("Door.obj");
  }
}

void XmlMapLoader::extractWalls() {
  tinyxml2::XMLElement *wallBoxElement = rootHandle.FirstChildElement("wall").ToElement();

  if (wallBoxElement) {
    do {
      Entity &wall = world.entityManager.create();

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
  tinyxml2::XMLElement *acidElement = rootHandle.FirstChildElement("acid").ToElement();

  if (acidElement) {
    do {
      Entity &acid = world.entityManager.create();

      Transform &t = acid.addComponent<Transform>();
      Vector3f position;
      XmlHelper::extractPosition(acidElement, position);
      t.setPosition(position);
      Vector3f angles;
      XmlHelper::extractRotation(acidElement, angles);
      t.setOrientation(Quaternion().fromAero(angles));
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
  tinyxml2::XMLElement *triggerElement = rootHandle.FirstChildElement("trigger").ToElement();

  if (triggerElement) {
    do {
      //! [Creating an Entity.]
      Entity &trigger = world.entityManager.create();

      Transform &t = trigger.addComponent<Transform>();
      //! [Creating an Entity.]
      Vector3f position;
      XmlHelper::extractPosition(triggerElement, position);
      t.setPosition(position);
      Vector3f angles;
      XmlHelper::extractRotation(triggerElement, angles);
      t.setOrientation(Quaternion().fromAero(angles));
      Vector3f scale;
      XmlHelper::extractScale(triggerElement, scale);
      t.setScale(scale);
      XmlTriggerHelper::extractTriggerActions(trigger, triggerElement);

    } while ((triggerElement = triggerElement->NextSiblingElement("trigger")) != nullptr);
  }
}

void XmlMapLoader::extractModels() {
  int mid = -1;
  string mesh("none");
  tinyxml2::XMLElement *modelElement = rootHandle.FirstChildElement("model").ToElement();
  if (modelElement){
    do {
      modelElement->QueryIntAttribute("material", &mid);
      mesh = modelElement->Attribute("mesh");

      Entity &model = world.entityManager.create();
      Transform &transform = model.addComponent<Transform>();
      Vector3f position;
      XmlHelper::extractPosition(modelElement, position);
      transform.setPosition(position);
      Vector3f angles;
      XmlHelper::extractRotation(modelElement, angles);
      transform.setOrientation(Quaternion().fromAero(angles));
      MeshDrawable &meshDrawable = model.addComponent<MeshDrawable>();
      meshDrawable.material = world.materials[mid];
      meshDrawable.mesh = MeshLoader::getMesh(mesh);
    } while ((modelElement = modelElement->NextSiblingElement("model")) != nullptr);
  }
}
} /* namespace radix */
