#include "game.h"

Game::Game(){
    std::cout << "new game" << std::endl;

}

void Game::initialise_game(){

    env = new Environment((char*)"assets/scene/main_level.txt");
    sprites.push_back(new Archer((Vector3){ 12.0f, 0.0f, 10.0f }, Teams::blue_team, env));
    sprites.push_back(new Archer((Vector3){ 14.0f, 0.0f, 10.0f }, Teams::blue_team, env));
    sprites.push_back(new Worker((Vector3){ 13.0f, 0.0f, 10.0f }, Teams::blue_team, env));
    sprites.push_back(new Worker((Vector3){ 15.0f, 0.0f, 10.0f }, Teams::blue_team, env));
    sprites.push_back(new Worker((Vector3){ 16.0f, 0.0f, 10.0f }, Teams::blue_team, env));

    sprites.push_back(new Archer((Vector3){ 5.0f, 0.0f, 10.0f }, Teams::red_team, env));
    sprites.push_back(new Archer((Vector3){ 5.0f, 0.0f, 11.0f }, Teams::red_team, env));
    sprites.push_back(new Worker((Vector3){ 5.0f, 0.0f, 12.0f }, Teams::red_team, env));
    sprites.push_back(new Worker((Vector3){ 5.0f, 0.0f, 13.0f }, Teams::red_team, env));
    sprites.push_back(new Worker((Vector3){ 5.0f, 0.0f, 14.0f }, Teams::red_team, env));


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    // Define the camera to look into 3D world
    camera.position = (Vector3){ 20.0f, 10.0f, 20.0f }; // Camera position
    camera.target = (Vector3){ 10.0f, 0.0f, 10.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 20.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_ORTHOGRAPHIC; 
}

void Game::update_draw(){

    // Update
    for (Sprite* i : sprites) {
        i->update(sprites);
    }

    // Draw
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode3D(camera);
            env->draw();

            for (Sprite* i : sprites) {
                i->draw();
            }

        EndMode3D();
        for (Sprite* i : sprites) {
            i->draw_health_bar(camera);
        }
        // draw the number of resources
        DrawText(std::to_string((int)points).c_str(), 10, 10, 30, BLACK);
    EndDrawing();
}

void Game::events(Teams team){
    // Events
    Vector3 ground_intersect;
    Vector2 corner2;

    Ray ray = { 0 };

    
    ray = GetMouseRay(GetMousePosition(), camera);
    if (IsMouseButtonPressed(1)){
        ground_intersect = ray_ground_intersection(ray);
        if(env->valid_target((Vector2){ ground_intersect.x, ground_intersect.z })){
            ground_intersect.y += 0.5f;
            for (Sprite* i : sprites) {
                if (i->team == team){
                    if (i->Type == Sprite_Type::worker_unit){
                        auto TypedI = (Worker*) i;
                        if (TypedI->selected){
                            TypedI->update_target(ground_intersect);
                        }
                    }
                    else if (i->Type == Sprite_Type::archer_unit){
                        auto TypedI = (Archer*) i;
                        if (TypedI->selected){
                            TypedI->update_target(ground_intersect);
                        }
                    } 
                }
            }
        }
    }

    if (IsMouseButtonPressed(0)){
        corner1 = GetMousePosition();
    }
    if (IsMouseButtonReleased(0)){
        corner2 = GetMousePosition();

        bool keep_current_selected = IsKeyDown(KEY_LEFT_SHIFT);
        
        for (Sprite* i : sprites) {
            if(i->team == team){
                i->is_selected(corner1, corner2, camera, keep_current_selected);
            }
        }
    }

    if (IsKeyPressed(KEY_W)){
        for (Sprite* i : sprites) {
            if(i->team == team){
                if (i->Type == Sprite_Type::worker_unit){
                    auto typedI = (Worker*) i;
                    if (typedI->selected){
                        typedI->new_mine_area();
                    }
                }
            }
        }
    }

    if (IsKeyPressed(KEY_A)){
        for (Sprite* i : sprites) {
            if(i->team == team){
                if (i->Type == Sprite_Type::archer_unit){
                    auto typedI = (Archer*) i;
                    if (typedI->selected){
                        typedI->toggle_attack();
                    }
                }
            }
        }
    }

    // add points when the timer expires
    if(points_timer <= 0.0f){
        if(team == Teams::red_team){
            points += env->get_number_of(Tiles::red_tile);
        }else if(team == Teams::blue_team){
            points += env->get_number_of(Tiles::blue_tile);
        }
        points_timer = point_countdown;
    } else{
        points_timer -= GetFrameTime();
    }


}

void Game::delete_sprite(unsigned int index){
    switch (sprites[index]->Type)
        {
        case Sprite_Type::worker_unit:
            delete (Worker*) sprites[index];
            break;
        case Sprite_Type::archer_unit:
            delete (Archer*) sprites[index];
            break;
        default:
            break;
        }
        sprites.erase(sprites.begin()+index);
}

Game::~Game(){
    delete env;
    
    for (Sprite* i : sprites) {
        
        switch (i->Type)
        {
        case Sprite_Type::worker_unit:
            delete (Worker*) i;
            break;
        case Sprite_Type::archer_unit:
            delete (Archer*) i;
            break;
        
        default:
            break;
        }
    }
}

void C_Game::run_game(){
    std::cout << "running" << std::endl;
    initialise_game();
    init_network();
    while (!WindowShouldClose()){ // Detect window close button or ESC key
        network();
        events(Teams::red_team);
        update_draw();
    }

    CloseWindow();
}

void C_Game::init_network(){
    client = Client(12531);
}

void C_Game::network(){
    //send
    Serialise pack;

    unsigned int counter = 0;
    for(Sprite* i: sprites){
        if(i->team == Teams::blue_team){
            if (i->health <= 0.0f){
                delete_sprite(counter);
                pack = pack << (int) Network_Headers::delete_sprite << (int)counter + 1;
            }
        }
        counter++;
    }

     if (IsKeyPressed(KEY_Q)){
        if(points > WORKER_COST){
            std::cout << "new worker made" << std::endl;
            points -= WORKER_COST;
            sprites.push_back(new Worker(spawn, Teams::red_team, env));
            pack = pack << (int)Network_Headers::add_worker;
        }

    }

    if (IsKeyPressed(KEY_S)){
        if(points > ARCHER_COST){
            std::cout << "new archer made" << std::endl;
            points -= ARCHER_COST;
            sprites.push_back(new Archer(spawn, Teams::red_team, env));
            pack = pack << (int)Network_Headers::add_archer;
        }
    }

    pack = pack << (int)Network_Headers::update_sprites;
    for(Sprite* i: sprites){
        if(i->team == Teams::red_team){
            pack = pack << i->position.x << i->position.z;
        }
    }
    for(Sprite* i: sprites){
        if(i->team == Teams::blue_team){
            pack = pack << i->health;
        }
    }

    pack = pack << (int)Network_Headers::update_terrain;
    Tile_Change changed_tile =  env->get_changed_tile();
    pack = pack << (int)changed_tile.pos << (int)changed_tile.new_type;

    client.write(pack.get_bytes());

    // recieve
    std::vector<Byte> bytes = client.recieve();

    Deserialise unpack(bytes);
    Network_Headers section_type = (Network_Headers) unpack.get_int();

    while (section_type == Network_Headers::delete_sprite){
        int index = unpack.get_int() - 1;
        if (index != -1){
            delete_sprite(index);
        }
        section_type = (Network_Headers) unpack.get_int();
    }

    while (section_type == Network_Headers::add_worker){
        std::cout << "new worker" << std::endl;
        sprites.push_back(new Worker(other_spawn, Teams::blue_team, env));
        section_type = (Network_Headers) unpack.get_int();
    }

    while(section_type == Network_Headers::add_archer){
        std::cout << "new archer" << std::endl;
        sprites.push_back(new Archer(other_spawn, Teams::blue_team, env));
        section_type = (Network_Headers) unpack.get_int();
    }

    if (section_type == Network_Headers::update_sprites){
        for(Sprite* i: sprites){
            if (i->team == Teams::blue_team){
                if(unpack.get_next_type() != Data_Types::finished){
                    float temp;
                    temp = unpack.get_real();
                    if(temp != 0.0f){
                        i->position.x = temp;
                    }
                    temp = unpack.get_real();
                    if(temp != 0.0f){
                        i->position.z = temp;
                    }
                }
            }
        }
        for(Sprite* i: sprites){
            if (i->team == Teams::red_team){
                if(unpack.get_next_type() != Data_Types::finished){
                    float temp;
                    temp = unpack.get_real();
                    if(temp != 0.0f){
                        i->health = temp;
                    }
                }
            }
        }
    }
    if(unpack.get_int() == (int)Network_Headers::update_terrain){
        while (unpack.get_next_type() != Data_Types::finished){
            int pos = unpack.get_int();
            int type = unpack.get_int();
            if(pos != 0 && type != 0){
                env->update_tile(pos, (Tiles)type);
            }
        }
    }
}



void S_Game::run_game(){
    std::cout << "running" << std::endl;
    initialise_game();
    init_network();
    while (!WindowShouldClose()){ // Detect window close button or ESC key
        network();
        events(Teams::blue_team);
        update_draw();
    }

    CloseWindow();
}

void S_Game::init_network(){
    server = Server(12531);
}

void S_Game::network(){
    //send
    Serialise pack;

    unsigned int counter = 0;
    for(Sprite* i: sprites){
        if(i->team == Teams::red_team){
            if (i->health <= 0.0f){
                delete_sprite(counter);
                pack = pack << (int) Network_Headers::delete_sprite << (int)counter + 1;
            }
        }
        counter++;
    }

    if (IsKeyPressed(KEY_Q)){
        if(points > WORKER_COST){
            std::cout << "new worker made" << std::endl;
            points -= WORKER_COST;
            sprites.push_back(new Worker(spawn, Teams::blue_team, env));
            pack = pack << (int)Network_Headers::add_worker;
        }

    }

    if (IsKeyPressed(KEY_S)){
        if(points > ARCHER_COST){
            std::cout << "new archer made" << std::endl;
            points -= ARCHER_COST;
            sprites.push_back(new Archer(spawn, Teams::blue_team, env));
            pack = pack << (int)Network_Headers::add_archer;
        }
    }

    pack = pack << (int)Network_Headers::update_sprites;
    for(Sprite* i: sprites){
        if(i->team == Teams::blue_team){
            pack = pack << i->position.x << i->position.z;
        }
    }
    for(Sprite* i: sprites){
        if(i->team == Teams::red_team){
            pack = pack << i->health;
        }
    }

    // send environment updates
    pack = pack << (int)Network_Headers::update_terrain;
    Tile_Change changed_tile =  env->get_changed_tile();
    pack = pack << (int)changed_tile.pos << (int)changed_tile.new_type;

    server.write(pack.get_bytes());

    //recieve
    std::vector<Byte> bytes = server.recieve();

    Deserialise unpack(bytes);
    Network_Headers section_type = (Network_Headers) unpack.get_int();

    while (section_type == Network_Headers::delete_sprite){
        int index = unpack.get_int() - 1;
        if (index != -1){
            delete_sprite(index);
        }
        
        section_type = (Network_Headers) unpack.get_int();
    }

    while (section_type == Network_Headers::add_worker){
        std::cout << "new worker" << std::endl;
        sprites.push_back(new Worker(other_spawn, Teams::red_team, env));
        section_type = (Network_Headers) unpack.get_int();
    }

    while(section_type == Network_Headers::add_archer){
        std::cout << "new archer" << std::endl;
        sprites.push_back(new Archer(other_spawn, Teams::red_team, env));
        section_type = (Network_Headers) unpack.get_int();
    }


    if (section_type == Network_Headers::update_sprites){
        for(Sprite* i: sprites){
            if (i->team == Teams::red_team){
                if(unpack.get_next_type() != Data_Types::finished){
                    float temp;
                    temp = unpack.get_real();
                    if(temp != 0.0f){
                        i->position.x = temp;
                    }
                    temp = unpack.get_real();
                    if(temp != 0.0f){
                        i->position.z = temp;
                    }
                }
            }
        }
        for(Sprite* i: sprites){
            if (i->team == Teams::blue_team){
                if(unpack.get_next_type() != Data_Types::finished){
                    float temp;
                    temp = unpack.get_real();
                    if(temp != 0.0f){
                        i->health = temp;
                    }
                }
            }
        }
    }

    if(unpack.get_int() == (int)Network_Headers::update_terrain){
        while (unpack.get_next_type() != Data_Types::finished){
            int pos = unpack.get_int();
            int type = unpack.get_int();
            if(pos != 0 && type != 0){
                env->update_tile(pos, (Tiles)type);
            }
        }
    }
}
void L_Game::run_game(){
    std::cout << "running" << std::endl;
    initialise_game();
    while (!WindowShouldClose()){ // Detect window close button or ESC key
        events(Teams::blue_team);
        events(Teams::red_team);
        update_draw();
    }

    CloseWindow();
}
