#ifndef __PUTPIXELS_HPP__
#define __PUTPIXELS_HPP__

#include "MathGraphicalLib/Vectors/Vector2.hpp"
#include "MathGraphicalLib/Vectors/Vector3.hpp"
#include "MathGraphicalLib/Vectors/Vector4.hpp"

#include <cstdint>
#include <vector>

namespace Vox::Utils::Images
{
	inline void PutPixel(std::vector<uint8_t> &image, MGL::Vectors::Vector2<size_t> &imgSize,
						 const MGL::Vectors::Vector2<size_t> &pixelCoord, bool opacity = false,
						 const MGL::Vectors::Vector3<float> &pixelColor = {255, 255, 255})
	{
		size_t rPixelIndex = pixelCoord[1] * 4 * imgSize[0] + (pixelCoord[0] * 4);
		for (size_t offset = 0; offset < 3; offset++)
			image[rPixelIndex + offset] = pixelColor[offset];
        image[rPixelIndex + 3] = 255;
        
	}
} // namespace Vox::Utils::Images

#endif // __PUTPIXELS_HPP__