#include "cyCodec.h"

namespace CYLLENE_SDK {

  Resource*
  ImageCodec::load(Path pathToResource) {
    FREE_IMAGE_FORMAT f = static_cast<FREE_IMAGE_FORMAT>( IMGEXT::E::_from_string_nocase(pathToResource.extension().c_str())._to_index());
    FreeImage_Load(f, pathToResource.fullPath().c_str());
  }


}