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

glm::vec3 cameraPosition = glm::vec3(0.0, 0.0, 4.0);
glm::mat3 cameraOrientation = glm::mat3(1.0);

void DrawTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour);
void DrawFullTriangle(DrawingWindow &window, CanvasTriangle &triangle, Colour colour);
std::vector<CanvasPoint> getLine(CanvasPoint from, CanvasPoint to);
glm::vec3 changeCameraPosition(glm::vec3);
glm::mat3 panCamera(glm::mat3 cameraOrientation, float angle);
glm::mat3 tiltCamera(glm::mat3 cameraOrientation, float angle);

void draw(DrawingWindow &window) {
	window.clearPixels();
	// drawLineTest(window);
	// render3DModel(window, modelTriangles, CAMERAPOS, depthMatrix);
	// drawLineTest(window);
	// textureMapping(window);

	// for (size_t y = 0; y < window.height; y++) {
	// 	for (size_t x = 0; x < window.width; x++) {
	// 		float red = rand() % 256;
	// 		float green = 0.0;
	// 		float blue = 0.0;
	// 		uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
	// 		window.setPixelColour(x, y, colour);
	// 	}
	// }

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
		} else if (event.key.keysym.sym == SDLK_w){
			cameraOrientation = tiltCamera(cameraOrientation, 0.1);
		} else if (event.key.keysym.sym == SDLK_s){
			cameraOrientation = tiltCamera(cameraOrientation, -0.1);
		} else if (event.key.keysym.sym == SDLK_d){
			cameraOrientation = panCamera(cameraOrientation, 0.1);
		} else if (event.key.keysym.sym == SDLK_a){
			cameraOrientation = panCamera(cameraOrientation, -0.1);
		}
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	} 
}

// There is not points outside of the edges when 'f' is pressed. With and without the
// drawTriangly function in this function
void FillBottomTriangle(DrawingWindow &window, CanvasTriangle &triangle, Colour colour){
	// Get the points on the left and right vertices
	CanvasPoint bottom = triangle.v2();
	// std::vector<float> leftVertice = interpolateSingleFloats(bottom.x, triangle.v1().x, bottom.y - triangle.v1().y + 1);
	// std::vector<float> rightVertice = interpolateSingleFloats(bottom.x, triangle.v0().x, bottom.y - triangle.v0().y + 1);

	if(triangle.v1().x > triangle.v0().x) std::swap(triangle.v1(), triangle.v0());
	std::vector<CanvasPoint> leftVertice = getLine(triangle.v1(), bottom, true);
	std::vector<CanvasPoint> rigthVertice = getLine(triangle.v0(), bottom, true);
	
	// std::vector<float> leftVertice = interpolateSingleFloats(triangle.v1().x, bottom.x, bottom.y - triangle.v1().y + 1);
	// std::vector<float> rightVertice = interpolateSingleFloats(triangle.v0().x, bottom.x, bottom.y - triangle.v1().y + 1);
	// DrawTriangle(window, triangle, WHITE);
	// This says on which row we draw the line
	int i = 0;
	int j = 0;
	for(int y = triangle.v0().y; y <= bottom.y; y++){
		if(i+2 < leftVertice.size() && triangle.v1().x < bottom.x) while(leftVertice[i+1].y == y) i++;
		if(j+2 < rigthVertice.size() && triangle.v0().x > bottom.x) while(rigthVertice[j+1].y == y) j++;
		// This are the coordinates of the left and right point of the line that we need to draw
		if(i < leftVertice.size() && j < rigthVertice.size()){
			int startX = leftVertice[i].x;
			int endX = rigthVertice[j].x;
			
			if(startX > endX) std::swap(startX, endX);
			for(int x = startX; x <= endX; x++){
				uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
				window.setPixelColour(x, y, colourInt);
			}
		} else break;
		if(i+2 < leftVertice.size() && triangle.v1().x > bottom.x) while(leftVertice[i+1].y == y) i++;
		if(j+2 < rigthVertice.size() && triangle.v0().x < bottom.x) while(rigthVertice[j+1].y == y) j++;
		i++;
		j++;
	}
	
}

