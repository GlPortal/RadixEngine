#include <radix/core/gl/Framebuffer.hpp>
#include <radix/env/Util.hpp>

#include <FreeImage.h>

namespace radix {
  FrameBuffer::~FrameBuffer() {
    unbind();
    glDeleteFramebuffers(1,  &frameBuffer);
    if(colorTexture != 0) {
      unbindTexture();
      glDeleteTextures(1, &colorTexture);
    }
    if(depthTexture != 0) {
      glDeleteRenderbuffers(1, &depthTexture);
    }
  }

  GLuint FrameBuffer::createTexture(const int width, const int height){
    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

    return textureID;
  }

  GLuint
  FrameBuffer::createRenderBuffer(const int width, const int height, GLenum type) {
    GLuint renderBuffer = 0;
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);

    return renderBuffer;
  }

  void
  FrameBuffer::create(const int viewportWidth, const int viewportHeight,
                      const FramebufferType type) {
    width  = viewportWidth;
    height = viewportHeight;

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    if(type & FramebufferType::Texture) {
      colorTexture = createTexture(viewportWidth, viewportHeight);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
          GL_RENDERBUFFER, colorTexture);
    }

    if(type & FramebufferType::Depth) {
      depthTexture = createRenderBuffer(viewportWidth, viewportHeight,
                    (type & FramebufferType::Stencil) ?
                    GL_DEPTH_COMPONENT : GL_DEPTH_STENCIL);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
          GL_RENDERBUFFER, depthTexture);

      if(type & FramebufferType::Stencil) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
            GL_RENDERBUFFER, depthTexture);
      }
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      Util::Log(Error, "FrameBuffer") << "Error to create FrameBuffer!";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void
  FrameBuffer::save(std::string&& fileName) {
      bind();
      std::vector<unsigned char> textureData(width*height*3);
      glReadPixels(0, 0, width, height,
          GL_RGB, GL_UNSIGNED_BYTE, textureData.data());
      FIBITMAP* Image = FreeImage_ConvertFromRawBits(textureData.data(),
          width, height, 3*width, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
      FreeImage_Save(FIF_BMP, Image, fileName.c_str(), 0);
      unbind();
    }
}

