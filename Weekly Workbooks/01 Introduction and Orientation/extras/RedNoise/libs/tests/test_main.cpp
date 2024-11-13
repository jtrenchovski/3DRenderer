#include <algorithm>
#include <sstream>
#include "Utils.h"
#include "CanvasTriangle.h"
#include "DrawingWindow.h"
#include "vector"
#include "CanvasPoint.h"
#include "Colour.h"
#include "TextureMap.h"
#include "iostream"
#include "unordered_map"

// This should be tested 

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

int main() {
    // Test for interpolateSingleFloats
    std::vector<float> expectedSingleFloatResults = {2.2, 3.25, 4.3, 5.35, 6.4, 7.45, 8.5};
    std::vector<float> result = interpolateSingleFloats(2.2, 8.5, 7);

    assert(result.size() == expectedSingleFloatResults.size());
    for(size_t i = 0; i < result.size(); ++i) {
        assert(result[i] == expectedSingleFloatResults[i]);
    }
    std::cout << "interpolateSingleFloats test passed!" << std::endl;

    // Test for interpolateThreeElementVectors
    glm::vec3 from(1.0, 4.0, 9.2);
    glm::vec3 to(4.0, 1.0, 9.8);
    std::vector<glm::vec3> expectedThreeElementResults = {
        glm::vec3(1.0, 4.0, 9.2),
        glm::vec3(2.0, 3.0, 9.4),
        glm::vec3(3.0, 2.0, 9.6),
        glm::vec3(4.0, 1.0, 9.8)
    };
    
    std::vector<glm::vec3> result3D = interpolateThreeElementVectors(from, to, 4);
    
    assert(result3D.size() == expectedThreeElementResults.size());
    for(size_t i = 0; i < result3D.size(); ++i) {
        for(size_t j = 0; j < 3; ++j) {
            assert(result3D[i][j] == expectedThreeElementResults[i][j]);
        }
    }
    std::cout << "interpolateThreeElementVectors test passed!" << std::endl;

    return 0;
}