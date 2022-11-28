#ifndef __WORKER__
#define __WORKER__

#include "raylib.h"
#include "utils.h"

class Worker
{
    private:
        Model worker_model;
        Color team;
        Shader shader;
        Vector3 position;
        Vector3 target;
        float speed;

    public:
        Worker(Color colour);
        void update();
        void update_target(Vector3 new_target);
        void draw();
        ~Worker();
        
};

#endif
