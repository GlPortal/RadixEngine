#ifndef RADIX_WINDOW_HPP
#define RADIX_WINDOW_HPP

#include <memory>
#include <string>
#include <vector>

#include <SDL2/SDL_video.h>

#include <Gwen/Controls/Canvas.h>
#include <Gwen/Skins/TexturedBase.h>

#include <radix/input/InputSource.hpp>
#include <radix/Viewport.hpp>

namespace radix {

class GlGwenRenderer;
class GWENInput;  

class Window : public Viewport, public InputSource {
public:
  Window();
  ~Window();
  void create(const char*);
  void setFullscreen();
  void swapBuffers();
  void getSize(int *width, int *height) const;
  void close();

  void lockMouse();
  void unlockMouse();

  std::unique_ptr<radix::GlGwenRenderer> gwenRenderer;
  std::unique_ptr<Gwen::Skin::TexturedBase> gwenSkin;
  std::unique_ptr<Gwen::Controls::Canvas> gwenCanvas;
  std::unique_ptr<radix::GWENInput> gwenInput;

  void processEvents();

  void keyPressed(KeyboardKey key, KeyboardModifier mod);
  void keyReleased(KeyboardKey key, KeyboardModifier mod);
  bool isKeyDown(KeyboardKey key);
  std::string getCharBuffer();
  void addToBuffer(const std::string &character);
  void clearBuffer();
  void truncateCharBuffer();
  void clear();
  
private:
  void initEpoxy();
  int width;
  int height;
  SDL_Window *window;
  SDL_GLContext context;

  std::vector<bool> keystates;
  std::string charbuffer;

  static const char* DEFAULT_TITLE;
  static const int DEFAULT_WIDTH, DEFAULT_HEIGHT;
};

} /* namespace radix */

#endif /* RADIX_WINDOW_HPP */
