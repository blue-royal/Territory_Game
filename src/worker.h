#ifndef __WORKER__
#define __WORKER__

#include "raylib.h"
#include "utils.h"
#include "environment.h"

class Worker
{
    private:
        Model worker_model;
        Color team;
        Shader shader;
        Environment* env;
        
        Vector3 target;
        Vector3 goal;
        float speed;

    public:
        Worker();
        Worker(Color colour, Environment* environ);
        void update();
        void update_target(Vector3 new_target);
        void next_node();
        void draw();
        ~Worker();

        Vector3 position;
};

#endif
