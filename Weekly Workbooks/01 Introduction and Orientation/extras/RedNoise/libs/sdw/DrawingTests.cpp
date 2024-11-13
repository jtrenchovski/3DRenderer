#include <algorithm>
#include <sstream>
#include "Utils.h"
#include "CanvasTriangle.h"
#include "DrawingWindow.h"
#include "vector"
#include "glm/glm.hpp"
#include "CanvasPoint.h"
#include "Colour.h"
#include "TextureMap.h"
#include "iostream"
#include "unordered_map"

using namespace std;

void drawGreyScaleGradient(DrawingWindow &window){
	window.clearPixels();
		std::vector<float> v = interpolateSingleFloats(255, 0, window.width);
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			float greyValue = v[x];
			uint32_t colour = (255 << 24) + (int(greyValue) << 16) + (int(greyValue) << 8) + int(greyValue);
			window.setPixelColour(x, y, colour);
		}
	}
}

void draw2DCoulourInterpolation(DrawingWindow &window){
	window.clearPixels();
	glm::vec3 topLeft(255, 0, 0);        // red 
	glm::vec3 topRight(0, 0, 255);       // blue 
	glm::vec3 bottomRight(0, 255, 0);    // green 
	glm::vec3 bottomLeft(255, 255, 0);   // yellow

	std::vector<glm::vec3> leftColumn = interpolateThreeElementVectors(topLeft, bottomLeft, window.height);
	std::vector<glm::vec3> rightColumn = interpolateThreeElementVectors(topRight, bottomRight, window.height);

	for (size_t y = 0; y < window.height; y++) {
		glm::vec3 leftMostColour = leftColumn[y];
		glm::vec3 rightMostColour = rightColumn[y];
		// Getting the colours between the left most and right most colours
		// This way we actually get the whole row
		std::vector<glm::vec3> rowVector = interpolateThreeElementVectors(leftMostColour, rightMostColour, window.width);
		for (size_t x = 0; x < window.width; x++) {
			uint32_t colour = (255 << 24) + (int(rowVector[x][0]) << 16) + (int(rowVector[x][1]) << 8) + int(rowVector[x][2]);
			window.setPixelColour(x, y, colour);
		}
	}
}