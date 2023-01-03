#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include <cmath>


#define GROUND_HEIGHT 0

std::vector<std::string> read_file(char *path);

enum Teams {red_team, blue_team};

bool ray_sphere_intersection(Ray ray, Vector3 center, float radius);
bool point_circle_intersection(Vector2 point, Vector2 center, float radius);
bool circle_circle_intersection(Vector2 center1, Vector2 center2, float radius1, float radius2);
Vector3 ray_ground_intersection(Ray ray);
Vector3 move_to_target(Vector3 position, Vector3 target, float speed);
Vector2 vec3_to_vec2(Vector3 vec);
Vector3 vec2_to_vec3_ground(Vector2 vec);
bool reached_target(Vector3 pos, Vector3 target);
