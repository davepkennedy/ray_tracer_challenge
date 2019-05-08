#pragma once

#include <color.h>
#include <tuple.h>

template <>
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString(const rt::color& c)
{
	std::wstringstream ss;
	ss << "color(" <<
		c.red << "," <<
		c.green << "," <<
		c.blue << ")";

	return ss.str().c_str();
}

template <>
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString(const rt::tuple<float,4>& t)
{
	std::wstringstream ss;
	ss << "tuple(" << "," <<
		t[x] << "," <<
		t[y] << "," <<
		t[z] << "," <<
		t[w] << ")";

	return ss.str().c_str();
}
