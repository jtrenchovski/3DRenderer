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

void FillBottomTriangle(DrawingWindow &window, CanvasTriangle &triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix);
void FillUpperTriangle(DrawingWindow &window, CanvasTriangle &triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix);
void DrawFullTriangle(DrawingWindow &window, CanvasTriangle &triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix);
CanvasPoint projectVertexOntoCanvasPoint(glm::vec3 cameraPosition, float focalLength, glm::vec3 vertexPosition, glm::mat3 cameraOrientation);
void render3DModel(DrawingWindow &window, std::vector<ModelTriangle> modelTriangles, float focalLength, glm::vec3 cameraPosition, glm::mat3 cameraOrientation);
void render3DModelWireFrame(DrawingWindow &window, std::vector<ModelTriangle> modelTriangles, float focalLength, glm::vec3 cameraPosition, glm::mat3 cameraOrientation);