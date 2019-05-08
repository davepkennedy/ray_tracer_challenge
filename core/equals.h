#pragma once

#include <string>

static const float _eps = 0.00001f;

template <typename T>
inline bool equals(const T& a, const T& b)
{
	return a == b;
}

template <>
inline bool equals(const float& a, const float& b) {
	float d = (a > b ? a - b : b - a);
	bool eq = d < _eps;
	return eq;
}

template<>
inline bool equals(const std::string& a, const std::string& b) {
	return a.compare(b) == 0;
}