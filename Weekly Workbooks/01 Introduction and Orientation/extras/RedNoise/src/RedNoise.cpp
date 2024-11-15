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
#include <Renderer.h>
#include <RayTriangleIntersection.h>

using namespace std;

#define WIDTH 320
#define HEIGHT 240
#define WHITE Colour(255, 255, 255)

glm::vec3 cameraPosition = glm::vec3(0.0, 0.0, 4.0);
glm::mat3 cameraOrientation = glm::mat3(1.0);
bool orbitBool = false;
int mode = 2;

void DrawTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour);
void DrawFullTriangle(DrawingWindow &window, CanvasTriangle &triangle, Colour colour);
std::vector<CanvasPoint> getLine(CanvasPoint from, CanvasPoint to);
glm::vec3 changeCameraPosition(glm::vec3);
glm::mat3 panCamera(glm::mat3 cameraOrientation, float angle);
glm::mat3 tiltCamera(glm::mat3 cameraOrientation, float angle);
void orbit(glm::mat3 &cameraOrientation, glm::vec3 &cameraPosition, float angle, bool orbitBool);
void lookAt(glm::mat3 &cameraOrientation, glm::vec3 cameraPosition, glm::vec3 lookAtPoint);

void drawRasterisedScene(DrawingWindow &window, std::vector<ModelTriangle> modelTriangles, float focalLength, glm::vec3 cameraPosition, glm::mat3 cameraOrientation) {
	window.clearPixels();
	render3DModel(window, modelTriangles, focalLength, cameraPosition, cameraOrientation);
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
		} else if (event.key.keysym.sym == SDLK_o){
			orbitBool = !orbitBool;
		} else if (event.key.keysym.sym == SDLK_1){
			mode = 1;
		} else if (event.key.keysym.sym == SDLK_2){
			mode = 2;
		} else if (event.key.keysym.sym == SDLK_3){
			mode = 3;
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

	if(triangle.v1().x > triangle.v0().x) std::swap(triangle.v1(), triangle.v0());
	std::vector<CanvasPoint> leftVertice = getLine(triangle.v1(), bottom, true);
	std::vector<CanvasPoint> rigthVertice = getLine(triangle.v0(), bottom, true);
	
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

void lookAt(glm::mat3 &cameraOrientation, glm::vec3 cameraPosition, glm::vec3 lookAtPoint){
	glm::vec3 forward = glm::normalize(cameraPosition - lookAtPoint);
	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, -1, 0)));
	glm::vec3 up = glm::normalize(glm::cross(forward, right));
	cameraOrientation = glm::mat3(right, up, forward);
}

void orbit(glm::mat3 &cameraOrientation, glm::vec3 &cameraPosition, float angle, bool orbitBool){
	if(!orbitBool) return;
	glm::mat3 rotationMatrix = glm::mat3(glm::vec3(cos(angle), 0.0, sin(angle)),
							glm::vec3(0.0, 1.0, 0.0),
							glm::vec3(-sin(angle), 0.0, cos(angle)));
	cameraPosition = rotationMatrix * cameraPosition;
	lookAt(cameraOrientation, cameraPosition, glm::vec3(0,0,0));
}

bool getClosestIntersection(std::vector<ModelTriangle> modelTriangles, glm::vec3 cameraPosition, glm::vec3 rayDirection, RayTriangleIntersection &closestIntersectionPoint){
	bool found = false;
	int i = 0;
	for(ModelTriangle triangle : modelTriangles){
		glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
		glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
		glm::vec3 SPVector = cameraPosition - triangle.vertices[0];
		glm::mat3 DEMatrix(-rayDirection, e0, e1);
		glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;
		float u = possibleSolution.y;
		float v = possibleSolution.z;
		if(possibleSolution[0] < closestIntersectionPoint.distanceFromCamera && 0 < possibleSolution[0] &&
				(u >= 0.0 && u <= 1.0) && (v >= 0.0 && v <= 1.0) && ((u + v) <= 1.0)){
			found = true;
			glm::vec3 intersectionPoint = triangle.vertices[0] + possibleSolution[1]*e0 + possibleSolution[2]*e1;
			closestIntersectionPoint = RayTriangleIntersection(intersectionPoint, possibleSolution[0], triangle, i);
		}
		i++;
	}
	return found;
}

