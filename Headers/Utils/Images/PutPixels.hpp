#ifndef __PUTPIXELS_HPP__
#define __PUTPIXELS_HPP__

#include "MathGraphicalLib/Vectors/Vector4.hpp"
#include "MathGraphicalLib/Vectors/Defines.hpp"

#include <cstdint>
#include <vector>

namespace Vox::Utils::Images
{
	using namespace MGL::Vectors::Types;

	inline void PutPixel(std::vector<uint8_t> &image, Vector2SizeT &imgSize,
						 const Vector2SizeT &pixelCoord, bool opacity = false,
						 const Vector3Float &pixelColor = {255, 255, 255})
	{
		(void)opacity;
		size_t rPixelIndex = pixelCoord[1] * 4 * imgSize[0] + (pixelCoord[0] * 4);
		for (size_t offset = 0; offset < 3; offset++)
			image[rPixelIndex + offset] = pixelColor[offset];
        image[rPixelIndex + 3] = 255;
        
	}
} // namespace Vox::Utils::Images

#endif // __PUTPIXELS_HPP__