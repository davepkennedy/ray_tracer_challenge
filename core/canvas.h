#pragma once

#include "color.h"
#include "size.h"
#include <vector>
#include <string>
#include <sstream>

inline int clamp(float f) {
	if (f < 0) { return 0; }
	if (f > 255) { return 255; }
	return ceilf(f);
}

std::ostream& operator << (std::ostream& s, const rt::color& c) {
	s << clamp(255*c.red) << " " << clamp(255*c.green) << " " << clamp(255*c.blue);
	return s;
}

namespace rt
{
	class canvas {
	private:
		std::vector<rt::color> _pixels;
		using iterator = std::vector<rt::color>::iterator;
		using const_iterator = std::vector<rt::color>::const_iterator;
	public:
		const int width;
		const int height;
	public:
		canvas(int w, int h)
			: width(w)
			, height(h)
		{
			_pixels.resize(w * h);
		}

		const_iterator begin() const {
			return _pixels.begin();
		}

		const_iterator end() const {
			return _pixels.end();
		}

		color& at(size s) {
			return _pixels[width * s.sy + s.sx];
		}

		const color& at(size s) const {
			return _pixels[width * s.sy + s.sx];
		}

		color& operator[](size s) {
			return at(s);
		}

		std::string toPpm() const {
			std::stringstream buf;
			buf << "P3" << std::endl;
			buf << width << " " << height << std::endl;
			buf << 255 << std::endl;

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					if (x) { buf << " "; }
					buf << at({x, y});
				}
				buf << std::endl;
			}

			return buf.str();
		}
	};
}