// void FillBottomTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour){
// 	// Get the points on the left and right vertices
// 	CanvasPoint bottom = triangle.v2();
// 	std::vector<float> leftVertice = interpolateSingleFloats(bottom.x, triangle.v1().x, bottom.y - triangle.v1().y + 1);
// 	std::vector<float> rightVertice = interpolateSingleFloats(bottom.x, triangle.v0().x, bottom.y - triangle.v0().y + 1);

// 	// This says on which row we draw the line
// 	int i = 0;
// 	for(int y = bottom.y; y >= triangle.v0().y; y--){
// 		// This are the coordinates of the left and right point of the line that we need to draw
// 		int startX = round(leftVertice[i]);
// 		int endX = round(rightVertice[i]);
		
// 		if(startX > endX) std::swap(startX, endX);
// 		for(int x = startX; x <= endX; x++){
// 			uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
// 			window.setPixelColour(x, y, colourInt);
// 		}
// 		i++;
// 	}
// 	// DrawTriangle(window, triangle, WHITE);
// }

// There is not points outside of the edges when 'f' is pressed. With and without the
// drawTriangly function in this function
// void FillUpperTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour){
// 	// Get the points on the left and right vertices
// 	CanvasPoint top = triangle.v0();
// 	std::vector<float> leftVertice = interpolateSingleFloats(top.x, triangle.v1().x, triangle.v1().y - top.y + 1);
// 	std::vector<float> rightVertice = interpolateSingleFloats(top.x, triangle.v2().x, triangle.v2().y - top.y + 1);

// 	// This says on which row we draw the line
// 	int i = 0;
// 	for(int y = top.y; y < triangle.v1().y; y++){
// 		// This are the coordinates of the left and right point of the line that we need to draw
// 		int startX = round(leftVertice[i]);
// 		int endX = round(rightVertice[i]);
		
// 		if(startX > endX) std::swap(startX, endX);
// 		for(int x = startX; x <= endX; x++){
// 			uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
// 			window.setPixelColour(x, y, colourInt);
// 		}
// 		i++;
// 	}
// 	// DrawTriangle(window, triangle, WHITE);
// }

void FillUpperTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour){
	// Get the points on the left and right vertices
	CanvasPoint top = triangle.v0();
	if(triangle.v1().x > triangle.v2().x) std::swap(triangle.v1(), triangle.v2());
	std::vector<CanvasPoint> leftVertice = getLine(top, triangle.v1(), true);
	std::vector<CanvasPoint> rigthVertice = getLine(top, triangle.v2(), true);
	
	// This says on which row we draw the line
	int i = 0;
	int j = 0;
	for(int y = top.y; y < triangle.v1().y; y++){
		if(i+2 < leftVertice.size() && triangle.v1().x < top.x) while(leftVertice[i+1].y == y) i++;
		if(j+2 < rigthVertice.size() && triangle.v2().x > top.x) while(rigthVertice[j+1].y == y) j++;
		// This are the coordinates of the left and right point of the line that we need to draw
		if(i < leftVertice.size() && j < rigthVertice.size()){
			int startX = leftVertice[i].x;
			int endX = rigthVertice[j].x;
			
			if(startX > endX) std::swap(startX, endX);
			for(int x = startX; x <= endX; x++){
				uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
				window.setPixelColour(x, y, colourInt);
			}
		} else break;
		if(i+2 < leftVertice.size() && triangle.v1().x > top.x) while(leftVertice[i+1].y == y) i++;
		if(j+2 < rigthVertice.size() && triangle.v2().x < top.x) while(rigthVertice[j+1].y == y) j++;
		i++;
		j++;
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

	DrawTriangle(window, triangle, WHITE);

	FillUpperTriangle(window, upperTriangle, colour);
	FillBottomTriangle(window, bottomTriangle, colour);

}

// void FillBottomTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix){
// 	// Get the points on the left and right vertices
// 	CanvasPoint bottom = triangle.v2();
// 	// std::vector<float> leftVertice = interpolateSingleFloats(bottom.x, triangle.v1().x, bottom.y - triangle.v1().y + 1);
// 	// std::vector<float> rightVertice = interpolateSingleFloats(bottom.x, triangle.v0().x, bottom.y - triangle.v0().y + 1);
	
// 	std::vector<CanvasPoint> leftVertice = getLine(bottom, triangle.v1(), true);
// 	std::vector<CanvasPoint> rigthVertice = getLine(bottom, triangle.v0(), true);

// 	std::vector<float> leftVerticeDepth = interpolateSingleFloats(bottom.depth, triangle.v1().depth, bottom.y - triangle.v1().y + 1);
// 	std::vector<float> rightVerticeDepth = interpolateSingleFloats(bottom.depth, triangle.v0().depth, bottom.y - triangle.v0().y + 1);

// 	// row to draw the line on 
// 	int i = 0;
// 	int j = 0;
// 	for(int y = bottom.y; y >= triangle.v0().y; y--){
// 		if(i+1 < leftVertice.size()) while(leftVertice[i+1].y == y) i++;
// 		if(i+1 < leftVertice.size()) while(rigthVertice[j+1].y == y) j++;
// 		// This are the coordinates of the left and right point of the line that we need to draw
// 		float startX = round(leftVertice[i].x);
// 		float endX = round(rigthVertice[j].x);
		
// 		float depthFrom = leftVerticeDepth[i];
// 		float depthTo = rightVerticeDepth[j];
		
// 		if(startX > endX){
// 			std::swap(startX, endX);
// 			std::swap(depthFrom, depthTo);
// 		}
		
// 		float diffDepth = depthTo - depthFrom;

// 		// We taking the bigger one of the 2 differences so there are no brakes in our line
// 		// Number of steps is how much times do we need to add the stepSize to reach to.x
// 		// float numberOfSteps = std::max(abs(diffX), abs(diffY));
// 		float stepSizeDepth = diffDepth/(endX - startX);
// 		int t = 0;
// 		// for(CanvasPoint pL : leftVerticeCanvas){
// 		// 	if(pL.y == y){
// 		// 		for(CanvasPoint pR : rigthVerticeCanvas){
// 		// 			if(pR.y == y){
// 		// 				startX = std::min(pL.x, pR.x);
// 		// 				endX = std::max(pL.x, pR.x);
// 		// 				for(int x = startX; x <= endX; x++){
// 		// 					uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
// 		// 					float depth = depthFrom + (stepSizeDepth*j);
// 		// 					if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
// 		// 						if(1/depth > depthMatrix[y][x]){
// 		// 							window.setPixelColour(x, y, colourInt);
// 		// 							depthMatrix[y][x] = 1/depth;
// 		// 						}
// 		// 					}
// 		// 					j++;
// 		// 				}		
// 		// 			}
// 		// 		}
// 		// 	}
// 		// }
// 		for(int x = startX; x <= endX; x++){
// 			uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
// 			float depth = depthFrom + (stepSizeDepth*t);
// 			if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
// 				if(1/depth > depthMatrix[y][x]){
// 					window.setPixelColour(x, y, colourInt);
// 					depthMatrix[y][x] = 1/depth;
// 				}
//         	}
// 			t++;
// 		}
// 		i++;
// 	}
// 	// DrawTriangle(window, triangle, colour, depthMatrix);
// 	// DrawTriangle(window, triangle, WHITE);
// }

// void FillBottomTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix){
// 	// Get the points on the left and right vertices
// 	CanvasPoint bottom = triangle.v2();
// 	std::vector<float> leftVertice = interpolateSingleFloats(bottom.x, triangle.v1().x, bottom.y - triangle.v1().y + 1);
// 	std::vector<float> rightVertice = interpolateSingleFloats(bottom.x, triangle.v0().x, bottom.y - triangle.v0().y + 1);

// 	std::vector<float> leftVerticeDepth = interpolateSingleFloats(bottom.depth, triangle.v1().depth, bottom.y - triangle.v1().y + 1);
// 	std::vector<float> rightVerticeDepth = interpolateSingleFloats(bottom.depth, triangle.v0().depth, bottom.y - triangle.v0().y + 1);

// 	// row to draw the line on 
// 	int i = 0;
// 	for(int y = bottom.y; y >= triangle.v0().y; y--){
// 		// This are the coordinates of the left and right point of the line that we need to draw
// 		float startX = round(leftVertice[i]);
// 		float endX = round(rightVertice[i]);
		
// 		float depthFrom = leftVerticeDepth[i];
// 		float depthTo = rightVerticeDepth[i];
		
// 		if(startX > endX){
// 			std::swap(startX, endX);
// 			std::swap(depthFrom, depthTo);
// 		}
		
// 		float diffDepth = depthTo - depthFrom;

// 		// We taking the bigger one of the 2 differences so there are no brakes in our line
// 		// Number of steps is how much times do we need to add the stepSize to reach to.x
// 		// float numberOfSteps = std::max(abs(diffX), abs(diffY));
// 		float stepSizeDepth = diffDepth/(endX - startX);
// 		int j = 0;
// 		for(int x = startX; x <= endX; x++){
// 			uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
// 			float depth = depthFrom + (stepSizeDepth*j);
// 			if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
// 				if(1/depth > depthMatrix[y][x]){
// 					window.setPixelColour(x, y, colourInt);
// 					depthMatrix[y][x] = 1/depth;
// 				}
//         	}
// 			j++;
// 		}
// 		i += 2;
// 	}
// 	// DrawTriangle(window, triangle, colour, depthMatrix);
// }

void FillBottomTriangle(DrawingWindow &window, CanvasTriangle &triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix){
	// Get the points on the left and right vertices
	CanvasPoint bottom = triangle.v2();
	
	if(triangle.v1().x > triangle.v0().x) std::swap(triangle.v1(), triangle.v0());
	std::vector<CanvasPoint> leftVertice = getLine(triangle.v1(), bottom, true);
	std::vector<CanvasPoint> rigthVertice = getLine(triangle.v0(), bottom, true);
	
	int i = 0;
	int j = 0;
	for(int y = triangle.v0().y; y <= bottom.y; y++){
		// if(i+2 < leftVertice.size() && (triangle.v1().x > bottom.x)) while(leftVertice[i+1].y == y) i++;
		// if(j+2 < rigthVertice.size() && (triangle.v0().x < bottom.x)) while(rigthVertice[j+1].y == y) j++;
		while(leftVertice[i+1].y == y) if(i+2 < leftVertice.size() && (triangle.v1().x < bottom.x)) i++; else break;
		while(rigthVertice[j+1].y == y) if(j+2 < rigthVertice.size() && (triangle.v0().x > bottom.x)) j++; else break;
		// if(i+2 < leftVertice.size() && (triangle.v1().x > bottom.x)) while(leftVertice[i+1].y == y) i++;
		// if(j+2 < rigthVertice.size()) while(rigthVertice[j+1].y == y) j++;
		// while(leftVertice[i+1].y == y) if(i+1 < leftVertice.size()) i++; else break;
		// while(rigthVertice[j+1].y == y) if(j+1 < rigthVertice.size()) j++; else break;

		// // This are the coordinates of the left and right point of the line that we need to draw
		if(i < leftVertice.size() && (j < rigthVertice.size())){
			float startX = leftVertice[i].x;
			float endX = rigthVertice[j].x;
			
			float depthFrom = leftVertice[i].depth;
			float depthTo = rigthVertice[j].depth;
			
			if(startX > endX){
				std::swap(startX, endX);
				std::swap(depthFrom, depthTo);
			}
			// We taking the bigger one of the 2 differences so there are no brakes in our line
			// Number of steps is how much times do we need to add the stepSize to reach to.x
			// float numberOfSteps = std::max(abs(diffX), abs(diffY));
			float stepSizeDepth = (depthTo - depthFrom)/(endX - startX);
			int t = 0;
			for(int x = startX; x <= endX; x++){
				uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
				float depth = depthFrom + (stepSizeDepth*t);
				if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
					if(1/depth > depthMatrix[y][x]){
						window.setPixelColour(x, y, colourInt);
						depthMatrix[y][x] = 1/depth;
					}
				}
				t++;
			}
		} else break;
		// if(i+2 < leftVertice.size() && triangle.v1().x < bottom.x) while(leftVertice[i+1].y == y) i++;
		// if(j+2 < rigthVertice.size() && triangle.v0().x > bottom.x) while(rigthVertice[j+1].y == y) j++;
		
		// while(leftVertice[i+1].y == y) if(i+1 < leftVertice.size()) i++; else break;
		// while(rigthVertice[j+1].y == y) if(j+1 < rigthVertice.size()) j++; else break;
		
		while(leftVertice[i+1].y == y) if(i+2 < leftVertice.size() && (triangle.v1().x > bottom.x)) i++; else break;
		while(rigthVertice[j+1].y == y) if(j+2 < rigthVertice.size() && (triangle.v0().x < bottom.x)) j++; else break;
		i++;
		j++;
	}
	// DrawTriangle(window, triangle, colour, depthMatrix);
}

