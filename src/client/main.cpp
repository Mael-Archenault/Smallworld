#include <iostream>

#include "client.h"
#include "engine.h"

// The following lines are here to check that SFML is installed and working
#include <unistd.h>

#include <SFML/Graphics.hpp>
#include <mutex>
#include <thread>

// end of test SFML

int main(int argc, char* argv[])
{
    client::Client client;
    client.run();
}
