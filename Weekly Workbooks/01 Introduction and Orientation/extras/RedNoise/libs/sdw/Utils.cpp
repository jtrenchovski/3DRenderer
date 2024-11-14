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

#define WIDTH 320
#define HEIGHT 240

CanvasTriangle getRandomTriangle(){
	CanvasPoint v0 = CanvasPoint((rand() + 50) % 319, rand() % 239);
	CanvasPoint v1 = CanvasPoint(rand() % 319, rand() % 239);
	CanvasPoint v2 = CanvasPoint(rand() % 319, rand() % 239);
	CanvasTriangle triangle = CanvasTriangle(v0, v1, v2);
	return triangle;
}

std::vector<std::string> split(const std::string &line, char delimiter) {
	auto haystack = line;
	std::vector<std::string> tokens;
	size_t pos;
	while ((pos = haystack.find(delimiter)) != std::string::npos) {
		tokens.push_back(haystack.substr(0, pos));
		haystack.erase(0, pos + 1);
	}
	// Push the remaining chars onto the vector
	tokens.push_back(haystack);
	return tokens;
}

// in fillUpper fucntion this is passed to - from + 1 so it is deviding
// by to - from
std::vector<float> interpolateSingleFloats(float from, float to, float numberOfValues){
	// Calculating the adding value - it's devided by n-1 because 
	// we have v[0] that is frmo and we need to add n-1 add values to get to n numbers
	float addValue = (to - from)/(numberOfValues-1);
	std::vector<float> v;
	for(int i = 0; i < numberOfValues; i++){
		v.push_back(from + i*addValue);
	}
	return v;
}

std::vector<glm::vec3> interpolateThreeElementVectors(glm::vec3 from, glm::vec3 to, float numberOfValues){
	std::vector<glm::vec3> vs(numberOfValues);
	// Calculating the adding value for every dimension
	float addValue1 = (to[0] - from[0])/(numberOfValues-1);
	float addValue2 = (to[1] - from[1])/(numberOfValues-1);
	float addValue3 = (to[2] - from[2])/(numberOfValues-1);

	for(int i = 0; i < numberOfValues; i++){
		glm::vec3 v(from[0] + addValue1*i, from[1] + addValue2*i, from[2] + addValue3*i);
		vs[i] = v;
	}
	return vs;
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

std::vector<CanvasPoint> getLine(CanvasPoint from, CanvasPoint to, bool is3D){
	// This is actually the numberOfValues-1 
	float diffX = to.x - from.x;
	float diffY = to.y - from.y;
	float diffDepth = to.depth - from.depth;

	// We taking the bigger one of the 2 differences so there are no brakes in our line
	// Number of steps is how much times do we need to add the stepSize to reach to.x
	float numberOfSteps = std::max(abs(diffX), abs(diffY));
	// For a more precise depth we get the difference in the z coordinate and 
	// later we add the amount of steps we need. So for example if we have 20 coordinates
	// but numberOfSteps is 3 we add 6 of the steps. If we devided it by 3 we would 
	// have unaccurate
	float numberOfStepsDepth = std::max(numberOfSteps, abs(diffDepth));
	float stepSizeX = diffX/numberOfSteps;
	float stepSizeY = diffY/numberOfSteps;
	float stepSizeDepth = diffDepth/numberOfSteps;

	std::vector<CanvasPoint> line;
	if(numberOfSteps == 0){
		line.push_back(from);
		return line;
	}

	line.push_back(from);
	// We need to add stepSize numberOfSteps times to get to to.x
	for(int i=1; i < numberOfSteps; i++){
		float x = from.x + (stepSizeX*i);
		float y = from.y + (stepSizeY*i);
		float depth = from.depth + (stepSizeDepth*i);
		line.push_back(CanvasPoint(round(x), round(y), depth));
	}
	line.push_back(to);

	return line;
}

// Correct 100%
void drawLine1(DrawingWindow &window, CanvasPoint from, CanvasPoint to, Colour colour){
	// This is actually the numberOfValues-1 
	float diffX = to.x - from.x;
	float diffY = to.y - from.y;

	// // We taking the bigger one of the 2 differences so there are no brakes in our line
	// // Number of steps is how much times do we need to add the stepSize to reach to.x
	float numberOfSteps = std::max(abs(diffX), abs(diffY));
	float stepSizeX = diffX/numberOfSteps;
	float stepSizeY = diffY/numberOfSteps;

	// We need to add stepSize numberOfSteps times to get to to.x
	for(int i=0; i <= numberOfSteps; i++){
		float x = from.x + (stepSizeX*i);
		float y = from.y + (stepSizeY*i);
		uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
		window.setPixelColour(round(x), round(y), colourInt);
	}
}

// correct 100%
void drawLine1(DrawingWindow &window, CanvasPoint from, CanvasPoint to, Colour colour, std::vector<std::vector<float>> &depthMatrix){
	// This is actually the numberOfValues-1 
	float diffX = to.x - from.x;
	float diffY = to.y - from.y;
	float diffDepth = to.depth - from.depth;

	// We taking the bigger one of the 2 differences so there are no brakes in our line
	// Number of steps is how much times do we need to add the stepSize to reach to.x
	float numberOfSteps = std::max(abs(diffX), abs(diffY));
	float stepSizeX = diffX/numberOfSteps;
	float stepSizeY = diffY/numberOfSteps;
	float stepSizeDepth = diffDepth/numberOfSteps;

	uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);

	// We need to add stepSize numberOfSteps times to get to to.x
	for(int i=0; i <= numberOfSteps; i++){
		float x = round(from.x + (stepSizeX*i));
		float y = round(from.y + (stepSizeY*i));
		float depth = from.depth + (stepSizeDepth*i);
		if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
			if(1/depth > depthMatrix[y][x]){
				window.setPixelColour(x, y, colourInt);
				depthMatrix[y][x] = 1/depth;
			} 
		}
	}
}

void DrawTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour){
	drawLine1(window, triangle.v0(), triangle.v1(), colour);
	drawLine1(window, triangle.v1(), triangle.v2(), colour);
	drawLine1(window, triangle.v0(), triangle.v2(), colour);
}

void DrawTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix){
	drawLine1(window, triangle.v0(), triangle.v1(), colour, depthMatrix);
	drawLine1(window, triangle.v1(), triangle.v2(), colour, depthMatrix);
	drawLine1(window, triangle.v0(), triangle.v2(), colour, depthMatrix);
}

// I tried drawing the upper and bottom triangle and the 
// split was CORRECT
CanvasPoint findLeftPoint(CanvasTriangle &triangle){
	CanvasPoint top = triangle.v0();
	CanvasPoint right = triangle.v1();
	CanvasPoint bottom = triangle.v2();
	float leftPointY = right.y;
	float proportionBigTriangle = abs(top.x - bottom.x)/abs(top.y - bottom.y);
	float verticalVerticeSmall = abs(top.y - leftPointY);
	float horizontalVerticeSmall = proportionBigTriangle * verticalVerticeSmall;
	float leftPointX;
	float leftPointDepth;
	float depth = (bottom.depth - top.depth)
					*(top.y - leftPointY)
					/(bottom.y - top.y);
	if(top.x > bottom.x){
		leftPointX = top.x - horizontalVerticeSmall;
	} else {
		leftPointX = top.x + horizontalVerticeSmall;
	}
	if(bottom.depth < top.depth) leftPointDepth = top.depth - depth;
	else leftPointDepth = bottom.depth + depth;
	return CanvasPoint(leftPointX, leftPointY, leftPointDepth);

	// CanvasPoint right;
	// float xDiff = bottom.x - top.x;
	// float yDiff = bottom.y - top.y;
	// float gradient = yDiff/xDiff;
	// float zDiff = bottom.depth - top.depth;
	

	// float midDif_Y = top.y - left.y;
	// float midDif_X = abs(midDif_Y / gradient);

	// float depth = zDiff*(midDif_Y/yDiff);

	// right.y = top.y - midDif_Y;
	// if (bottom.x < top.x){
	// 	right.x = top.x - midDif_X;
	// 	if(bottom.depth < top.depth) right.depth = top.depth - depth ;
	// 	else right.depth = top.depth + depth ;
	// } else{
	// 	right.x = top.x + midDif_X;
	// 	if(bottom.depth < top.depth) right.depth = top.depth - depth ;
	// 	else right.depth = top.depth + depth ;
	// }
	// return right;
}

std::vector<ModelTriangle> readOBJfile(string filename, float scale, std::unordered_map<std::string, Colour> &colourHashMap){
	std::string line;
	ifstream fileStream(filename);
	std::vector<ModelTriangle> modelTriangles;
	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(0, 0, 0));
	Colour colour;
	if(fileStream.is_open()){
		while(getline(fileStream, line)){
			std::vector<std::string> lineSplit = split(line, ' ');
			// usemtl means use the colour for this triangle
			if(lineSplit[0] == "usemtl"){
				colour = colourHashMap[lineSplit[1]];
			// v is vector
			}else if(lineSplit[0] == "v"){
				vertices.push_back(glm::vec3(scale*std::stof(lineSplit[1]), scale*std::stof(lineSplit[2]), scale*std::stof(lineSplit[3])));
			// f is facet or triangle
			} else if(lineSplit[0] == "f"){
				std::vector<int> vIndex;
				for (int i=1; i<4; i++){
					vIndex.push_back(std::stoi(split(lineSplit[i], '/').at(0)));
				}
				ModelTriangle temp = ModelTriangle(vertices[vIndex[0]], vertices[vIndex[1]], vertices[vIndex[2]], colour);
				modelTriangles.push_back(temp);
			} else{
				continue;
			}
		}
	}else{
		cout << "Failed to open file." << endl;
	}

	fileStream.close();
	return modelTriangles;
}

unordered_map<std::string, Colour> readMTLfile(std::string filename){
	std::ifstream fileStream(filename);
	std::unordered_map<std::string, Colour> colourHashMap;

	std::string name;
	std::string line;
	if(fileStream.is_open()){
		while(std::getline(fileStream, line)){
			std::vector<std::string> lineSplit = split(line, ' ');
			if(lineSplit[0] == "newmtl"){
				name = lineSplit[1];
			}else if(lineSplit[0] == "Kd"){
				// The 
				Colour colour = Colour(name, 255 * std::stof(lineSplit[1]), 255 * std::stof(lineSplit[2]), 255 * std::stof(lineSplit[3]));
				colourHashMap[name] = colour;
			}
		}
	}else{
		cout<<"Failed to open file: "<< filename << endl;
	}

	fileStream.close();
	return colourHashMap;
}

// void drawLine(DrawingWindow &window, CanvasPoint from, CanvasPoint to, Colour colour){
// 	std::vector<CanvasPoint> line = getLine(from, to);
// 	for(int i=0; i<line.size(); i++){
// 		int x = line[i].x;
// 		int y = line[i].y;
// 		uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
// 		window.setPixelColour(x, y, colourInt);
// 	}
// }