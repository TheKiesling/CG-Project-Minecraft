#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <cstdlib>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include "glm/ext.hpp"

#include "color.h"
#include "intersect.h"
#include "object.h"
#include "light.h"
#include "camera.h"
#include "cube.h"
#include "imageloader.h"
#include "skybox.h"

#include "./materials/netherrack.h"
#include "./materials/obsidian.h"
#include "./materials/portal.h"
#include "./materials/gold.h"
#include "./materials/diamond.h"
#include "./materials/stone.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float ASPECT_RATIO = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
const int MAX_RECURSION = 3;
const float BIAS = 0.0001f;

SDL_Renderer* renderer;
std::vector<Object*> objects;
Light light = {
    glm::vec3(-10.0f, 10.0f, 20.0f), 
    1.0f, 
    Color(255, 0,0)
};
Camera camera(glm::vec3(0.0, 0.0, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 10.0f);


void point(glm::vec2 position, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer, position.x, position.y);
}

float castShadow(const glm::vec3& shadowOrigin, const glm::vec3& lightDir, Object* hitObject) {
    for (auto& obj : objects) {
        if (obj != hitObject) {
            Intersect shadowIntersect = obj->rayIntersect(shadowOrigin, lightDir);
            if (shadowIntersect.isIntersecting && shadowIntersect.dist > 0) {
                float shadowRatio = shadowIntersect.dist / glm::length(light.position - shadowOrigin);
                shadowRatio = glm::min(1.0f, shadowRatio);
                return 1.0f - shadowRatio;
            }
        }
    }
    return 1.0f;
}

Color castRay(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const short recursion = 0, Object* currentObj = nullptr) {
    float zBuffer = 99999;
    Object* hitObject = nullptr;
    Intersect intersect;

    for (const auto& object : objects) {
        Intersect i = object->rayIntersect(rayOrigin, rayDirection);
        if (i.isIntersecting && i.dist < zBuffer && currentObj != object) {
            zBuffer = i.dist;
            hitObject = object;
            intersect = i;
        }
    }

    if (!intersect.isIntersecting || recursion == MAX_RECURSION) {
        // return Color(173, 216, 230);
        return Skybox::getColor(rayOrigin, rayDirection);
    }

    glm::vec3 lightDir = glm::normalize(light.position - intersect.point);
    glm::vec3 viewDir = glm::normalize(rayOrigin - intersect.point);
    glm::vec3 reflectDir = glm::reflect(-glm::normalize(rayOrigin), intersect.normal);     

    float shadowIntensity = castShadow(intersect.point, lightDir, hitObject);

    float diffuseLightIntensity = std::max(0.0f, glm::dot(intersect.normal, lightDir));
    float specReflection = glm::dot(viewDir, reflectDir);
    
    Material mat = hitObject->material;

    float specLightIntensity = std::pow(std::max(0.0f, glm::dot(viewDir, reflectDir)), mat.specularCoefficient);

    Color reflectedColor(0.0f, 0.0f, 0.0f);
    if (mat.reflectivity > 0) {
        glm::vec3 origin = intersect.point + intersect.normal * BIAS;
        reflectedColor = castRay(origin, reflectDir, recursion + 1, hitObject); 
    }

    Color refractedColor(0.0f, 0.0f, 0.0f);
    if (mat.transparency > 0) {
        glm::vec3 origin = intersect.point - intersect.normal * BIAS;
        glm::vec3 refractDir = glm::refract(rayDirection, intersect.normal, mat.refractionIndex);
        refractedColor = castRay(origin, refractDir, recursion + 1, hitObject); 
    }

    Color materialLight = intersect.hasColor ? intersect.color : mat.diffuse;

    Color diffuseLight = materialLight * light.intensity * diffuseLightIntensity * mat.albedo * shadowIntensity;
    Color specularLight = light.color * light.intensity * specLightIntensity * mat.specularAlbedo * shadowIntensity;
    Color color = (diffuseLight + specularLight) * (1.0f - mat.reflectivity - mat.transparency) + reflectedColor * mat.reflectivity + refractedColor * mat.transparency;
    return color;
} 

