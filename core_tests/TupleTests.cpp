#include "pch.h"
#include "CppUnitTest.h"

#include <tuple.h>
#include <string>
#include <sstream>
#include <vector>

#include "StringForm.h"

static const float EPSILON = 0.00001f;


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace coretests
{
	TEST_CLASS(TupleTests)
	{
	public:

		TEST_METHOD(PointHasW_1)
		{
			auto p = rt::point<float>(4, -4, 3);
			Assert::AreEqual(4.0f, p[0]);
			Assert::AreEqual(-4.0f, p[1]);
			Assert::AreEqual(3.0f, p[2]);
			Assert::AreEqual(1.0f, p[3]);
		}

		TEST_METHOD(VectorHasW_0)
		{
			auto p = rt::vector<float>(4, -4, 3);
			Assert::AreEqual(4.0f, p[0]);
			Assert::AreEqual(-4.0f, p[1]);
			Assert::AreEqual(3.0f, p[2]);
			Assert::AreEqual(0.0f, p[3]);
		}

		TEST_METHOD(AddingTuples)
		{
			auto a1 = rt::tuple<float, 4>({ 3, -2, 5, 1 });
			auto a2 = rt::tuple<float, 4>({ -2, 3, 1, 0 });
			Assert::AreEqual(rt::tuple<float, 4>({ 1, 1, 6, 1 }), a1 + a2);
		}

		TEST_METHOD(SubtractingTuples)
		{
			auto p1 = rt::point<float>(3, 2, 1);
			auto p2 = rt::point<float>(5, 6, 7);
			Assert::AreEqual(rt::vector<float>(-2, -4, -6), p1 - p2);
		}

		TEST_METHOD(NegatingTuples)
		{
			auto a = rt::tuple<float, 4>({ 1, -2, 3, -4 });
			Assert::AreEqual(rt::tuple<float, 4>({ -1, 2, -3, 4 }), -a);
		}

		TEST_METHOD(MultiplyByScalar)
		{
			auto a = rt::tuple<float, 4>({ 1, -2, 3, -4 });
			Assert::AreEqual(rt::tuple<float, 4>({ 3.5, -7, 10.5f, -14 }), a * 3.5f);
		}

		TEST_METHOD(DividingByScalar)
		{
			auto a = rt::tuple<float, 4>({ 1, -2, 3, -4 });
			Assert::AreEqual(rt::tuple<float, 4>({ 0.5, -1, 1.5, -2 }), a / 2);
		}

		TEST_METHOD(ComputeMagnitude)
		{
			std::vector<rt::tuple<float,4>> cases = {
				rt::vector<float>(1,0,0),
				rt::vector<float>(0,1,0),
				rt::vector<float>(0,0,1),
				rt::vector<float>(1,2,3),
				rt::vector<float>(-1,-2,-3),
			};

			std::vector<float> expectations = {
				1,
				1,
				1,
				sqrtf(14),
				sqrtf(14)
			};
			for (int i = 0; i < 5; i++) {
				Assert::AreEqual(expectations[i], cases[i].magnitude());
			}
		}

		TEST_METHOD(Normalize_4_0_0)
		{
			auto v = rt::vector<float>(4, 0, 0);
			Assert::AreEqual(rt::vector<float>(1, 0, 0), v.normalize());
		}

		TEST_METHOD(Normalize_1_2_3)
		{
			float d = sqrtf(14);
			auto v = rt::vector<float>(1,2,3);
			Assert::AreEqual(rt::vector<float>(1/d, 2/d, 3/d), v.normalize());
		}

		TEST_METHOD(MagnitudeOfNormalized)
		{
			auto v = rt::vector<float>(1, 2, 3);
			auto n = v.normalize();
			Assert::AreEqual(1.f, n.magnitude(), EPSILON);
		}

		TEST_METHOD(DotProduct)
		{
			auto a = rt::vector<float>(1, 2, 3);
			auto b = rt::vector<float>(2, 3, 4);
			Assert::AreEqual(20.f, dot(a, b));
		}

		TEST_METHOD(CrossProduct)
		{
			auto a = rt::vector<float>(1, 2, 3);
			auto b = rt::vector<float>(2, 3, 4);

			Assert::AreEqual(rt::vector<float>(-1, 2, -1), rt::cross(a, b));
			Assert::AreEqual(rt::vector<float>(1, -2, 1), rt::cross(b,a));
		}
	};
}
