#ifndef SPRIT_SHEET_LOADER_HPP
#define SPRIT_SHEET_LOADER_HPP
#include <string>

#include <radix/core/math/Vector2i.hpp>


namespace radix {
struct SpritSheet {
  int fullWidth() { return mSpritSize.width; }
  int fullHeight() { return mSpritSize.height; }

  int width() { return mGridSize.width; }
  int height() { return mGridSize.height; }

  radix::Vector2i mGridSize;
  radix::Vector2i mSpritSize;
};

class SpritSheetLoader {
public:
  void load(const std::string& textureID);

  void setSpritSize(const radix::Vector2i& vec);

  SpritSheet create();

protected:
  std::string     mTextureID;
  radix::Vector2i mGridSize;
};
} // namespace radix
#endif //! SPRIT_SHEET_LOADER_HPP
