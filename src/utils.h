#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include <cmath>

#define GROUND_HEIGHT 0

std::vector<std::string> read_file(char *path);

bool ray_sphere_intersection(Ray ray, Vector3 center, float radius);
Vector3 ray_ground_intersection(Ray ray);
Vector3 move_to_target(Vector3 position, Vector3 target, float speed);
