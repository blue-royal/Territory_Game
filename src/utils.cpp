#include "utils.h"

std::vector<std::string> read_file(char* path){
    std::fstream newfile;
    std::vector<std::string> new_text;
    newfile.open(path,std::ios::in); //open a file to perform read operation using file object
    if (newfile.is_open()){ //checking whether the file is open
        std::string tp;
        
        while(std::getline(newfile, tp)){ //read data from file object and put it into string.
            new_text.push_back(tp);
        }
      newfile.close(); //close the file object.
   }
    return new_text;
}

bool ray_sphere_intersection(Ray ray, Vector3 center, float radius){
    // sets up equation in for x^2 + bx + x = 0
    
    Vector3 ray_to_sphere = Vector3Add(ray.position, Vector3Negate(center));
    float b = 2.0f * Vector3DotProduct(Vector3Normalize(ray.direction), ray_to_sphere);
    float c = std::pow(Vector3Length(ray_to_sphere), 2.0f) - std::pow(radius, 2.0f);

    // if discriminant is greater than zero then they intersect
    if (std::pow(b, 2) - (4*c) >= 0.0f){
        return true;
        std::cout << "worked" << std::endl;
    } 
    return false;
}

Vector3 ray_ground_intersection(Ray ray){
    float dist = GROUND_HEIGHT-ray.position.y / ray.direction.y;
    return Vector3Add(ray.position, Vector3Scale(ray.direction, dist));
}

bool point_circle_intersection(Vector2 point, Vector2 center, float radius){
    if (Vector2Distance(point, center) <= radius){
        return true;
    }
    return false;
}

bool circle_circle_intersection(Vector2 center1, Vector2 center2, float radius1, float radius2){
    if (Vector2Distance(center1, center2) < radius1 + radius2){
        return true;
    }
    return false;
}

Vector3 move_to_target(Vector3 position, Vector3 target, float speed){
    return Vector3Add(position, Vector3Scale( Vector3Normalize( Vector3Subtract(target, position) ), speed * GetFrameTime()));
}

Vector2 vec3_to_vec2(Vector3 vec){
    return (Vector2){ vec.x, vec.z };
}

Vector3 vec2_to_vec3_ground(Vector2 vec){
    return (Vector3){ vec.x, GROUND_HEIGHT, vec.y };
}

bool reached_target(Vector3 pos, Vector3 target){
    return abs(pos.x - target.x) < 0.05f && abs(pos.z - target.z) < 0.05f ;
}

Vector2 centralise_vec2(Vector2 vec){
    return (Vector2){ (float)((int)vec.x)+0.5f, (float)((int)vec.y)+0.5f };
}
