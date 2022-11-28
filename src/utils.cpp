#include "utils.h"

std::vector<std::string> read_file(char *path){
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
    float c = std::powf(Vector3Length(ray_to_sphere), 2.0f) - std::powf(radius, 2.0f);
    std::cout << "running check" << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    std::cout << std::powf(b, 2) - (4*c) << std::endl;

    // if discriminant is greater than zero then they intersect
    if (std::powf(b, 2) - (4*c) >= 0.0f){
        return true;
        std::cout << "worked" << std::endl;
    } 
    return false;
}

Vector3 ray_ground_intersection(Ray ray){
    float dist = GROUND_HEIGHT-ray.position.y / ray.direction.y;
    return Vector3Add(ray.position, Vector3Scale(ray.direction, dist));
}

Vector3 move_to_target(Vector3 position, Vector3 target, float speed){
    return Vector3Add(position, Vector3Scale( Vector3Normalize( Vector3Subtract(target, position) ), speed));
}

