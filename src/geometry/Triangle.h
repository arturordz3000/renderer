#pragma once

#include "../math/point.h"
#include "../vendor/tga/tgaimage.h"
#include <vector>

using namespace std;

namespace Renderer
{
	void drawTriangle(Point& point1, Point& point2, Point& point3, TGAImage& image, TGAColor color);
	void drawTriangle(vector<Point> triangle, TGAImage& image, TGAColor color);
	void drawTriangle(vector<Vector3<float>> triangle, float* zBuffer, TGAImage& image, TGAColor color);
	void drawTriangle(vector<Vector3<float>> triangle, float* zBuffer, TGAImage& image, TGAImage& texture, vector<Vector2<float>> uv, float lightIntensity);
}

