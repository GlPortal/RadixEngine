#include <radix/data/texture/SpritSheetLoader.hpp>
#include <radix/data/texture/TextureLoader.hpp>


namespace radix {
void SpritSheetLoader::load(const std::string& textureID) {
  mTextureID = textureID;
}

SpritSheet SpritSheetLoader::create() {
  auto texture = TextureLoader::getTexture(mTextureID);
  if(texture.height > 0 && texture.width > 0) {
    const radix::Vector2i fullDim{texture.width, texture.height};
    radix::Vector2i gridDim = fullDim;
    if(mGridSize.height > 0 && mGridSize.width > 0) {
      gridDim = {fullDim.width  / mGridSize.width,
                 fullDim.height / mGridSize.height};
    }
    return SpritSheet{gridDim, fullDim};
  }
  return SpritSheet{};
}

void SpritSheetLoader::setSpritSize(const radix::Vector2i& vec) {
  mGridSize = vec;
}

} // namespace radix
