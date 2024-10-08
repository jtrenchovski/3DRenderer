#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <Colour.h>
#include <TextureMap.h>

#define WIDTH 320
#define HEIGHT 240

void DrawTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour);
void DrawFullTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour);

void draw(DrawingWindow &window) {
	window.clearPixels();
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			float red = rand() % 256;
			float green = 0.0;
			float blue = 0.0;
			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
			window.setPixelColour(x, y, colour);
		}
	}
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
		else if (event.key.keysym.sym == SDLK_u){
			Colour colour = Colour(rand() % 256, rand() % 256, rand() % 256);
			CanvasPoint v0 = CanvasPoint(rand() % 319, rand() % 239);
			CanvasPoint v1 = CanvasPoint(rand() % 319, rand() % 239);
			CanvasPoint v2 = CanvasPoint(rand() % 319, rand() % 239);
			CanvasTriangle triangle = CanvasTriangle(v0, v1, v2);
			DrawTriangle(window, triangle, colour);
		} else if (event.key.keysym.sym == SDLK_f){
			Colour colour = Colour(rand() % 256, rand() % 256, rand() % 256);
			CanvasPoint v0 = CanvasPoint(rand() % 319, rand() % 239);
			CanvasPoint v1 = CanvasPoint(rand() % 319, rand() % 239);
			CanvasPoint v2 = CanvasPoint(rand() % 319, rand() % 239);
			CanvasTriangle triangle = CanvasTriangle(v0, v1, v2);
			DrawFullTriangle(window, triangle, colour);
		} 
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	} 
}

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

std::vector<CanvasPoint> getLine(CanvasPoint from, CanvasPoint to){
	// This is actually the numberOfValues-1 
	float diffX = to.x - from.x;
	float diffY = to.y - from.y;

	// We taking the bigger one of the 2 differences so there are no brakes in our line
	// Number of steps is how much times do we need to add the stepSize to reach to.x
	float numberOfSteps = std::max(abs(diffX), abs(diffY));
	float stepSizeX = diffX/numberOfSteps;
	float stepSizeY = diffY/numberOfSteps;

	std::vector<CanvasPoint> line(numberOfSteps+1);

	// We need to add stepSize numberOfSteps times to get to to.x
	for(int i=0; i <= numberOfSteps; i++){
		float x = from.x + (stepSizeX*i);
		float y = from.y + (stepSizeY*i);
		line[i] = CanvasPoint(round(x), round(y));
	}

	return line;
}

void drawLine(DrawingWindow &window, CanvasPoint from, CanvasPoint to, Colour colour){
	std::vector<CanvasPoint> line = getLine(from, to);
	for(int i=0; i<line.size(); i++){
		int x = line[i].x;
		int y = line[i].y;
		uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
		window.setPixelColour(x, y, colourInt);
	}
}

std::vector<CanvasPoint> interpolateTwoCanvasPoints(CanvasPoint from, CanvasPoint to, float numberOfValues){
	std::vector<CanvasPoint> points(numberOfValues);
	// Calculating the adding value for every dimension
	float addValue1 = (to.x - from.x)/(numberOfValues-1);
	float addValue2 = (to.y - from.y)/(numberOfValues-1);

	for(int i = 0; i < numberOfValues; i++){
		CanvasPoint p = CanvasPoint(round(from.x + addValue1*i), round(from.y + addValue2*i));
		points[i] = p;
	}
	return points;
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

// The lines are not symetrical, should it be that way?
void drawLineTest(DrawingWindow &window){
	window.clearPixels();
	CanvasPoint topLeft = CanvasPoint(0, 0);
	CanvasPoint topRight = CanvasPoint(319, 0);
	CanvasPoint midMid = CanvasPoint(160, 120);
	CanvasPoint midTop = CanvasPoint(160, 0);
	CanvasPoint midBot = CanvasPoint(160, 239);
	CanvasPoint midThird = CanvasPoint(107, 120);
	CanvasPoint mid2Thirds = CanvasPoint(214, 120);

	Colour white = Colour(255, 255, 255);

	drawLine(window, topLeft, midMid, white);
	drawLine(window, topRight, midMid, white);
	drawLine(window, midTop, midBot, white);
	drawLine(window, midThird, mid2Thirds, white);
}

void DrawTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour){
	drawLine(window, triangle.v0(), triangle.v1(), colour);
	drawLine(window, triangle.v1(), triangle.v2(), colour);
	drawLine(window, triangle.v0(), triangle.v2(), colour);
}

void FillUpperTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour){
	// Get the points on the left and right vertices
	std::vector<float> leftVertice = interpolateSingleFloats(triangle.v0().x, triangle.v1().x, triangle.v1().y - triangle.v0().y + 1);
	std::vector<float> rightVertice = interpolateSingleFloats(triangle.v0().x, triangle.v2().x, triangle.v2().y - triangle.v0().y + 1);

	for(int y = triangle.v0().y; y <= triangle.v1().y; y++){
		// This says on which row we draw the line
		int row
		 = y - triangle.v0().y;
		// This are the coordinates of the left and right point of the line that we need to draw
		int startX = leftVertice[row
		];
		int endX = rightVertice[row
		];

		if(startX > endX) std::swap(startX, endX);

		for(int x = startX + 1; x <= endX; x++){
			uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
			window.setPixelColour(x, y, colourInt);
		}
	}
}

void FillBottomTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour){
	// Get the points on the left and right vertices
	std::vector<float> leftVertice = interpolateSingleFloats(triangle.v2().x, triangle.v1().x, triangle.v2().y - triangle.v1().y + 1);
	std::vector<float> rightVertice = interpolateSingleFloats(triangle.v2().x, triangle.v0().x, triangle.v2().y - triangle.v0().y+1);

	for(int y = triangle.v2().y; y >= triangle.v0().y; y--){
		// This says on which row we draw the line
		int row
		 = triangle.v2().y - y;
		// This are the coordinates of the left and right point of the line that we need to draw
		int startX = leftVertice[row
		];
		int endX = rightVertice[row
		];

		if(startX > endX) std::swap(startX, endX);

		for(int x = startX + 1; x <= endX; x++){
			uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
			window.setPixelColour(x, y, colourInt);
		}
	}
}

CanvasPoint findLeftPoint(CanvasTriangle triangle){
	float leftPointY = triangle.v1().y;
	float proportionBigTriangle = abs(triangle.v0().x - triangle.v2().x)/abs(triangle.v0().y - triangle.v2().y);
	float verticalVerticeSmall = abs(triangle.v0().y - leftPointY);
	float horizontalVerticeSmall = proportionBigTriangle * verticalVerticeSmall;
	float leftPointX;
	if(triangle.v0().x > triangle.v2().x){
		leftPointX = triangle.v0().x - horizontalVerticeSmall;
	} else {
		leftPointX = triangle.v0().x + horizontalVerticeSmall;
	}
	return CanvasPoint(leftPointX, leftPointY);
}

void DrawFullTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour){
	Colour white = Colour(255, 255, 255);

	if (triangle.v0().y > triangle.v1().y) std::swap(triangle.v0(), triangle.v1());
	if (triangle.v1().y > triangle.v2().y) std::swap(triangle.v1(), triangle.v2());
	if (triangle.v0().y > triangle.v1().y) std::swap(triangle.v0(), triangle.v1());

	CanvasPoint leftPoint = findLeftPoint(triangle);

	CanvasTriangle upperTriangle = CanvasTriangle(triangle.v0(), leftPoint, triangle.v1());
	CanvasTriangle bottomTriangle = CanvasTriangle(triangle.v1(), leftPoint, triangle.v2());

	FillUpperTriangle(window, upperTriangle, colour);
	FillBottomTriangle(window, bottomTriangle, colour);

	DrawTriangle(window, triangle, white);
}

uint32_t getIndex(size_t width, int x, int y){
	return width*y + x;
}

std::vector<CanvasPoint> getSourceLine(CanvasTriangle triangle, float proportion, int numberOfPoints){
	std::vector<CanvasPoint> v0v1Vertice = getLine(triangle.v0(), triangle.v1());
	std::vector<CanvasPoint> v0v2Vertice = getLine(triangle.v0(), triangle.v2());

	int index1 = round(v0v1Vertice.size()*proportion/(1+proportion));
	CanvasPoint p1 = v0v1Vertice[index1];
	int index2 = round(v0v2Vertice.size()*proportion/(1+proportion));
	CanvasPoint p2 = v0v2Vertice[index2];

	return interpolateTwoCanvasPoints(p1, p2, numberOfPoints);
}

void textureMapUpperTriangle(DrawingWindow &window, CanvasTriangle triangle, CanvasTriangle sourceTriangle, TextureMap textureMap){
	// Get the points on the left and right vertices
	std::vector<float> leftVertice = interpolateSingleFloats(triangle.v0().x, triangle.v1().x, triangle.v1().y - triangle.v0().y + 1);
	std::vector<float> rightVertice = interpolateSingleFloats(triangle.v0().x, triangle.v2().x, triangle.v2().y - triangle.v0().y + 1);

	for(int y = triangle.v0().y; y <= triangle.v1().y; y++){
		float proportion = (y-triangle.v0().y)/(triangle.v1().y - y + 1);
		// This says on which row we draw the line
		int row = y - triangle.v0().y;
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
	drawLine(window, leftPoint, v1, white);

	CanvasTriangle upperSourceTriangle = CanvasTriangle(v0S, v1S, leftPointS);
	CanvasTriangle bottomSourceTriangle = CanvasTriangle(v1S, leftPointS, v2S);

	textureMapUpperTriangle(window, targetTriangle, upperSourceTriangle, textureMap);
	textureMapBottomTriangle(window, targetTriangle, bottomSourceTriangle, textureMap);

	DrawTriangle(window, targetTriangle, white);
}

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;

	std::vector<float> result;
	result = interpolateSingleFloats(2.2, 8.5, 7);
	for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
	std::cout << std::endl;

	glm::vec3 from(1.0, 4.0, 9.2);
	glm::vec3 to(4.0, 1.0, 9.8);

	std::vector<glm::vec3> result3D;
	result3D = interpolateThreeElementVectors(from, to, 4);
	for(size_t i=0; i<result3D.size(); i++){
		std::cout << "(";
		for(size_t j=0; j<3; j++){
			std::cout << result3D[i][j] << " ";
		}
		std::cout << ")";
		std::cout << std::endl;
	}

	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		textureMapping(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
