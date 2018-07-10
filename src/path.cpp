#include "time.h"
#include <stdio.h>
#include <stdlib.h>

#include "path/camera.hpp"
#include "path/material.hpp"
#include "path/objects.hpp"
#include "path/renderer.hpp"
#include "path/scene.hpp"

using namespace rt::path;

int main(int argc, char* argv[]) {

  time_t start, stop;
  time(&start);    // Start execution timer
  int samples = 4; // Default samples per pixel

  if (argc == 2)
    samples = atoi(argv[1]);

  Camera camera = Camera(glm::dvec3(0, -5, 2.5), glm::dvec3(0, 0, 1), 1280,
                         720); // Create camera
  Scene scene = Scene();       // Create scene

  // Add objects to scene
  scene.add(new Sphere(glm::dvec3(0, 0, -1000), 1000, Material()));
  scene.add(new Sphere(glm::dvec3(-1004, 0, 0), 1000,
                       Material(DIFF, glm::dvec3(0.85, 0.4, 0.4))));
  scene.add(new Sphere(glm::dvec3(1004, 0, 0), 1000,
                       Material(DIFF, glm::dvec3(0.4, 0.4, 0.85))));
  scene.add(new Sphere(glm::dvec3(0, 1006, 0), 1000, Material()));
  scene.add(new Sphere(glm::dvec3(0, 0, 110), 100,
                       Material(EMIT, glm::dvec3(1, 1, 1), glm::dvec3(2.2, 2.2, 2.2))));
  scene.add(new Mesh(glm::dvec3(), "../obj/dragon2.obj",
                     Material(DIFF, glm::dvec3(0.9, 0.9, 0.9))));

  Renderer renderer =
      Renderer(&scene, &camera); // Create renderer with our scene and camera
  renderer.render(samples);      // Render image to pixel buffer
  renderer.save_image("render.png"); // Save image

  // Print duration information
  time(&stop);
  double diff = difftime(stop, start);
  int hrs = (int)diff / 3600;
  int mins = ((int)diff / 60) - (hrs * 60);
  int secs = (int)diff - (hrs * 3600) - (mins * 60);
  printf("\rRendering (%i samples): Complete!\nTime Taken: %i hrs, %i mins, %i "
         "secs\n\n",
         samples, hrs, mins, secs);
  return 0;
}
