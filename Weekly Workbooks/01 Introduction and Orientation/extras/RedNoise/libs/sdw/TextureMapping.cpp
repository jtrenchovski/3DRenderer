#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "CanvasTriangle.h"
#include "DrawingWindow.h"
#include "vector"
#include "glm/glm.hpp"
#include "CanvasPoint.h"
#include "Colour.h"
#include "TextureMap.h"
#include "iostream"
#include "unordered_map"
#include "ModelTriangle.h"
#include "Utils.h"

using namespace std;

uint32_t getIndex(size_t width, int x, int y){
	return width*y + x;
}

// Correct 100%
std::vector<CanvasPoint> getLine(CanvasPoint from, CanvasPoint to){
	// This is actually the numberOfValues-1 
	float diffX = to.x - from.x;
	float diffY = to.y - from.y;

	// We taking the bigger one of the 2 differences so there are no brakes in our line
	// Number of steps is how much times do we need to add the stepSize to reach to.x
	float numberOfSteps = std::max(abs(diffX), abs(diffY));
	float stepSizeX = diffX/numberOfSteps;
	float stepSizeY = diffY/numberOfSteps;

	std::vector<CanvasPoint> line;

	// We need to add stepSize numberOfSteps times to get to to.x
	for(int i=0; i <= numberOfSteps; i++){
		float x = from.x + (stepSizeX*i);
		float y = from.y + (stepSizeY*i);
		line.push_back(CanvasPoint(round(x), round(y)));
	}

	return line;
}

std::vector<CanvasPoint> interpolateTwoCanvasPoints(CanvasPoint from, CanvasPoint to, float numberOfValues){
	std::vector<CanvasPoint> points(numberOfValues);
	// Calculating the adding value for every dimension
	float addValue1 = (to.x - from.x)/(numberOfValues-1);
	float addValue2 = (to.y - from.y)/(numberOfValues-1);

	for(int i = 0; i < numberOfValues; i++){
		CanvasPoint p = CanvasPoint(from.x + addValue1*i, from.y + addValue2*i);
		points[i] = p;
	}
	return points;
}

// Given the proportion of where the point is on the line in the target triangle, get the same point with the same
// proportion but in the source triangle(the triangle with texture)
std::vector<CanvasPoint> getSourceLine(CanvasTriangle triangle, float proportion, int numberOfPoints){
	std::vector<CanvasPoint> v0v1Vertice = getLine(triangle.v0(), triangle.v1());
	std::vector<CanvasPoint> v0v2Vertice = getLine(triangle.v0(), triangle.v2());

	int index1 = round(v0v1Vertice.size()*proportion/(1+proportion));
	CanvasPoint p1 = v0v1Vertice[index1];
	int index2 = round(v0v2Vertice.size()*proportion/(1+proportion));
	CanvasPoint p2 = v0v2Vertice[index2];

	return interpolateTwoCanvasPoints(p1, p2, numberOfPoints);
}

void textureMapUpperTriangle(DrawingWindow &window, CanvasTriangle &triangle, CanvasTriangle sourceTriangle, TextureMap textureMap){
	CanvasPoint top = triangle.v0();
	// Get the points on the left and right vertices
	std::vector<float> leftVertice = interpolateSingleFloats(top.x, triangle.v1().x, triangle.v1().y - top.y + 1);
	std::vector<float> rightVertice = interpolateSingleFloats(top.x, triangle.v2().x, triangle.v2().y - top.y + 1);

	for(int y = top.y; y <= triangle.v1().y; y++){
		float proportion = (y-top.y)/(triangle.v1().y - y + 1);
		// This says on which row we draw the line
		int row = y - top.y;
		// This are the coordinates of the left and right point of the line that we need to draw
		int startX = leftVertice[row];
		int endX = rightVertice[row];

		if(startX > endX) std::swap(startX, endX);

		// Find what pixels should we draw
		int numberOfPoints = endX - startX;
		std::vector<CanvasPoint> points = getSourceLine(sourceTriangle, proportion, numberOfPoints);

		for(int x = startX + 1; x <= endX; x++){
			// counter counts the elements from point(the row that is in the source texture map)
			int counter = x - (startX + 1);
			CanvasPoint p = points[counter];
			std::vector<u_int32_t> pixels = textureMap.pixels;
			window.setPixelColour(x, y, pixels[getIndex(textureMap.width, p.x, p.y)]);
		}
	}
}

