
#include <radix/BaseGame.hpp>
#include <radix/env/Environment.hpp>
#include <radix/SoundManager.hpp>
#include <radix/system/PlayerSystem.hpp>
#include <radix/system/PhysicsSystem.hpp>
#include <radix/component/Player.hpp>
#include <radix/env/ArgumentsParser.hpp>

namespace radix {

Fps BaseGame::fps;

BaseGame::BaseGame() :
    world(window),
    config(),
    gameWorld(window),
    closed(false) {
  radix::Environment::init();
  config = Environment::getConfig();
  radix::ArgumentsParser::populateConfig(config);
  window.setConfig(config);
}

BaseGame::~BaseGame() {
}

void BaseGame::setup() {
  SoundManager::init();
  createWindow();
  world.setConfig(config);
  world.create();
  renderer = std::make_unique<Renderer>(world);
  camera = std::make_unique<Camera>();
  SystemManager::Transaction systemTransaction = world.systems.transact();
  systemTransaction.addSystem<PlayerSystem>(this);
  systemTransaction.addSystem<PhysicsSystem>(this);
  createScreenshotCallbackHolder();
  nextUpdate = SDL_GetTicks(), lastUpdate = 0, lastRender = 0;
  renderer->setViewport(&window);
  screenRenderer = std::make_unique<ScreenRenderer>(world, *renderer.get(), gameWorld);
  initHook();
  loadMap();
  renderer->init();
  renderer->addRenderer(*screenRenderer);
}

bool BaseGame::isRunning() {
  return !closed;
}

World* BaseGame::getWorld() {
  return &world;
}

ScreenRenderer* BaseGame::getScreenRenderer() {
  return screenRenderer.get();
}

GameWorld* BaseGame::getGameWorld() {
  return &gameWorld;
}

void BaseGame::update() {
  int skipped = 0;
  currentTime = SDL_GetTicks();

  while (currentTime > nextUpdate && skipped < MAX_SKIP) {
    nextUpdate += SKIP_TIME;
    skipped++;
  }
  int elapsedTime = currentTime - lastUpdate;
  SoundManager::update(world.getPlayer());
  world.update(TimeDelta::msec(elapsedTime));
  lastUpdate = currentTime;
}

void BaseGame::processInput() { } /* to avoid pure virtual function */
void BaseGame::initHook() { }

void BaseGame::cleanUp() {
  world.destroy();
  window.close();
}

void BaseGame::render() {
  prepareCamera();
  renderer->render();
  gameWorld.getScreens()->clear();
  fps.countCycle();
  window.swapBuffers();
  lastRender = currentTime;
}

void BaseGame::prepareCamera() {
  camera->setPerspective();
  int viewportWidth, viewportHeight;
  window.getSize(&viewportWidth, &viewportHeight);
  camera->setAspect((float)viewportWidth / viewportHeight);
  const Transform &playerTransform = world.getPlayer().getComponent<Transform>();
  Vector3f headOffset(0, playerTransform.getScale().y, 0);
  camera->setPosition(playerTransform.getPosition() + headOffset);
  const Player &playerComponent = world.getPlayer().getComponent<Player>();
  camera->setOrientation(playerComponent.getHeadOrientation());
}

void BaseGame::close() {
  closed = true;
}

void BaseGame::loadMap() {
  XmlMapLoader mapLoader(world);
  std::string mapPath = config.getMapPath();
  if (mapPath.length() > 0) {
    mapLoader.load(mapPath);
  } else {
    mapLoader.load(Environment::getDataDir() + defaultMap);
  }
}

void BaseGame::createWindow() {
  window.create(windowTitle.c_str());
  if(config.getCursorVisibility()) {
    window.unlockMouse();
  } else {
    window.lockMouse();
  }
}

void BaseGame::createScreenshotCallbackHolder() {
  screenshotCallbackHolder =
    world.event.addObserver(InputSource::KeyReleasedEvent::Type, [this](const radix::Event &event) {
        const int key =  ((InputSource::KeyReleasedEvent &) event).key;
        if (key == SDL_SCANCODE_G) {
          this->window.printScreenToFile(radix::Environment::getDataDir() + "/screenshot.bmp");
        }
      });
}
} /* namespace radix */
