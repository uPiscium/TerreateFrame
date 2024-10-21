#ifndef __TERREATE_GRAPHICS_CONVERTER_HPP__
#define __TERREATE_GRAPHICS_CONVERTER_HPP__

#include "compute.hpp"
#include "defines.hpp"
#include "texture.hpp"

namespace TerreateGraphics::Compute {
using namespace TerreateGraphics::Defines;
using namespace TerreateGraphics::Compute;

class ImageConverter final : public TerreateObjectBase {
private:
  static Uint const sKernelInputSize = 16;
  static Str const sKernelSource;

private:
  GLObject mInputTexture;
  ComputeKernel mKernel;

private:
  void CreateInputTexture();

public:
  /*
   * @brief Construct a new Image Converter object
   */
  ImageConverter();
  ~ImageConverter();

  /*
   * @brief: Stretch the image data to a texture
   * @param: name: The name of the texture
   * @param: width: The width of the image
   * @param: height: The height of the image
   * @param: channels: The number of channels in the image
   * @param: pixels: The pixel data of the image
   * @param: storage: The texture to store the data
   */
  void Convert(Str const &name, Uint const &width, Uint const &height,
               Uint const &channels, Ubyte const *pixels, Texture &storage);
  /*
   * @brief: Stretch the image data to specified layer of texture
   * @param: name: The name of the texture
   * @param: index: The layer of the texture
   * @param: width: The width of the image
   * @param: height: The height of the image
   * @param: channels: The number of channels in the image
   * @param: pixels: The pixel data of the image
   * @param: storage: The texture to store the data
   */
  void Convert(Str const &name, Uint const &index, Uint const &width,
               Uint const &height, Uint const &channels, Ubyte const *pixels,
               Texture &storage);
};
} // namespace TerreateGraphics::Compute

#endif // __TERREATE_GRAPHICS_CONVERTER_HPP__