void textureMapBottomTriangle(DrawingWindow &window, CanvasTriangle triangle, CanvasTriangle sourceTriangle, TextureMap textureMap){
	// Get the points on the left and right vertices
	std::vector<float> leftVertice = interpolateSingleFloats(triangle.v2().x, triangle.v1().x, triangle.v2().y - triangle.v1().y + 1);
	std::vector<float> rightVertice = interpolateSingleFloats(triangle.v2().x, triangle.v0().x, triangle.v2().y - triangle.v0().y+1);

	// NE RABOTI SO V0
	for(int y = triangle.v2().y; y >= triangle.v1().y; y--){
		float proportion = (triangle.v2().y - y)/(y - triangle.v1().y + 1);
		// This says on which row we draw the line
		int row = triangle.v2().y - y;
		// This are the coordinates of the left and right point of the line that we need to draw
		int startX = leftVertice[row];
		int endX = rightVertice[row];

		if(startX > endX) std::swap(startX, endX);
		int numberOfPoints = endX - startX;
		CanvasTriangle newTriangle = CanvasTriangle(sourceTriangle.v2(), sourceTriangle.v0(), sourceTriangle.v1());
		std::vector<CanvasPoint> points = getSourceLine(newTriangle, proportion, numberOfPoints);

		for(int x = startX + 1; x <= endX; x++){
			int counter = x - (startX + 1);
			CanvasPoint p = points[counter];
			std::vector<u_int32_t> pixels = textureMap.pixels;
			window.setPixelColour(x, y, pixels[getIndex(textureMap.width, p.x, p.y)]);
		}
	}
}

void textureMapping(DrawingWindow &window){
	TextureMap textureMap = TextureMap("texture.ppm");

	// Both triangles are sorted by y coordinate
	// Target triangle
	CanvasPoint v0 = CanvasPoint(160, 10);
	CanvasPoint v1 = CanvasPoint(10, 150);
	CanvasPoint v2 = CanvasPoint(300, 230);
	CanvasTriangle targetTriangle = CanvasTriangle(v0, v1, v2);

	// Source triangle
	CanvasPoint v0S = CanvasPoint(195, 5);
	CanvasPoint v1S = CanvasPoint(65, 330);
	CanvasPoint v2S = CanvasPoint(395, 380);
	
	CanvasTriangle sourceTriangle = CanvasTriangle(v0S, v1S, v2S);

	// Put it as a global
	Colour white = Colour(255, 255, 255);
	
	CanvasPoint leftPoint = findLeftPoint(targetTriangle);
	// Proportion on where the point is on the top-bottom vertice
	float leftPointProportion = (leftPoint.y - v0.y)/(v2.y - leftPoint.y);

	std::vector<CanvasPoint> v0v2Vertice = getLine(v0S, v2S);

	int index = round(v0v2Vertice.size()*leftPointProportion/(1+leftPointProportion));
	CanvasPoint leftPointS = v0v2Vertice[index];
	drawLine1(window, leftPoint, v1, white);

	CanvasTriangle upperSourceTriangle = CanvasTriangle(v0S, v1S, leftPointS);
	CanvasTriangle bottomSourceTriangle = CanvasTriangle(v1S, leftPointS, v2S);

	textureMapUpperTriangle(window, targetTriangle, upperSourceTriangle, textureMap);
	textureMapBottomTriangle(window, targetTriangle, bottomSourceTriangle, textureMap);

	// DrawTriangle(window, targetTriangle, white);
}
