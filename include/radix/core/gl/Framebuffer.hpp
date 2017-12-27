#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP
//#include <glad/glad.h>
#include <string>
#include <utility>
#include <radix/core/gl/OpenGL.hpp>

namespace radix {
  enum FramebufferType {
    Texture = 1,
    Depth   = 2,
    Stencil = 4
  };

  struct FrameBuffer {
    ~FrameBuffer();

    /** 
    * @brief create uchar RGB texture
    * with Nearest filter and clamp to edge
    * 
    * @param width  texture width  level 0
    * @param height texture height level 0
    * 
    * @return OpenGL texture handler
    */
    static GLuint createTexture(const int width, const int height);

    /** 
    * @brief create renderbuffer
    * 
    * @param width buffer width
    * @param height buffer height
    * @param type buffer type ex. GL_RGB
    * 
    * @return OpenGL renderbuffer handler
    */
    static GLuint createRenderBuffer(const int width, const int height,
                                         GLenum type);

    /** 
    * @brief create Framebuffer and attachments
    * 
    * @param viewportWidth  Framebuffer width
    * @param viewportHeight Framebuffer height
    * @param types          types of attachments
    */
    void create(const int viewportWidth, const int viewportHeight,
                const FramebufferType types);

    /** 
    * @brief bind framebuffer
    */
    inline void bind() {
      glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    }

    /** 
    * @brief unbind framebuffer
    */
    inline static void unbind() {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /** 
    * @brief bind color Texture
    */
    inline void bindTexture() {
      if(colorTexture != 0) {
        glBindTexture(GL_TEXTURE_2D, colorTexture);
      }
    }

    /** 
    * @brief unbind color Texture
    */
    inline static void unbindTexture() {
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    /** 
    * @brief Save all attachment to hard
    * 
    * @param dir location to save images
    */
    void save(std::string&& dir);

  protected:
    int    width        = 0;
    int    height       = 0;
    GLuint frameBuffer  = 0;
    GLuint colorTexture = 0;
    GLuint depthTexture = 0;
  };
}
#endif //!FRAME_BUFFER_HPP