void setUp() {
    Material obsidian = {
        Color(0, 0, 0),
        0.8f,
        0.0f,
        1000.0f,
        0.2f,
        0.0f
    };

    Material portal = {
        Color(128, 0, 128),  
        1.0f,   
        1.0f,             
        0.9f,                
        0.1f,               
        0.5f,                
        1.5f                 
    };
    
    Material gold = {
        Color(255, 215, 0),   
        1.0f,                 
        8.0f,                 
        0.4f,                
        0.6f,                 
        0.0f,                 
        1.5f                  
    };

    Material diamond = {
        Color(127, 213, 240), 
        1.0f,                  
        10.0f,              
        0.8f,                
        0.8f,                  
        0.0f,                  
        2.4f                   
    };

    Material netherrack = {
        Color(153, 25, 25),  
        1.0f,                
        0.5f,                
        0.3f,                
        0.1f,                
        0.0f,                
        1.0f                 
    };

    Material stone = {
        Color(128, 128, 128), 
        1.0f,                 
        0.0f,              
        0.0f,               
        0.0f,               
        0.0f,                
        1.0f                 
    };


    objects.push_back(new Netherrack(glm::vec3(-1.5f, -0.5f, -1.0f), glm::vec3(3.0f, 0.0f, -4.0f), netherrack));
    objects.push_back(new Netherrack(glm::vec3(-2.0f, -1.5f, 0.0f), glm::vec3(3.5f, -1.0f, -4.0f), netherrack));
    objects.push_back(new Netherrack(glm::vec3(-2.0f, -1.0f, -1.5f), glm::vec3(3.5f, -0.5f, -4.0f), netherrack));


    objects.push_back(new Stone(glm::vec3(-1.5f, -1.0f, 0.0f), glm::vec3(3.0f, -0.75f, -1.0f), stone));
    objects.push_back(new Stone(glm::vec3(-1.5f, -0.75f, -0.25f), glm::vec3(3.0f, -0.5f, -1.0f), stone));
    objects.push_back(new Stone(glm::vec3(-1.5f, -0.5f, -0.5f), glm::vec3(3.0f, -0.25f, -1.0f), stone));
    objects.push_back(new Stone(glm::vec3(-1.0f, -0.25f, -0.75f), glm::vec3(2.5f, 0.0f, -1.0f), stone));

    objects.push_back(new Stone(glm::vec3(3.0f, -1.0f, -0.5f), glm::vec3(3.5f, -0.75f, -0.75f), stone));
    objects.push_back(new Stone(glm::vec3(3.0f, -1.0f, -0.75f), glm::vec3(3.5f, -0.5f, -1.0f), stone));
    objects.push_back(new Stone(glm::vec3(3.0f, -1.0f, -1.0f), glm::vec3(3.5f, 2.0f, -1.5f), stone));

    objects.push_back(new Stone(glm::vec3(-2.0f, -1.0f, -0.5f), glm::vec3(-1.5f, -0.75f, -0.75f), stone));
    objects.push_back(new Stone(glm::vec3(-2.0f, -1.0f, -0.75f), glm::vec3(-1.5f, -0.5f, -1.0f), stone));
    objects.push_back(new Stone(glm::vec3(-2.0f, -1.0f, -1.0f), glm::vec3(-1.5f, 3.5f, -1.5f), stone));

    objects.push_back(new Stone(glm::vec3(-2.0f, 3.0f, -1.0f), glm::vec3(0.0f, 3.5f, -1.5f), stone));
    objects.push_back(new Stone(glm::vec3(-2.0f, 2.5f, -1.0f), glm::vec3(-1.0f, 3.0f, -1.5f), stone));

    objects.push_back(new Gold(glm::vec3(0.0f, 3.0f, -1.0f), glm::vec3(1.0f, 3.5f, -1.5f), gold)); 

    objects.push_back(new Obsidian(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.5f, 0.5f, -1.5f), obsidian)); 
    objects.push_back(new Obsidian(glm::vec3(-0.5f, 0.5f, -1.0f), glm::vec3(0.0f, 2.5f, -1.5f), obsidian)); 
    objects.push_back(new Obsidian(glm::vec3(1.5f, 0.5f, -1.0f), glm::vec3(2.0f, 2.5f, -1.5f), obsidian)); 
    objects.push_back(new Obsidian(glm::vec3(0.0f, 2.5f, -1.0f), glm::vec3(1.5f, 3.0f, -1.5f), obsidian)); 

    objects.push_back(new Portal(glm::vec3(0.0f, 0.5f, -1.0f), glm::vec3(1.5f, 2.5f, -1.5f), portal));

    objects.push_back(new Gold(glm::vec3(-1.5f, 0.0f, -3.0f), glm::vec3(0.5f, 2.1f, -4.0f), gold)); 

    objects.push_back(new Diamond(glm::vec3(1.0f, 0.0f, -3.0f), glm::vec3(3.0f, 2.1f, -4.0f), diamond));
    
}

