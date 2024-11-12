#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <Colour.h>
#include <TextureMap.h>
#include <iostream>
#include <ModelTriangle.h>
#include <unordered_map>
#include <TextureMapping.h>

using namespace std;

#define WIDTH 320
#define HEIGHT 240
#define WHITE Colour(255, 255, 255)
#define CAMERAPOS glm::vec3(0.0, 0.0, 4.0)

void DrawTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour);
void DrawFullTriangle(DrawingWindow &window, CanvasTriangle &triangle, Colour colour);
std::vector<CanvasPoint> getLine(CanvasPoint from, CanvasPoint to);

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
			CanvasTriangle triangle = getRandomTriangle();
			DrawTriangle(window, triangle, colour);
		} else if (event.key.keysym.sym == SDLK_f){
			Colour colour = Colour(rand() % 256, rand() % 256, rand() % 256);
			CanvasTriangle triangle = getRandomTriangle();
			DrawFullTriangle(window, triangle, colour);
			DrawTriangle(window, triangle, WHITE);
		} 
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
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

	drawLine1(window, topLeft, midMid, white);
	drawLine1(window, topRight, midMid, white);
	drawLine1(window, midTop, midBot, white);
	drawLine1(window, midThird, mid2Thirds, white);
}

// There is not points outside of the edges when 'f' is pressed. With and without the
// drawTriangly function in this function
void FillBottomTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour){
	// Get the points on the left and right vertices
	CanvasPoint bottom = triangle.v2();
	std::vector<float> leftVertice = interpolateSingleFloats(bottom.x, triangle.v1().x, bottom.y - triangle.v1().y + 1);
	std::vector<float> rightVertice = interpolateSingleFloats(bottom.x, triangle.v0().x, bottom.y - triangle.v0().y + 1);

	int i = 0;
	for(int y = bottom.y; y >= triangle.v0().y; y--){
		// This says on which row we draw the line
		// int row = bottom.y - y;
		// This are the coordinates of the left and right point of the line that we need to draw
		int startX = round(leftVertice[i]);
		int endX = round(rightVertice[i]);
		
		if(startX > endX) std::swap(startX, endX);
		// CanvasPoint from = CanvasPoint(startX, y);
		// CanvasPoint to = CanvasPoint(endX, y);
		// drawLine1(window, from, to, colour);
		for(int x = startX; x <= endX; x++){
			uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
			window.setPixelColour(x, y, colourInt);
		}
		i++;
	}
	DrawTriangle(window, triangle, WHITE);
}

// There is not points outside of the edges when 'f' is pressed. With and without the
// drawTriangly function in this function
void FillUpperTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour){
	// Get the points on the left and right vertices
	CanvasPoint top = triangle.v0();
	std::vector<float> leftVertice = interpolateSingleFloats(top.x, triangle.v1().x, triangle.v1().y - top.y + 1);
	std::vector<float> rightVertice = interpolateSingleFloats(top.x, triangle.v2().x, triangle.v2().y - top.y + 1);

	int i = 0;
	for(int y = top.y; y < triangle.v1().y; y++){
		// This says on which row we draw the line
		// int row = y - top.y;
		// This are the coordinates of the left and right point of the line that we need to draw
		int startX = round(leftVertice[i]);
		int endX = round(rightVertice[i]);

		if(startX > endX) std::swap(startX, endX);
		// CanvasPoint from = CanvasPoint(startX, y);
		// CanvasPoint to = CanvasPoint(endX, y);

		// drawLine1(window, from, to, colour);
		i++;
		for(int x = startX; x <= endX; x++){
			uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
			window.setPixelColour(x, y, colourInt);
		}
	}
	// DrawTriangle(window, triangle, WHITE);
}

// With drawTriangle in here no points are outside of the boundary 
void DrawFullTriangle(DrawingWindow &window, CanvasTriangle &triangle, Colour colour){
	if (triangle.v0().y > triangle.v1().y) std::swap(triangle.v0(), triangle.v1());
	if (triangle.v1().y > triangle.v2().y) std::swap(triangle.v1(), triangle.v2());
	if (triangle.v0().y > triangle.v1().y) std::swap(triangle.v0(), triangle.v1());

	CanvasPoint leftPoint = findLeftPoint(triangle);

	CanvasTriangle upperTriangle = CanvasTriangle(triangle.v0(), leftPoint, triangle.v1());
	CanvasTriangle bottomTriangle = CanvasTriangle(triangle.v1(), leftPoint, triangle.v2());

	FillUpperTriangle(window, upperTriangle, colour);
	FillBottomTriangle(window, bottomTriangle, colour);

	DrawTriangle(window, triangle, WHITE);
}

void FillBottomTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix){
	// Get the points on the left and right vertices
	CanvasPoint bottom = triangle.v2();
	std::vector<float> leftVertice = interpolateSingleFloats(bottom.x, triangle.v1().x, bottom.y - triangle.v1().y + 1);
	std::vector<float> rightVertice = interpolateSingleFloats(bottom.x, triangle.v0().x, bottom.y - triangle.v0().y + 1);

	std::vector<float> leftVerticeDepth = interpolateSingleFloats(bottom.depth, triangle.v1().depth, bottom.y - triangle.v1().y + 1);
	std::vector<float> rightVerticeDepth = interpolateSingleFloats(bottom.depth, triangle.v0().depth, bottom.y - triangle.v0().y + 1);

	int i = 0;
	for(int y = bottom.y; y >= triangle.v0().y; y--){
		// This says on which row we draw the line
		// int row = bottom.y - y;
		// This are the coordinates of the left and right point of the line that we need to draw
		float startX = round(leftVertice[i]);
		float endX = round(rightVertice[i]);
		float depthFrom = leftVerticeDepth[i];
		float depthTo = rightVerticeDepth[i];
		
		if(startX > endX){
			std::swap(startX, endX);
			std::swap(depthFrom, depthTo);
		}
		// CanvasPoint from = CanvasPoint(startX, y, depthFrom);
		// CanvasPoint to = CanvasPoint(endX, y, depthTo);
		// drawLine1(window, from, to, colour, depthMatrix);
		// float diffX = endX - startX;
		// float diffY = 0;
		float diffDepth = depthTo - depthFrom;

		// We taking the bigger one of the 2 differences so there are no brakes in our line
		// Number of steps is how much times do we need to add the stepSize to reach to.x
		// float numberOfSteps = std::max(abs(diffX), abs(diffY));
		float stepSizeDepth = diffDepth/(endX - startX);
		int j = 0;
		for(int x = startX; x <= endX; x++){
			uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
			float depth = depthFrom + (stepSizeDepth*j);
			if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
				if(1/depth >= depthMatrix[y][x]){
					window.setPixelColour(x, y, colourInt);
					depthMatrix[y][x] = 1/depth;
				}
        	}
			j++;
		}
		i++;
	}
	DrawTriangle(window, triangle, colour, depthMatrix);
}

void FillUpperTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix){
	// Get the points on the left and right vertices
	CanvasPoint top = triangle.v0();
	// The problem should be here - the interpolation doesn't give the same pixel as the
	// drawLine between. But it is the same logic so the problem can be 
	// if there are 2 white(edge) pixels on the same line. But the function
	// will paint them as well. So it's the interpolation function 
	// Next step: compare them
	std::vector<float> leftVertice = interpolateSingleFloats(top.x, triangle.v1().x, triangle.v1().y - top.y + 1);
	std::vector<float> rightVertice = interpolateSingleFloats(top.x, triangle.v2().x, triangle.v2().y - top.y + 1);

	std::vector<float> leftVerticeDepth = interpolateSingleFloats(top.depth, triangle.v1().depth, triangle.v1().y - top.y + 1);
	std::vector<float> rightVerticeDepth = interpolateSingleFloats(top.depth, triangle.v2().depth, triangle.v2().y - top.y + 1);

	// int i = 0;
	for(int y = top.y; y < triangle.v1().y; y++){
		// This says on which row we draw the line
		int row = y - top.y;
		// This are the coordinates of the left and right point of the line that we need to draw
		float startX = round(leftVertice[row]);
		float endX = round(rightVertice[row]);

		float depthFrom = leftVerticeDepth[row];
		float depthTo = rightVerticeDepth[row];
		
		if(startX > endX){
			std::swap(startX, endX);
			std::swap(depthFrom, depthTo);
		}
		
		// CanvasPoint from = CanvasPoint(startX, y, depthFrom);
		// CanvasPoint to = CanvasPoint(endX, y, depthTo);
		// float diffX = to.x - from.x;
		// float diffY = to.y - from.y;
		// float diffDepth = to.depth - from.depth;

		// We taking the bigger one of the 2 differences so there are no brakes in our line
		// Number of steps is how much times do we need to add the stepSize to reach to.x
		float stepSizeDepth = (depthTo - depthFrom)/(endX - startX);
		int i = 0;
		for(int x = startX; x <= endX; x++){
			uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
			float depth = depthFrom + (stepSizeDepth*i);
			if(1/depth >= depthMatrix[y][x]){
				window.setPixelColour(x, y, colourInt);
				depthMatrix[y][x] = 1/depth;
			}
			i++;
		}
		// drawLine1(window, from, to, colour, depthMatrix);
	}
	// uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
	DrawTriangle(window, triangle, colour, depthMatrix);
}

