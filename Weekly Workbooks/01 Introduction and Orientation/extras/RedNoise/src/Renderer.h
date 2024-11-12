#include <DrawingWindow.h>
#include <CanvasTriangle.h>
#include <Colour.h>
#include <vector>
#include <glm/glm.hpp>
#include <ModelTriangle.h>

void drawLine(DrawingWindow &window, CanvasPoint from, CanvasPoint to, Colour colour);
void DrawTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour);
void DrawFullTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix);
void render3DModel(DrawingWindow &window, std::vector<ModelTriangle> modelTriangles, glm::vec3 cameraPosition, std::vector<std::vector<float>> &depthMatrix);
