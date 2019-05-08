#include "pch.h"
#include "CppUnitTest.h"

#include <canvas.h>
static const float EPSILON = 0.00001f;

#include "StringForm.h"

template <>
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString(const rt::canvas& c)
{
	std::wstringstream ss;
	ss << "canvas(" <<
		c.width << "," <<
		c.height << ")";

	return ss.str().c_str();
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace coretests
{
	TEST_CLASS(CanvasTests)
	{
	public:
		TEST_METHOD(CreatingACanvas)
		{
			auto color = rt::color(0, 0, 0);
			auto canvas = rt::canvas(10, 20);
			Assert::AreEqual(10, canvas.width);
			Assert::AreEqual(20, canvas.height);

			for (auto pixel : canvas) {
				Assert::AreEqual(color, pixel);
			}
		}

		TEST_METHOD(WritePixelToCanvas)
		{
			auto red = rt::color(1, 0, 0);
			auto canvas = rt::canvas(10, 20);
			canvas[{2, 3}] = red;
			Assert::AreEqual(red, canvas[{2, 3}]);
		}

		TEST_METHOD(ConstructingThePPMHeader)
		{
			auto c = rt::canvas(5, 3);
			auto ppm = c.toPpm();
			Assert::AreEqual("P3\n5 3\n255\n", ppm.substr(0, 11).c_str());
		}

		TEST_METHOD(ConstructingThePPMPixelData)
		{
			auto c = rt::canvas(5, 3);
			auto c1 = rt::color(1.5, 0, 0);
			auto c2 = rt::color(0, 0.5, 0);
			auto c3 = rt::color(-0.5, 0, 1);

			c[{0,0}] = c1;
			c[{2,1}] = c2;
			c[{4,2}] = c3;

			std::istringstream buf{ c.toPpm() };
			std::string line;
			for (int i = 0; i < 3; i++) {
				std::getline(buf, line);
			}
			std::getline(buf, line);
			Assert::AreEqual("255 0 0 0 0 0 0 0 0 0 0 0 0 0 0", line.c_str());
			std::getline(buf, line);
			Assert::AreEqual("0 0 0 0 0 0 0 128 0 0 0 0 0 0 0", line.c_str());
			std::getline(buf, line);
			Assert::AreEqual("0 0 0 0 0 0 0 0 0 0 0 0 0 0 255", line.c_str());
		}

		TEST_METHOD(PPMEndsWithNewLine)
		{
			auto c = rt::canvas(5, 3);
			auto ppm = c.toPpm();
			Assert::AreEqual('\n', ppm[ppm.size() - 1]);
		}
	};
};