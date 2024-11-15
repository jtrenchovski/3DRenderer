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
#include "Renderer.h"

#define WIDTH 320
#define HEIGHT 240

void FillBottomTriangle(DrawingWindow &window, CanvasTriangle &triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix){
	// Get the points on the left and right vertices
	CanvasPoint bottom = triangle.v2();
	
	if(triangle.v1().x > triangle.v0().x) std::swap(triangle.v1(), triangle.v0());
	std::vector<CanvasPoint> leftVertice = getLine(triangle.v1(), bottom, true);
	std::vector<CanvasPoint> rigthVertice = getLine(triangle.v0(), bottom, true);
	
	int i = 0;
	int j = 0;
	for(int y = triangle.v0().y; y <= bottom.y; y++){
		while(leftVertice[i+1].y == y) if(i+2 < leftVertice.size() && (triangle.v1().x < bottom.x)) i++; else break;
		while(rigthVertice[j+1].y == y) if(j+2 < rigthVertice.size() && (triangle.v0().x > bottom.x)) j++; else break;

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
			float stepSizeDepth = (depthTo - depthFrom)/(endX - startX + 1);
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
	
		while(leftVertice[i+1].y == y) if(i+2 < leftVertice.size() && (triangle.v1().x > bottom.x)) i++; else break;
		while(rigthVertice[j+1].y == y) if(j+2 < rigthVertice.size() && (triangle.v0().x < bottom.x)) j++; else break;
		i++;
		j++;
	}
	// DrawTriangle(window, triangle, colour, depthMatrix);
}

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
		float stepSizeDepth = (depthTo - depthFrom)/(endX - startX + 1);
				
		// We taking the bigger one of the 2 differences so there are no brakes in our line
		// Number of steps is how much times do we need to add the stepSize to reach to.x

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

void render3DModelWireFrame(DrawingWindow &window, std::vector<ModelTriangle> modelTriangles, float focalLength, glm::vec3 cameraPosition, glm::mat3 cameraOrientation){
	
	std::vector<std::vector<float>> depthMatrix(HEIGHT, std::vector<float>(WIDTH, 0));

	for(ModelTriangle triangle3D : modelTriangles){
		std::array<glm::vec3, 3> vertices = triangle3D.vertices;

		// this is the points on the image plane and each one have some depth
		// (how close the projected point is to the image plane)
		CanvasPoint v0 = projectVertexOntoCanvasPoint(cameraPosition, focalLength, vertices[0], cameraOrientation);
		CanvasPoint v1 = projectVertexOntoCanvasPoint(cameraPosition, focalLength, vertices[1], cameraOrientation);
		CanvasPoint v2 = projectVertexOntoCanvasPoint(cameraPosition, focalLength, vertices[2], cameraOrientation);
		
		CanvasTriangle triangle2D = CanvasTriangle(v0, v1, v2);

		DrawTriangle(window, triangle2D, triangle3D.colour, depthMatrix);
		// DrawFullTriangle(window, triangle2D, triangle3D.colour, depthMatrix);
		// DrawTriangle(window, triangle2D, triangle3D.colour, depthMatrix);
		// DrawTriangle(window, triangle2D, WHITE);
	}
}