// void FillUpperTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix){
// 	// Get the points on the left and rigth vertices
// 	CanvasPoint top = triangle.v0();
// 	// The problem should be here - the interpolation doesn't give the same pixel as the
// 	// drawLine between. But it is the same logic so the problem can be 
// 	// if there are 2 white(edge) pixels on the same line. But the function
// 	// will paint them as well. So it's the interpolation function 
// 	// Next step: compare them

// 	// FIXXXX
// 	// For a fix without drawTriangle we need to interpolate for top.x - triangle.x and get the right most
// 	// or left most pixels of a row and paint to that column
// 	std::vector<float> leftVertice = interpolateSingleFloats(top.x, triangle.v1().x, triangle.v1().y - top.y + 1);
// 	std::vector<float> rightVertice = interpolateSingleFloats(top.x, triangle.v2().x, triangle.v2().y - top.y + 1);
	
// 	// std::vector<CanvasPoint> leftVerticeCanvas = getLine(top, triangle.v1());
// 	// std::vector<CanvasPoint> rigthVerticeCanvas = getLine(top, triangle.v2());

// 	std::vector<float> leftVerticeDepth = interpolateSingleFloats(top.depth, triangle.v1().depth, triangle.v1().y - top.y + 1);
// 	std::vector<float> rightVerticeDepth = interpolateSingleFloats(top.depth, triangle.v2().depth, triangle.v2().y - top.y + 1);

