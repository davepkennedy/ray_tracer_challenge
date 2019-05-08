#pragma once

#include <ostream>

namespace rt
{
	class color {
	public:
		/*const*/ float red;
		/*const*/ float green;
		/*const*/ float blue;
	public:
		color() : color(0, 0, 0) {}

		color(float r, float g, float b)
			: red(r)
			, green(g)
			, blue(b)
		{

		}

		bool operator== (const color& other) const
		{
			return red == other.red &&
				green == other.green &&
				blue == other.blue;
		}

		color operator+ (const color& other) const
		{
			return color(red + other.red,
				green + other.green,
				blue + other.blue);
		}

		color operator- (const color & other) const
		{
			return color(red - other.red,
				green - other.green,
				blue - other.blue);
		}

		color operator* (float scalar) const
		{
			return color(red * scalar,
				green * scalar,
				blue * scalar);
		}

		color operator* (const color & other) const
		{
			return color(red * other.red,
				green * other.green,
				blue * other.blue);
		}

	};
}
