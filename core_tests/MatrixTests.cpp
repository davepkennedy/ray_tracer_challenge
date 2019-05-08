#include "pch.h"
#include "CppUnitTest.h"

#include <color.h>
#include <matrix.h>
#include <tuple.h>

static const float EPSILON = 0.00001f;

#include "StringForm.h"

template <int W, int H>
std::wstring MatrixToString(const rt::matrix<W, H>& m) {
	std::wstringstream ss;
	ss << "matrix(" << std::endl;
	for (int r = 0; r < m.height; r++) {
		for (int c = 0; c < m.width; c++) {
			ss << m[{c, r}] << ", ";
		}
		ss << std::endl;
	}
	return ss.str().c_str();
}

template <>
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString(const rt::matrix4x4& m)
{
	return MatrixToString(m);
}

template <>
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString(const rt::matrix3x3& m)
{
	return MatrixToString(m);
}

template <>
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString(const rt::matrix2x2& m)
{
	return MatrixToString(m);
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace coretests
{
	TEST_CLASS(MatrixTests)
	{
		TEST_METHOD(ConstructingA4x4Matrix)
		{
			auto mat = rt::matrix4x4({
				1,2,3,4,
				5.5,6.5,7.5,8.5,
				9,10,11,12,
				13.5,14.5,15.5,16.5
				});

			Assert::AreEqual(1.f, mat[{rt::column(0), rt::row(0)}]);
			Assert::AreEqual(4.f, mat[{rt::column(0), rt::row(3)}]);
			Assert::AreEqual(5.5f, mat[{rt::column(1), rt::row(0)}]);
			Assert::AreEqual(7.5f, mat[{rt::column(1), rt::row(2)}]);
			Assert::AreEqual(11.f, mat[{rt::column(2), rt::row(2)}]);
			Assert::AreEqual(13.5f, mat[{rt::column(3), rt::row(0)}]);
			Assert::AreEqual(15.5f, mat[{rt::column(3), rt::row(2)}]);
		}

		TEST_METHOD(A2DMatrixShouldBeRepresentable)
		{
			auto mat = rt::matrix2x2({
				-3,5,
				1,-2
				});

			Assert::AreEqual(-3.0f, mat[{rt::column(0), rt::row(0)}]);
			Assert::AreEqual(5.0f, mat[{rt::column(0), rt::row(1)}]);
			Assert::AreEqual(1.0f, mat[{rt::column(1), rt::row(0)}]);
			Assert::AreEqual(-2.0f, mat[{rt::column(1), rt::row(1)}]);
		}

		TEST_METHOD(A3DMatrixShouldBeRepresentable)
		{
			auto mat = rt::matrix3x3({
				-3,5,0,
				1,-2,-7,
				0,1,1
				});

			Assert::AreEqual(-3.0f, mat[{rt::column(0), rt::row(0)}]);
			Assert::AreEqual(-2.0f, mat[{rt::column(1), rt::row(1)}]);
			Assert::AreEqual(1.0f, mat[{rt::column(2), rt::row(2)}]);
		}

		TEST_METHOD(MatrixEqualityWithIdenticalMatrices)
		{
			auto matA = rt::matrix4x4({
				1, 2, 3, 4,
				5, 6, 7, 8,
				9, 10, 11, 12,
				13, 14, 15, 16
				});
			auto matB = rt::matrix4x4({
				1, 2, 3, 4,
				5, 6, 7, 8,
				9, 10, 11, 12,
				13, 14, 15, 16
				});
			Assert::IsTrue(matA==matB);
		}

		TEST_METHOD(MatrixEqualityWithDifferentMatrices)
		{
			auto matA = rt::matrix4x4({
				1, 2, 3, 4,
				5, 6, 7, 8,
				8, 7, 6, 5,
				4, 3, 2, 1
				});
			auto matB = rt::matrix4x4({
				2, 3, 4, 5,
				6, 7, 8, 9,
				9, 8, 7, 6,
				5, 4, 3, 2
				});
			Assert::IsTrue(matA != matB);
		}

		TEST_METHOD(MultiplyingTwoMatrices) 
		{
			auto matA = rt::matrix4x4({
				1,2,3,4,
				5,6,7,8,
				9,8,7,6,
				5,4,3,2
			});
			auto matB = rt::matrix4x4({
				-2,1,2,3,
				3,2,1,-1,
				4,3,6,5,
				1,2,7,8
			});

			auto exp = rt::matrix4x4({
				20, 22, 50, 48,
				44, 54, 114, 108,
				40, 58, 110, 102,
				16, 26, 46, 42
			});
			auto matC = matA * matB;
			Assert::AreEqual(exp, matC);
		}

		TEST_METHOD(MultiplyByATuple)
		{
			auto matrix = rt::matrix4x4({
				1,2,3,4,
				2,4,4,2,
				8,6,4,1,
				0,0,0,1
				});
			auto tuple = rt::tuple<float, 4>({ 1, 2, 3, 1 });
			Assert::AreEqual(rt::tuple<float, 4>({ 18, 24, 33, 1 }), matrix * tuple);
		}

		TEST_METHOD(MultiplyByIdentityMatrix) {
			auto matrix = rt::matrix4x4({
				0,1,2,4,
				1,2,4,8,
				2,4,8,16,
				4,8,16,32
				});

			Assert::AreEqual(matrix, matrix * rt::matrix4x4::identity());
		}

		TEST_METHOD(MultiplyIdentityMatrixByTuple) {
			auto tuple = rt::tuple<float, 4>({ 1, 2, 3, 4 });
			Assert::AreEqual(tuple, rt::matrix4x4::identity() * tuple);
		}

		TEST_METHOD(TransposeAMatrix) {
			auto a = rt::matrix4x4({
				0,9,3,0,
				9,8,0,8,
				1,8,5,3,
				0,0,5,8
			});
			auto b = rt::matrix4x4({
				0,9,1,0,
				9,8,8,0,
				3,0,5,5,
				0,8,3,8
			});
			Assert::AreEqual(b, a.transpose());
		}

		TEST_METHOD(DeterminantOf2x2)
		{
			auto m = rt::matrix2x2({
				1,5,
				-3,2
				});
			Assert::AreEqual(17.f, m.determinant());
		}

		TEST_METHOD(SubmatrixOf3x3) {
			auto m = rt::matrix3x3({
				1,5,0,
				-3,2,7,
				0,6,-3});

			Assert::AreEqual(rt::matrix2x2({
				-3,2,
				0,6 }), m.submatrix(0, 2));
		}

		TEST_METHOD(SubmatrixOf4x4) {
			auto m = rt::matrix4x4({
				-6,1,1,6,
				-8,5,8,6,
				-1,0,8,2,
				-7,1,-1,1
			});

			Assert::AreEqual(rt::matrix3x3({
				-6,1,6,
				-8,8,6,
				-7,-1,1 }), m.submatrix(2, 1));
		}

		TEST_METHOD(MinorOf3x3Matrix) {
			auto m = rt::matrix3x3({
				3,5,0,
				2,-1,-7,
				6,-1,5
				});
			auto b = m.submatrix(1, 0);
			Assert::AreEqual(25.f, b.determinant());
			Assert::AreEqual(25.f, m.minor(1, 0));
		}

		TEST_METHOD(CofactorOf3x3Matrix)
		{
			auto m = rt::matrix3x3({
				3,5,0,
				2,-1,-7,
				6,-1,5
				});
			Assert::AreEqual(-12.f, m.minor(0, 0));
			Assert::AreEqual(-12.f, m.cofactor(0, 0));
			Assert::AreEqual(25.f, m.minor(1, 0));
			Assert::AreEqual(-25.f, m.cofactor(1, 0));
		}

		TEST_METHOD(DeterminantOf3x3Matrix)
		{
			auto m = rt::matrix3x3({
				1,2,6,
				-5,8,-4,
				2,6,4
				});

			Assert::AreEqual(56.f, m.cofactor(0, 0));
			Assert::AreEqual(12.f, m.cofactor(0, 1));
			Assert::AreEqual(-46.f, m.cofactor(0, 2));
			Assert::AreEqual(-196.f, m.determinant());
		}

		TEST_METHOD(DeterminantOf4x4Matrix)
		{
			auto m = rt::matrix4x4({
				-2,-8,3,5,
				-3,1,7,3,
				1,2,-9,6,
				-6,7,7,-9
				});

			Assert::AreEqual(690.f, m.cofactor(0, 0));
			Assert::AreEqual(447.f, m.cofactor(0, 1));
			Assert::AreEqual(210.f, m.cofactor(0, 2));
			Assert::AreEqual(51.f, m.cofactor(0, 3));
			Assert::AreEqual(-4071.f, m.determinant());
		}

		TEST_METHOD(MatrixIsInvertible)
		{
			auto m = rt::matrix4x4({
				6,4,4,4,
				5,5,7,6,
				4,-9,3,-7,
				9,1,7,-6
				});
			Assert::AreEqual(-2120.f, m.determinant());
			auto b = m.invert();
		}

		TEST_METHOD(MatrixIsNotInvertible)
		{
			auto m = rt::matrix4x4({
				-4,2,-2,-2,
				9,6,2,6,
				0,-5,1,-5,
				0,0,0,0
				});
			Assert::AreEqual(0.f, m.determinant());
			Assert::ExpectException<std::runtime_error>([m] {m.invert(); });
		}

		TEST_METHOD(CalculateTheInverseOfAMatrix)
		{
			auto a = rt::matrix4x4({
				-5,2,6,-8,
				1,-5,1,8,
				7,7,-6, -7,
				1,-3,7,4
				});
			auto b = a.invert();
			Assert::AreEqual(532.f, a.determinant());
			Assert::AreEqual(-160.f, a.cofactor(2, 3));
			Assert::AreEqual(-160.f / 532, b [{rt::column(3), rt::row(2)}]);
			Assert::AreEqual(105.f, a.cofactor(3, 2));
			Assert::AreEqual(105.f / 532, b[{rt::column(2), rt::row(3)}]);

			Assert::AreEqual(rt::matrix4x4({
				0.21805f, 0.45113f, 0.24060f, -0.04511f,
				-0.80827f, -1.45677f, -0.44361f, 0.52068f,
				-0.07895f, -0.22368f, -0.05263f, 0.19737f,
				-0.52256f, -0.81391f, -0.30075f, 0.30639f }), b);
		}

		TEST_METHOD(CalculatingTheInverseOfAnotherMatrix)
		{

			auto a = rt::matrix4x4({
				8, -5, 9, 2,
				7, 5, 6, 1,
				-6, 0, 9, 6,
				-3, 0, -9, -4
				});

			auto b = rt::matrix4x4({
				-0.15385, -0.15385, -0.28205, -0.53846,
				-0.07692, 0.12308, 0.02564, 0.03077,
				0.35897, 0.35897, 0.43590, 0.92308,
				-0.69231, -0.69231, -0.76923, -1.92308
				});

			Assert::AreEqual(b, a.invert());
		}

		TEST_METHOD(CalculatingTheInverseOfAThirdMatrix)
		{
			auto a = rt::matrix4x4({
				9, 3, 0, 9,
				-5, -2, -6, -3,
				-4, 9, 6, 4,
				-7, 6, 6, 2
				});
			auto b = rt::matrix4x4({
				-0.04074, -0.07778, 0.14444, -0.22222,
				-0.07778, 0.03333, 0.36667, -0.33333,
				-0.02901, -0.14630, -0.10926, 0.12963,
				 0.17778, 0.06667, -0.26667, 0.33333,
				});
			Assert::AreEqual(b, a.invert());
		}

		TEST_METHOD(MultiplyingAProductByItsInverse)
		{
			auto a = rt::matrix4x4({
				3, -9, 7, 3,
				3, -8, 2, -9,
				-4, 4, 4, 1,
				-6, 5, -1, 1,
				});
			auto b = rt::matrix4x4({
				8, 2, 2, 2,
				3, -1, 7, 0,
				7, 0, 5, 4,
				6, -2, 0, 5,
				});
			auto c = a * b;
			Assert::AreEqual(a, c * b.invert());
		}
	};
}