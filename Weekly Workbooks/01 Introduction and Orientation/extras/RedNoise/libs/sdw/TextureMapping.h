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

void textureMapUpperTriangle(DrawingWindow &window, CanvasTriangle &triangle, CanvasTriangle sourceTriangle, TextureMap textureMap);
void textureMapBottomTriangle(DrawingWindow &window, CanvasTriangle triangle, CanvasTriangle sourceTriangle, TextureMap textureMap);
void textureMapping(DrawingWindow &window);
std::vector<CanvasPoint> interpolateTwoCanvasPoints(CanvasPoint from, CanvasPoint to, float numberOfValues);
std::vector<CanvasPoint> getSourceLine(CanvasTriangle triangle, float proportion, int numberOfPoints);
std::vector<CanvasPoint> getLine(CanvasPoint from, CanvasPoint to);
uint32_t getIndex(size_t width, int x, int y);