#include "barycentric.h"
#include <iostream>

using namespace std;

namespace Renderer 
{
	const Vector3<float> INVALID_VECTOR(-1, 1, 1);

	Vector3<float> computeBarycentricVector(const std::vector<Point> &triangleVertices, const Point &point)
	{
		/**
		* In a triangle, we have points A, B and C. In order to detect that a point (P) resides in a triangle ABC,
		* we first need to compute the vectors AB, AC and PA
		**/
		Point a = triangleVertices[0];
		Point b = triangleVertices[1];
		Point c = triangleVertices[2];
		Point ab = b - a;
		Point ac = c - a;
		Point pa = a - point;

		/**
		* The Barycentric coordinates say that if a point (P) is inside a triangle,
		* it has to comply with the following formula:
		*	P = A + uAB + vAC
		* where u and v are the weights of each of the vectors and they have values between 0 and 1
		* 
		* The formula above can also be stated as follows:
		*	uAB + vAC + PA = 0
		* If we re-write this formula using components x and y, we get the following equation system:
		*	uABx + vACx + PAx = 0
		*	uABy + vACy + PAy = 0
		* 
		* The system above can be re-written in matrix form:
		*	[u, v, 1][ABx, ACx, PAx] = 0
		*	[u, v, 1][ABy, ACy, PAy] = 0
		* 
		* The two matrices above tell us that, in order to find if a point P resides in a triangle,
		* we need to find u and v values that are orthogonal to (ABx, ACx, PAx) and (ABy, ACy, PAy)
		* at the same time, so we can easily find those values by computing the cross product of (ABx, ACx, PAx) and (ABy, ACy, PAy).
		*/
		Vector3<float> xComponents(ab.x, ac.x, pa.x);
		Vector3<float> yComponents(ab.y, ac.y, pa.y);
		auto result = Vector3<float>::CrossProduct(xComponents, yComponents);

		/**
		* If the abs(z) component of the cross product is less than 1,
		* that means that is most likely 0.
		* Basically, we have a degenerate triangle, so we return an invalid vector (a vector with one negative component).
		* A degenerate triangle has all three of its vertices lying on the same straight line, so the triangle is squashed completely flat:
		* https://undergroundmathematics.org/glossary/degenerate#:~:text=A%20degenerate%20triangle%20has%20all,triangle%20is%20squashed%20completely%20flat.
		*/
		if (abs(result.z) < 1)
		{
			return INVALID_VECTOR;
		}

		/**
		* The original Barycentric coordinates formula is:
		*	P = A + uB + vC
		* which can be converted to:
		*	P = uA + vAB + wAC
		* However, we usually like to represent the equation as:
		*	P = wA + uAB + vAC
		* That's because if we expand the the original equation we get:
		*	P = A + uAB + vAC
		*	P = A + u(B-A) + v(C-A)
		*	P = A + uB - uA + vC - vA
		* Let's reorder the elements:
		*	P = A - uA - vA + uB + vC
		* and we factorize it:
		*	P = (1-u-v)A + uB + vC
		* 1-u-v comes from the fact that the sum of u + v + w = 1, which means that u + v <= 1,
		* so w can be converted to 1 - u - v. A more detailed explanation can be found here: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates.html
		* Here, we return the point now that we have the u and v values.
		* u and v are the x and y components of the cross product,
		* and they are... divided by z in order to be "normalized" from 0 to 1? Not sure about this.
		*/
		return Vector3<float>(1.0f - (result.x + result.y) / result.z, result.y / result.z, result.x / result.z);
	}

	// Same as the above, but taking 3d coordinates
	Vector3<float> computeBarycentricVector(const std::vector<Vector3<float>>& triangleVertices, Vector3<float>& point)
	{
		Vector3<float> a = triangleVertices[0];
		Vector3<float> b = triangleVertices[1];
		Vector3<float> c = triangleVertices[2];
		Vector3<float> ac = c - a;
		Vector3<float> ab = b - a;
		Vector3<float> pa = a - point;

		Vector3<float> xComponents(ac.x, ab.x, pa.x);
		Vector3<float> yComponents(ac.y, ab.y, pa.y);
		auto result = Vector3<float>::CrossProduct(xComponents, yComponents);

		//cout << "bary : " << result.x << " " << result.y << " " << result.z << endl;

		if (abs(result.z) > 1e-2)
		{
			return Vector3<float>(1.0f - (result.x + result.y) / result.z, result.y / result.z, result.x / result.z);
		}

		// Getting a value less than 0 means that the point lies outside of the triangle.
		return INVALID_VECTOR;
	}
}