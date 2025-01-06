#pragma once

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

std::vector<std::string> split(const std::string &line, char delimiter);
CanvasTriangle getRandomTriangle();
std::vector<float> interpolateSingleFloats(float from, float to, float numberOfValues);
std::vector<glm::vec3> interpolateThreeElementVectors(glm::vec3 from, glm::vec3 to, float numberOfValues);
std::vector<CanvasPoint> interpolateTwoCanvasPoints(CanvasPoint from, CanvasPoint to, float numberOfValues);
std::vector<CanvasPoint> getLine(CanvasPoint from, CanvasPoint to);
std::vector<CanvasPoint> getLine(CanvasPoint from, CanvasPoint to, bool is3D);

void drawLine1(DrawingWindow &window, CanvasPoint from, CanvasPoint to, Colour colour);
void drawLine1(DrawingWindow &window, CanvasPoint from, CanvasPoint to, Colour colour, std::vector<std::vector<float>> &depthMatrix);

void DrawTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour);
void DrawTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour, std::vector<std::vector<float>> &depthMatrix);
CanvasPoint findLeftPoint(CanvasTriangle &triangle);

std::vector<ModelTriangle> readOBJfile(std::string filename, float scale, std::unordered_map<std::string, Colour> &colourHashMap);
std::unordered_map<std::string, Colour> readMTLfile(std::string filename);