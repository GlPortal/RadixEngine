#ifndef SPRIT_SHEET_LOADER_HPP
#define SPRIT_SHEET_LOADER_HPP
#include <string>

#include <radix/core/math/Vector2i.hpp>
#include <radix/core/math/Rectangle.hpp>


//=====================================================
//
//      V
//
//      ^
//      | spritSheet contain a lot of cells
//      | every cell is a frame at time
//  =   +-----------------------------------+
//  |   |  0 |  1 |  2 |  3 |  ......       |
//  |   |    |    |    |    |               |
//  |   +-----------------------------------+
//  |   | 16 | 17 |    |    |               |
//  |   |    |    |    |    |               |
//  |   +-----------------------------------+
//  H   |    |    |    |    |               |
//  e   |    |    |    |    |               |
//  i   +-----------------------------------+
//  g   |    |    |    |    |               |
//  h   |    |    |    |    |               |
//  t   +-----------------------------------+
//  |   |    |    |    |    |               |
//  |   |    |    |    |    |               |
//  |   +-----------------------------------+
//  |   |    |    |    |    |               |
//  |   |    |    |    |    |               |
//  =   +------------------------------------------> U
//
//    |--------------Width----------------|
//
//      |-----W-----| 
//
//  =   +-----------+
//  |   |           |
//  |   |           |
//  H   |   Grid 0  |
//  |   |           |
//  |   |           |
//  =   +-----------+
//
//=====================================================

namespace radix {
struct SpritSheet {
  int fullWidth() const { return mSpritSize.width; }
  int fullHeight() const { return mSpritSize.height; }

  int width() const { return mGridSize.width; }
  int height() const { return mGridSize.height; }

  int getFramesCount() const {
    return (mSpritSize.width / mGridSize.width) *
           (mSpritSize.height / mGridSize.height);
  }

  radix::RectangleF operator[](int index) const {
    const int w = (fullWidth()  / width());
    const int h = (fullHeight() / height());
    int x = index % w;
    int y = (index / w) % h;

    return {x * width()  / static_cast<float>(fullWidth()),
            y * height() / static_cast<float>(fullHeight()),
            width()      / static_cast<float>(fullWidth()),
            height()     / static_cast<float>(fullHeight())};
  }

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
  radix::Vector2i mCellSize;
};
} // namespace radix
#endif //! SPRIT_SHEET_LOADER_HPP
