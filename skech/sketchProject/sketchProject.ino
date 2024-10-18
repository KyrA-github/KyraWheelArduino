#include "src/Core.hpp"

Core* core = nullptr; 

void setup()
{
    core = new Core(); 
}

void loop()
{
    if (core != nullptr) {
        core->gameLoop();  
    }
}