void render() {
    float fov = 3.1415/3;
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {

            float screenX = (2.0f * (x + 0.5f)) / SCREEN_WIDTH - 1.0f;
            float screenY = -(2.0f * (y + 0.5f)) / SCREEN_HEIGHT + 1.0f;
            screenX *= ASPECT_RATIO;
            screenX *= tan(fov/2.0f);
            screenY *= tan(fov/2.0f);


            glm::vec3 cameraDir = glm::normalize(camera.target - camera.position);

            glm::vec3 cameraX = glm::normalize(glm::cross(cameraDir, camera.up));
            glm::vec3 cameraY = glm::normalize(glm::cross(cameraX, cameraDir));
            glm::vec3 rayDirection = glm::normalize(
                cameraDir + cameraX * screenX + cameraY * screenY
            );
           
            Color pixelColor = castRay(camera.position, rayDirection);

            point(glm::vec2(x, y), pixelColor);
        }
    }
}

int main(int argc, char* argv[]) {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Hello World - FPS: 0", 
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                          SCREEN_WIDTH, SCREEN_HEIGHT, 
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    ImageLoader::loadImage("grass", "../assets/grama.jpg", 800.0f, 800.0f);
    ImageLoader::loadImage("obsidian", "../assets/obsidian.jpg", 512.0f, 512.0f);
    ImageLoader::loadImage("portal", "../assets/portal.jpg", 160.0f, 160.0f);
    ImageLoader::loadImage("gold", "../assets/gold.jpg", 512.0f, 512.0f);
    ImageLoader::loadImage("diamond", "../assets/diamond.jpg", 300.0f, 300.0f);
    ImageLoader::loadImage("netherrack", "../assets/netherrack.jpeg", 400.0f, 400.0f);
    ImageLoader::loadImage("stone", "../assets/stone.png", 800.0f, 800.0f);

    ImageLoader::loadImage("upSky", "../assets/ceil.jpg", 4096.0f, 434.0f);
    ImageLoader::loadImage("sideSky", "../assets/skybox.jpg", 4096.0f, 2160.0f);
    ImageLoader::loadImage("floor", "../assets/floor.jpg", 1200.0f, 200.0f);

    bool running = true;
    SDL_Event event;

    int frameCount = 0;
    Uint32 startTime = SDL_GetTicks();
    Uint32 currentTime = startTime;
    
    setUp();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        camera.moveZ(-1.0f);
                        break;
                    case SDLK_DOWN:
                        camera.moveZ(1.0f);
                        break;
                    case SDLK_LEFT:
                        camera.rotate(-1.0f, 0.0f);
                        break;
                    case SDLK_RIGHT:
                        camera.rotate(1.0f, 0.0f);
                        break;
                    case SDLK_w:
                        camera.moveY(1.0f);
                        break;
                    case SDLK_s:
                        camera.moveY(-1.0f);
                        break;
                    case SDLK_a:
                        camera.moveX(-1.0f);
                        break;
                    case SDLK_d:
                        camera.moveX(1.0f);
                        break;
                 }
            }


        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        render();

        // Present the renderer
        SDL_RenderPresent(renderer);

        frameCount++;

        // Calculate and display FPS
        if (SDL_GetTicks() - currentTime >= 1000) {
            currentTime = SDL_GetTicks();
            std::string title = "FPS: " + std::to_string(frameCount);
            SDL_SetWindowTitle(window, title.c_str());
            frameCount = 0;
        }
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

