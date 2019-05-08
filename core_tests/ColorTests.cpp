#include "pch.h"
#include "CppUnitTest.h"

#include <color.h>
static const float EPSILON = 0.00001f;

#include "StringForm.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace coretests
{
	TEST_CLASS(ColorTests)
	{
	public:
		TEST_METHOD(ColorsAreRGBTuples)
		{
			auto c = rt::color(-0.5f, 0.4f, 1.7f);
			Assert::AreEqual(-0.5f, c.red);
			Assert::AreEqual(0.4f, c.green);
			Assert::AreEqual(1.7f, c.blue);
		}

		TEST_METHOD(AddingColors)
		{
			auto a = rt::color(0.9f, 0.6f, 0.75f);
			auto b = rt::color(0.7f, 0.1f, 0.25f);
			auto c = a + b;
			Assert::AreEqual(1.6f, c.red, EPSILON);
			Assert::AreEqual(0.7f, c.green, EPSILON);
			Assert::AreEqual(1.0f, c.blue, EPSILON);
		}

		TEST_METHOD(SubtractingColors)
		{
			auto a = rt::color(0.9f, 0.6f, 0.75f);
			auto b = rt::color(0.7f, 0.1f, 0.25f);
			auto c = a - b;
			Assert::AreEqual(0.2f, c.red, EPSILON);
			Assert::AreEqual(0.5f, c.green, EPSILON);
			Assert::AreEqual(0.5f, c.blue, EPSILON);
		}

		TEST_METHOD(MultiplyByScalar)
		{
			auto a = rt::color(0.2f, 0.3f, 0.4f);
			auto b = a * 2;

			Assert::AreEqual(0.4f, b.red);
			Assert::AreEqual(0.6f, b.green);
			Assert::AreEqual(0.8f, b.blue);
		}

		TEST_METHOD(MultiplyByColor)
		{
			auto a = rt::color(1.0f, 0.2f, 0.4f);
			auto b = rt::color(0.9f, 1.0f, 0.1f);
			auto c = a * b;
			Assert::AreEqual(0.9f, c.red, EPSILON);
			Assert::AreEqual(0.2f, c.green, EPSILON);
			Assert::AreEqual(0.04f, c.blue, EPSILON);
		}
	};
}