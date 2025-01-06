# 3D Renderer

This is a 3D rendering application implemented as part of a coursework project. The program supports multiple rendering techniques and interactive controls to explore 3D scenes.

## Features Implemented

- **Flat Colour 2D Filled Triangle Rasterizing**: Triangles are rasterized with flat colours.
- **OBJ Geometry File Loading**: Supports loading and rendering geometry from OBJ files.
- **Wireframe 3D Scene Rendering**: Renders 3D scenes in wireframe mode.
- **Keyboard Control of Camera Position**: Move the camera using keyboard inputs.
- **OBJ Material File Loading**: Materials from `.mtl` files are loaded and applied to objects.
- **Flat Colour 3D Scene Rasterizing**: 3D scenes are rasterized with flat colours.
- **Depth Buffer**: Resolves occlusion by using a depth buffer.
- **Keyboard Control of Camera Orientation**: Camera can be oriented using keyboard inputs with an orientation matrix.
- **Hard Shadows**: Shadows are rendered with ambient lighting but without soft edges.
- **Diffuse Lighting**: Lighting is based on proximity and angle of incidence.
- **Simple Animation**: Implements camera orbit and basic object transformations.
- **Specular Lighting**: Includes a clearly visible specular highlight (specular "spot").
- **Phong Shading**: Implements smooth shading across the surface of objects.

## Usage

1. Clone the repository and navigate to the project directory.
2. Run the following command to build and run the application:
   ```bash
   make