void DrawFullTriangle(DrawingWindow &window, CanvasTriangle &triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix){
	if (triangle.v0().y > triangle.v1().y) std::swap(triangle.v0(), triangle.v1());
	if (triangle.v1().y > triangle.v2().y) std::swap(triangle.v1(), triangle.v2());
	if (triangle.v0().y > triangle.v1().y) std::swap(triangle.v0(), triangle.v1());

	CanvasPoint leftPoint = findLeftPoint(triangle);

	CanvasTriangle upperTriangle = CanvasTriangle(triangle.v0(), leftPoint, triangle.v1());
	CanvasTriangle bottomTriangle = CanvasTriangle(triangle.v1(), leftPoint, triangle.v2());

	FillUpperTriangle(window, upperTriangle, colour, depthMatrix);
	FillBottomTriangle(window, bottomTriangle, colour, depthMatrix);

	// DrawTriangle(window, triangle, WHITE);
}

// correct for sure
CanvasPoint projectVertexOntoCanvasPoint(glm::vec3 cameraPosition, float focalLength, glm::vec3 vertexPosition){
	glm::vec3 transpositionVector = vertexPosition - cameraPosition;
	
	float u = -150*focalLength*transpositionVector[0]/transpositionVector[2] + WIDTH/2;
	float v = 150*focalLength*transpositionVector[1]/transpositionVector[2] + HEIGHT/2;

	// -transpositionVector[2] is the z which is the depth(how close it is to the camera)
	// we put - because z is already negative 
	return CanvasPoint(u, v, -transpositionVector[2]);
}

// correct 100 %
void render3DModel(DrawingWindow &window, std::vector<ModelTriangle> modelTriangles, glm::vec3 cameraPosition, std::vector<std::vector<float>> &depthMatrix){
	
	for(ModelTriangle triangle3D : modelTriangles){
		std::array<glm::vec3, 3> vertices = triangle3D.vertices;

		// this is the points on the image plane and each one have some depth
		// (how close the projected point is to the image plane)
		CanvasPoint v0 = projectVertexOntoCanvasPoint(cameraPosition, 2.0, vertices[0]);
		CanvasPoint v1 = projectVertexOntoCanvasPoint(cameraPosition, 2.0, vertices[1]);
		CanvasPoint v2 = projectVertexOntoCanvasPoint(cameraPosition, 2.0, vertices[2]);
		
		CanvasTriangle triangle2D = CanvasTriangle(v0, v1, v2);

		// DrawTriangle(window, triangle2D, WHITE);

		// DrawFullTriangle(window, triangle2D, triangle3D.colour);
		DrawFullTriangle(window, triangle2D, triangle3D.colour, depthMatrix);
	}
}


int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;

	// std::vector<float> result;
	// result = interpolateSingleFloats(2.2, 8.5, 7);
	// for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
	// std::cout << std::endl;

	// glm::vec3 from(1.0, 4.0, 9.2);
	// glm::vec3 to(4.0, 1.0, 9.8);

	// std::vector<glm::vec3> result3D;
	// result3D = interpolateThreeElementVectors(from, to, 4);
	// for(size_t i=0; i<result3D.size(); i++){
	// 	std::cout << "(";
	// 	for(size_t j=0; j<3; j++){
	// 		std::cout << result3D[i][j] << " ";
	// 	}
	// 	std::cout << ")";
	// 	std::cout << std::endl;
	// }

	unordered_map<std::string, Colour> colourHashMap = readMTLfile("cornell-box.mtl");
	std::vector<ModelTriangle> modelTriangles = readOBJfile("cornell-box.obj", 0.35, colourHashMap);
	
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		std::vector<std::vector<float>> depthMatrix(HEIGHT, std::vector<float>(WIDTH, 0));
		// drawLineTest(window);
		render3DModel(window, modelTriangles, CAMERAPOS, depthMatrix);
		// drawLineTest(window);
		// textureMapping(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