// 	int i = 0;
// 	for(int y = top.y; y <= triangle.v1().y; y++){
// 		// This says on which row we draw the line
// 		int row = y - top.y;
// 		// This are the coordinates of the left and right point of the line that we need to draw
// 		float startX = round(leftVertice[i]);
// 		float endX = round(rightVertice[i]);

// 		float depthFrom = leftVerticeDepth[i];
// 		float depthTo = rightVerticeDepth[i];
		
// 		if(startX > endX){
// 			std::swap(startX, endX);
// 			std::swap(depthFrom, depthTo);
// 		}
		
// 		// We taking the bigger one of the 2 differences so there are no brakes in our line
// 		// Number of steps is how much times do we need to add the stepSize to reach to.x
// 		float stepSizeDepth = (depthTo - depthFrom)/(endX - startX);

// 		// int j = 0;
// 		// for(CanvasPoint pL : leftVerticeCanvas){
// 		// 	if(pL.y == y){
// 		// 		for(CanvasPoint pR : rigthVerticeCanvas){
// 		// 			if(pR.y == y){
// 		// 				startX = std::min(pL.x, pR.x);
// 		// 				endX = std::max(pL.x, pR.x);
// 		// 				for(int x = startX; x <= endX; x++){
// 		// 					uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
// 		// 					float depth = depthFrom + (stepSizeDepth*j);
// 		// 					if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
// 		// 						if(1/depth > depthMatrix[y][x]){
// 		// 							window.setPixelColour(x, y, colourInt);
// 		// 							depthMatrix[y][x] = 1/depth;
// 		// 						}
// 		// 					}
// 		// 					j++;
// 		// 				}		
// 		// 			}
// 		// 		}
// 		// 	}
// 		// }
// 		int j = 0;
// 		for(int x = startX; x <= endX; x++){
// 			uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
// 			float depth = depthFrom + (stepSizeDepth*j);
// 			if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
// 				if(1/depth > depthMatrix[y][x]){
// 					window.setPixelColour(x, y, colourInt);
// 					depthMatrix[y][x] = 1/depth;
// 				}
//         	}	
// 			j++;
// 		}
// 		i++;
// 		// drawLine1(window, from, to, colour, depthMatrix);
// 	}
// 	// DrawTriangle(window, triangle, colour, depthMatrix);
// 	// DrawTriangle(window, triangle, WHITE);
// }