glm::vec3 getRayDirection(float focalLength, CanvasPoint imagePlanePoint){
	glm::vec3 imagePointCameraSpace(imagePlanePoint.x, imagePlanePoint.y, -focalLength);
	glm::vec3 imagePointWorldSpace = imagePointCameraSpace*cameraOrientation;
	glm::vec3 direction = glm::normalize(imagePointWorldSpace);
	// cout << direction.x << direction.y << direction.z << endl;
	return direction;
}

bool isShadow(std::vector<ModelTriangle> modelTriangles, glm::vec3 lightSource, glm::vec3 targetPoint){
	glm::vec3 shadowRay = glm::normalize(lightSource - targetPoint);
	float l1 = glm::distance(lightSource, targetPoint);
	RayTriangleIntersection intersection = RayTriangleIntersection();
	// Adding 0.01f*shadowRay so the shadow isn't too close
	if(getClosestIntersection(modelTriangles, targetPoint + 0.01f*shadowRay, shadowRay, intersection)){
		if(l1 > glm::distance(intersection.intersectionPoint, targetPoint)){
			return true;
		}
	} 
	return false;
}

void drawRayTracing(DrawingWindow &window, std::vector<ModelTriangle> modelTriangles, glm::vec3 cameraPosition, float focalLength, glm::vec3 lightSource){
	window.clearPixels();

	for(int j= -HEIGHT/2; j < HEIGHT/2; j++){
		for(int i= -WIDTH/2 ; i < WIDTH/2; i++){
			glm::vec3 rayDirection = getRayDirection(focalLength, CanvasPoint(i, j));
			RayTriangleIntersection intersection = RayTriangleIntersection();
			if(getClosestIntersection(modelTriangles, cameraPosition, rayDirection, intersection)){
				// cout << triangleIntersection.intersectionPoint.x << triangleIntersection.intersectionPoint.y << triangleIntersection.intersectionPoint.z << triangleIntersection.intersectedTriangle << endl;
				// cout << triangleIntersection.distanceFromCamera << endl;

				// Is it a shadow
				if(!isShadow(modelTriangles, lightSource, intersection.intersectionPoint)){
					Colour colour = intersection.intersectedTriangle.colour;
					uint32_t colourInt = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
					window.setPixelColour(i + WIDTH/2, -j + HEIGHT/2, colourInt);
				}
			}
		}
	}
}

void drawWireFrame(DrawingWindow &window, std::vector<ModelTriangle> modelTriangles, float focalLength, glm::vec3 cameraPosition, glm::mat3 cameraOrientation){
	window.clearPixels();
	render3DModelWireFrame(window, modelTriangles, focalLength, cameraPosition, cameraOrientation);
}

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;

	float focalLength = 2.0;
	float scale = 0.35; // 0.35
	unordered_map<std::string, Colour> colourHashMap = readMTLfile("cornell-box.mtl");
	glm::vec3 lightSource = glm::vec3(0, 2.4, 0) * scale; 
	std::vector<ModelTriangle> modelTriangles = readOBJfile("cornell-box.obj", scale, colourHashMap); 

	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		
		if(mode == 1){
			drawWireFrame(window, modelTriangles, focalLength, cameraPosition, cameraOrientation);
		} else if(mode == 2){
			drawRasterisedScene(window, modelTriangles, focalLength, cameraPosition, cameraOrientation);
		} else if(mode == 3){
			drawRayTracing(window, modelTriangles, cameraPosition, WIDTH, lightSource);
		}
		orbit(cameraOrientation, cameraPosition, 0.005, orbitBool);
		
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
