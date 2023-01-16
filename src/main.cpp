#include "game.h"
#include "menu.h"
#include <iostream>

#define SERVER 0
#define CLIENT 1

int main() {

    InitWindow(800, 500, "Territory");

    int res = run_menu();
    if (res == SERVER)
    {
        // Game* territory_game = new Game();
        // territory_game->run_game();

        // delete territory_game;

        Server server = Server(1843);
        server.recieve();
    
    } 
    else if (res == CLIENT){
        // Game* territory_game = new Game();
        // territory_game->run_game();

        // delete territory_game;
        Client client = Client(1843);
        Serialise to_test;
        to_test = to_test << (int) 258 << true << false << 12.75f;
        client.send(to_test.get_bytes());
    }

    return 0;
}
