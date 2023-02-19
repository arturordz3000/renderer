#pragma once


namespace Renderer 
{
	template<class T> class Vector3
	{
		T x;
		T y;
		T z;

		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
	};
}