void FillUpperTriangle(DrawingWindow &window, CanvasTriangle &triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix){
	// Get the points on the left and rigth vertices
	CanvasPoint top = triangle.v0();
	if(triangle.v1().x > triangle.v2().x) std::swap(triangle.v1(), triangle.v2());
	std::vector<CanvasPoint> leftVertice = getLine(top, triangle.v1(), true);
	std::vector<CanvasPoint> rigthVertice = getLine(top, triangle.v2(), true);
	
	// This says on which row we draw the line
	int i = 0;
	int j = 0;
	for(int y = top.y; y <= triangle.v1().y; y++){
		while(leftVertice[i+1].y == y) if(i+2 < leftVertice.size() && triangle.v1().x < top.x) i++; else break;
		while(rigthVertice[j+1].y == y) if(j+2 < rigthVertice.size() && triangle.v2().x > top.x) j++; else break;
		// This are the coordinates of the left and right point of the line that we need to draw
		if(i < leftVertice.size() && j < rigthVertice.size()){
		float startX = leftVertice[i].x;
		float endX = rigthVertice[j].x;

		float depthFrom = leftVertice[i].depth;
		float depthTo = rigthVertice[j].depth;
		
		if(startX > endX){
			std::swap(startX, endX);
			std::swap(depthFrom, depthTo);
		}
			float temp = std::max(abs(leftVertice[i].x - rigthVertice[j].x), abs(leftVertice[i].y - rigthVertice[j].y));
			float stepsDepth = std::max(abs(leftVertice[i].depth - rigthVertice[j].depth), temp);
			// We taking the bigger one of the 2 differences so there are no brakes in our line
			// Number of steps is how much times do we need to add the stepSize to reach to.x
			// float numberOfSteps = std::max(abs(diffX), abs(diffY));
			float stepSizeDepth = (depthTo - depthFrom)/(endX - startX);
			
	// 	float diffX = to.x - from.x;
	// float diffY = to.y - from.y;
	// float diffDepth = to.depth - from.depth;

	// float numberOfSteps = std::max(abs(diffX), abs(diffY));
	// float numberOfStepsDepth = std::max(numberOfSteps, abs(diffDepth));
		
		// We taking the bigger one of the 2 differences so there are no brakes in our line
		// Number of steps is how much times do we need to add the stepSize to reach to.x
		// float stepSizeDepth = (depthTo - depthFrom)/(endX - startX + 1);

		int t = 0;
		for(int x = startX; x <= endX; x++){
			uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
			float depth = depthFrom + (stepSizeDepth*t);
			if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
				if(1/depth > depthMatrix[y][x]){
					window.setPixelColour(x, y, colourInt);
					depthMatrix[y][x] = 1/depth;
				}
        	}	
			t++;
		}
		} else break;
		while(leftVertice[i+1].y == y) if(i+2 < leftVertice.size() && triangle.v1().x > top.x) i++; else break;
		while(rigthVertice[j+1].y == y) if(j+2 < rigthVertice.size() && triangle.v2().x < top.x) j++; else break;
		i++;
		j++;
		// drawLine1(window, from, to, colour, depthMatrix);
	}
	// DrawTriangle(window, triangle, colour, depthMatrix);
	// DrawTriangle(window, triangle, WHITE);
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
	
	// FillUpperTriangle(window, upperTriangle, colour);
	// FillBottomTriangle(window, bottomTriangle, colour);

	// DrawTriangle(window, triangle, colour, depthMatrix);
	// DrawTriangle(window, triangle, WHITE);
}

