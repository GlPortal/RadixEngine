#ifndef RADIX_TEXTURE_HPP
#define RADIX_TEXTURE_HPP

namespace radix {

struct Texture {
  /**
   * @brief default constructor
   */
  Texture() = default;

  /**
   * @brief Texture constructor
   * @param handle  OpenGL Texture handler
   * @param width   Texture width
   * @param height  Texture height
   */
  Texture(unsigned int handle, int width, int height)
      : handle(handle), width(width), height(height) {}

  unsigned int handle = 0; /** < OpenGL Texture handler */
  int width;               /** < Texture width */
  int height;              /** < Texture height */
};

} /* namespace radix */

#endif /* RADIX_TEXTURE_HPP */
