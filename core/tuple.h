#pragma once

#include <functional>
#include <utility>
#include <vector>
#include <array>

const int x = 0;
const int y = 1;
const int z = 2;
const int w = 3;

namespace rt {

	template <typename T>
	struct cons {
		using type = T;
		using cons_type = cons<T>;

		type value;
		cons_type* next;

		cons(type v) :value(v), next(nullptr) {}
		~cons() { delete next; }

		void add(type v) {
			if (next) {
				next->add(v);
			}
			else {
				next = new cons(v);
			}
		}

		type& at(size_t idx) {
			if (0 == idx) {
				return value;
			}
			if (next) {
				return next->at(idx - 1);
			}
			throw std::runtime_error("idx out of bounds");
		}

		size_t length() const {
			return 1 + (next ? next->length() : 0);
		}

	};

	template <typename T>
	class const_tuple_iterator {
	public:
		using type = T;
		using this_type = const_tuple_iterator<T>;
	private:
		cons<T>* _it;
	public:

		const_tuple_iterator(cons<T>* it) :_it(it) {}

		type operator*() const {
			return _it->value;
		}

		void operator++ (int) {
			_it = _it->next;

		}
		void operator++ () {
			_it = _it->next;

		}

		bool operator==(const this_type& other) const {
			return _it == other._it;
		}

		bool operator!= (const this_type& other) const {
			return _it != other._it;
		}
	};

	template <typename T, int S>
	class tuple : public std::array<T,S>{
	public:
		using type = T;
		using tuple_type = tuple<T,S>;
		
		static const size_t size = S;


		tuple() {}

		tuple(const std::array<type, size>& elems) : std::array<type,size>(elems) {}

	public:

		bool operator==(const tuple_type& other) const {
			for (auto a = this->begin(), b = other.begin(); a != this->end(); a++, b++) {
				if (*a != *b) {
					return false;
				}
			}
			return true;
		}

		template<typename U>
		tuple<U,S> map(std::function<U(const T&)> fn) const {
			tuple<U,S> ret;
			for (size_t i = 0; i < size; i++) {
				ret[i] = fn((*this)[i]);
			}
			return ret;
		}

		tuple<std::pair<type, type>,S> zip(const tuple_type& other) const {
			tuple<std::pair<type, type>, S> zipped;
			for (int i = 0; i < size; i++) {
				zipped[i] = std::pair<type, type>((*this)[i], other[i]);
			}
			return zipped;
		}

		tuple_type operator-()const {
			return map<type>([](const type & t) {return -t; });
		}

		tuple_type operator+(const tuple_type& other) const {
			return zip(other).map<type>([](const std::pair<type, type> & p) {return p.first + p.second; });
		}

		tuple_type operator += (const tuple_type& other) {
			tuple_type t = (*this) + other;
			(*this) = t;
			return t;
		}

		tuple_type operator-(const tuple_type& other) const {
			return zip(other).map<type>([](const std::pair<type, type> & p) {return p.first - p.second; });
		}

		tuple_type operator*(const tuple_type& other) const {
			return zip(other).map<type>([](const std::pair<type, type> & p) {return p.first* p.second; });
		}

		tuple_type operator/(const tuple_type& other) const {
			return zip(other).map<type>([](const std::pair<type, type> & p) {return p.first / p.second; });
		}

		tuple_type operator*(type v) const {
			return map<type>([v](type in) {return v * in; });
		}

		tuple_type operator/(type v) const {
			return map<type>([v](type in) {return in / v; });
		}

		float magnitude() const {
			float t = 0;
			for (auto v : (*this)) {
				t += v * v;
			}
			return sqrtf(t);
		}

		tuple_type normalize() const {
			float m = magnitude();
			tuple_type ret;
			for (int i = 0; i < size; i++) {
				ret[i] = (*this)[i] / m;
			}
			return ret;
		}

		type sum() const {
			float t = 0;
			for (auto v : (*this)) {
				t += v;
			}
			return t;
		}

		static tuple_type sized(size_t size) {
			tuple_type ret;
			for (size_t i = 0; i < size; i++) {
				ret.add(type());
			}
			return ret;
		}

	};

	template<typename T>
	tuple<T,4> vector(T x, T y, T z) {
		return tuple<T, 4>({ x, y, z, 0 });
	}

	template<typename T>
	tuple<T,4> point(T x, T y, T z) {
		return tuple<T,4>({ x, y, z, 1 });
	}

	template<typename T>
	inline float dot(const tuple<T,4>& a, const tuple<T,4>& b)
	{
		return a[0] * b[0] +
			a[1] * b[1] +
			a[2] * b[2] +
			a[3] * b[3];
	}

	template<typename T>
	tuple<T,4> cross(const tuple<T,4>& a, const tuple<T,4>& b)
	{
		return vector(
			a[1] * b[2] - a[2] * b[1],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - a[1] * b[0]
		);
	}
}