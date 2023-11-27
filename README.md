# Raytracing Minecraft

https://github.com/TheKiesling/CG-Project-Minecraft/assets/84196494/b066627d-bd65-41d5-aa8f-ee58c26bd5fe

## Overview

This project focuses on raytracing, a rendering technique that simulates the behavior of light as it interacts with objects in a scene. The objective is to create a realistic 3D environment by tracing the path of light rays. Different materials have been implemented, offering diverse reflective and refractive properties to the objects in the scene.

### Screenshots

| ![image](https://github.com/TheKiesling/CG-Project-Minecraft/assets/84196494/d6334a47-e5c7-484f-850f-a57f5065d389) | ![image](https://github.com/TheKiesling/CG-Project-Minecraft/assets/84196494/d05d5fa7-ec24-4596-b145-0d6ac9592cd4) |
|------------  | -------------  |

## Project Structure

The project is structured as follows:

- `main.cpp`: Entry point of the raytracing application.
- `color.h`: Contains color structures and utilities.
- `intersect.h`: Defines intersection structures and calculations.
- `object.h`: Abstract class for objects in the scene.
- `light.h`: Holds information about the light source in the scene.
- `camera.h`: Manages the camera's position and orientation.
- `cube.h`: Class representing a cube object in the scene.
- `imageloader.h`: Handles image loading and manipulation.
- `skybox.h`: Deals with the rendering of a skybox in the scene.
- `materials/`: Folder containing different material classes used in objects.

## Materials

### Netherrack
- **Diffuse Color:** Dark Red.
- **Reflectivity:** 0.1
- **Transparency:** 0.0
- **Specular Coefficient:** 0.5
- **Specular Albedo:** White (1, 1, 1)
- **Albedo:** White (1, 1, 1)

### Obsidian
- **Diffuse Color:** Black.
- **Reflectivity:** 0.2
- **Transparency:** 0.0
- **Specular Coefficient:** 0.0
- **Specular Albedo:** Black (0, 0, 0)
- **Albedo:** White (1, 1, 1)

### Portal
- **Diffuse Color:** Purple.
- **Reflectivity:** 0.5
- **Transparency:** 0.5
- **Specular Coefficient:** 1.0
- **Specular Albedo:** White (1, 1, 1)
- **Albedo:** White (1, 1, 1)
- **Refraction Index:** 1.5

### Gold
- **Diffuse Color:** Gold.
- **Reflectivity:** 0.6
- **Transparency:** 0.0
- **Specular Coefficient:** 8.0
- **Specular Albedo:** White (1, 1, 1)
- **Albedo:** White (1, 1, 1)

### Diamond
- **Diffuse Color:** Light Blue.
- **Reflectivity:** 0.8
- **Transparency:** 0.2
- **Specular Coefficient:** 10.0
- **Specular Albedo:** White (1, 1, 1)
- **Albedo:** White (1, 1, 1)
- **Refraction Index:** 2.4

### Stone
- **Diffuse Color:** Gray.
- **Reflectivity:** 0.0
- **Transparency:** 0.0
- **Specular Coefficient:** 0.0
- **Specular Albedo:** Black (0, 0, 0)
- **Albedo:** White (1, 1, 1)


## Features

- **Raytracing Rendering:** Utilizes raytracing techniques to render a 3D environment, simulating light interactions with various materials.
- **Material Properties:** Implements different material types with varying reflective and refractive characteristics.
- **Dynamic Textures:** Renders textures onto objects based on the assigned materials.

## Usage

1. Compile and run the `main.cpp` file.
2. Use the controls to navigate the camera through the scene (specified in the application).
3. Observe the rendering of materials with different reflective and refractive properties.

## Contributing

Contributions to improve the raytracing implementation or add new materials are welcome! Follow these steps:

1. Fork the repository.
2. Create a new branch: `git checkout -b feature/new-feature`.
3. Make your changes and commit them: `git commit -m 'Add a new feature'`.
4. Push to the branch: `git push origin feature/new-feature`.
5. Open a pull request.