// correct for sure
CanvasPoint projectVertexOntoCanvasPoint(glm::vec3 cameraPosition, float focalLength, glm::vec3 vertexPosition, glm::mat3 cameraOrientation){
	
	// Adjust vector to point from camera position(because before it was from (0,0,0) - world center)
	// and adjust the orientation
	glm::vec3 adjustedVector = (vertexPosition - cameraPosition) * cameraOrientation;
	
	float u = -160*focalLength*adjustedVector[0]/adjustedVector[2] + WIDTH/2;
	float v = 160*focalLength*adjustedVector[1]/adjustedVector[2] + HEIGHT/2;

	// -adjustedVector[2] is the z which is the depth(how close it is to the camera)
	// we put - because z is already negative 
	return CanvasPoint(u, v, -adjustedVector[2]);
}

// Rotate around X - axis
glm::mat3 tiltCamera(glm::mat3 cameraOrientation, float angle){
	glm::mat3 rotationMatrix = glm::mat3(glm::vec3(1.0, 0.0, 0.0),
								glm::vec3(0.0, cos(angle), sin(angle)),
								glm::vec3(0.0, -sin(angle), cos(angle)));
	return rotationMatrix*cameraOrientation;
}

// Rotate around y - axis
glm::mat3 panCamera(glm::mat3 cameraOrientation, float angle){
	glm::mat3 rotationMatrix = glm::mat3(glm::vec3(cos(angle), 0.0, sin(angle)),
								glm::vec3(0.0, 1.0, 0.0),
								glm::vec3(-sin(angle), 0.0, cos(angle)));
	return rotationMatrix*cameraOrientation;
}

// correct 100 %
void render3DModel(DrawingWindow &window, std::vector<ModelTriangle> modelTriangles, float focalLength, glm::vec3 cameraPosition, glm::mat3 cameraOrientation){
	
	std::vector<std::vector<float>> depthMatrix(HEIGHT, std::vector<float>(WIDTH, 0));

	for(ModelTriangle triangle3D : modelTriangles){
		std::array<glm::vec3, 3> vertices = triangle3D.vertices;

		// this is the points on the image plane and each one have some depth
		// (how close the projected point is to the image plane)
		CanvasPoint v0 = projectVertexOntoCanvasPoint(cameraPosition, focalLength, vertices[0], cameraOrientation);
		CanvasPoint v1 = projectVertexOntoCanvasPoint(cameraPosition, focalLength, vertices[1], cameraOrientation);
		CanvasPoint v2 = projectVertexOntoCanvasPoint(cameraPosition, focalLength, vertices[2], cameraOrientation);
		
		CanvasTriangle triangle2D = CanvasTriangle(v0, v1, v2);

		// DrawFullTriangle(window, triangle2D, triangle3D.colour);
		DrawFullTriangle(window, triangle2D, triangle3D.colour, depthMatrix);
		// DrawTriangle(window, triangle2D, triangle3D.colour, depthMatrix);
		// DrawTriangle(window, triangle2D, WHITE);
	}
}

void lookAt(glm::mat3 &cameraOrientation, glm::vec3 cameraPosition, glm::vec3 lookAtPoint){
	glm::vec3 forward = glm::normalize(cameraPosition - lookAtPoint);
	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, -1, 0)));
	glm::vec3 up = glm::normalize(glm::cross(forward, right));
	cameraOrientation = glm::mat3(right, up, forward);
}

void orbit(glm::mat3 &cameraOrientation, glm::vec3 &cameraPosition, float angle){
	glm::mat3 rotationMatrix = glm::mat3(glm::vec3(cos(angle), 0.0, sin(angle)),
							glm::vec3(0.0, 1.0, 0.0),
							glm::vec3(-sin(angle), 0.0, cos(angle)));
	cameraPosition = rotationMatrix * cameraPosition;
	lookAt(cameraOrientation, cameraPosition, glm::vec3(0,0,0));
}

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;

	unordered_map<std::string, Colour> colourHashMap = readMTLfile("cornell-box.mtl");
	std::vector<ModelTriangle> modelTriangles = readOBJfile("cornell-box.obj", 0.35, colourHashMap);

	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		
		// draw(window);
		window.clearPixels();
		// drawLineTest(window);
		orbit(cameraOrientation, cameraPosition, 0.005);
		render3DModel(window, modelTriangles, 2.0, cameraPosition, cameraOrientation);
		// drawLineTest(window);
		// textureMapping(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
