#include "engine.h"

namespace engine{
    Engine::Engine(){};

    void Engine::add_command(Command command){
        command_queue.push(command);
    }

    void Engine::update(){
        
    }

}