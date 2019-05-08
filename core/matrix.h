#pragma once

#include <array>
#include <algorithm>
#include <initializer_list>
#include "size.h"
#include "equals.h"
#include "tuple.h"

namespace rt
{
	class row {
	public:
		const size_t r;
		explicit row(size_t _r) :r(_r) {}
		operator size_t() { return r; }
	};

	class column {
	public:
		const size_t c;
		explicit column(size_t _c) :c(_c) {}
		operator size_t() { return c; }
	};

	template <int W, int H>
	class matrix {
	public:
		static const int width = W;
		static const int height = H;

		using matrix_type = matrix<W, H>;
	private:
		std::array<float, W* H> _elems;
	protected:
		float& at(const std::pair<column, row>& rc) 
		{
			return _elems[width * rc.second.r + rc.first.c];
		}
		float at(const std::pair<column, row>& rc) const
		{
			return _elems[width * rc.second.r + rc.first.c];
		}
	public:

		matrix() : matrix(std::array<float, W* H>()) {}

		matrix(const std::array<float, W* H>& elems)
			//: _elems(elems)
		{
			int i = 0;
			for (auto ie = elems.begin(); ie != elems.end(); ie++, i++) {
				int r = i % width;
				int c = i / width;
				(*this)[{column(c), row(r)}] = *ie;
			}
		}

		float& operator[](const std::pair<column, row>& rc)
		{
			return at(rc);
		}

		float operator[](const std::pair<column, row>& rc) const
		{
			return at(rc);
		}

		tuple<float,width> operator[](const row& r) const {
			tuple<float, width> result;
			for (int x = 0; x < width; x++) {
				result[x] = (*this)[{column(x), row(r)}];
			}
			return result;
		}

		tuple<float,width> operator[](const column& c) const {
			tuple<float, height> result;
			for (int y = 0; y < height; y++) {
				result[y] = (*this)[{column(c), row(y)}];
			}
			return result;
		}

		bool operator==(const matrix_type& other) const
		{
			for (auto ia = _elems.begin(), ib = other._elems.begin(); 
				ia != _elems.end(); ia++, ib++) 
			{
				if (!equals(*ia,*ib)) {
					return false;
				}
			}
			return true;
		}

		bool operator!=(const matrix_type& other) const 
		{
			for (auto ia = _elems.begin(), ib = other._elems.begin();
				ia != _elems.end(); ia++, ib++)
			{
				if (equals(*ia,*ib)) {
					return false;
				}
			}
			return true;
		}

		matrix<H, W> operator* (const matrix_type& other) const
		{
			matrix<H, W> result;
			for (int y = 0; y < height; y++) {
				auto r = other[row(y)];
				for (int x = 0; x < width; x++) {
					auto c = (*this)[column(x)];
					result[{column(x), row(y)}] = (r * c).sum();
				}
			}
			return result;
		}

		tuple<float,height> operator* (const tuple<float,height>& other) const
		{
			tuple<float, height> result;
			for (int y = 0; y < height; y++) {
				auto r = (*this)[column(y)];
				result[y] = (r * other).sum();
			}
			return result;
		}

		static matrix_type identity() {
			matrix_type m;
			for (int i = 0; i < std::min(width, height); i++) {
				m[{column(i), row(i)}] = 1;
			}
			return m;
		}

		matrix<H, W> transpose() const {
			matrix<H, W> result;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					result[{column(y), row(x)}] = (*this)[{column(x), row(y)}];
				}
			}
			return result;
		}

		matrix<W - 1, H - 1> submatrix(int c, int r) const
		{
			matrix<W - 1, H - 1> result;

			for (int sy = 0, dy = 0; sy < height; sy++) {
				if (sy == r) { continue; }
				for (int sx = 0, dx = 0; sx < width; sx++) {
					if (sx == c) {
						continue;
					}
						result[{column(dx), row(dy)}] = (*this)[{column(sx), row(sy)}];
						dx++;
				}
				dy++;
			}
			return result;
		}

		float minor(int c, int r) const {
			return submatrix(c, r).determinant();
		}

		float cofactor(int c, int r) const 
		{
			float m = minor(c, r);
			if ((c + r) % 2) {
				return -m;
			}
			return m;
		}

		matrix_type invert() const {
			float d = determinant();
			if (d == 0) {
				throw std::runtime_error("matrix is not invertable");
			}
			matrix_type inverted;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					inverted[{column(x), row(y)}] = cofactor(x, y) / d;
				}
			}
			return inverted.transpose();
		}

		float determinant() const;
	};

	using matrix2x2 = matrix<2,2>;
	using matrix3x3 = matrix<3,3>;
	using matrix4x4 = matrix<4,4>;

	template<int W, int H>
	inline float matrix<W, H>::determinant() const
	{
		float d = 0;
		auto r = (*this)[column(0)];
		for (size_t i = 0; i < r.size; i++) {
			float ri = r[i];
			float cf = cofactor(0,i);
			d += ri * cf;
		}
		return d;
	}

	template<>
	inline float matrix<2,2>::determinant() const
	{
		return ((*this)[{column(0), row(0)}] * (*this)[{column(1), row(1)}]) - 
			((*this)[{column(1), row(0)}] * (*this)[{column(0), row(1)}]);
	}

} 