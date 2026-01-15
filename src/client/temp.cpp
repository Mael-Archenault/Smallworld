#include <SFML/Network.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    ////////////////////////////////////////////////////////////////
    // Connect to localhost server on port 8888
    sf::Http http("http://localhost", 8888);

    // Create GET request for the root path
    sf::Http::Request request("/rooms/create");
    request.setMethod(sf::Http::Request::Get);

    // Send request
    sf::Http::Response response = http.sendRequest(request);

    // Print status and body
    std::cout << "Status: " << response.getStatus() << "\n";
    std::cout << "Body:\n" << response.getBody() << "\n";

    std::string session_token = response.getBody();

    ///////////////////////////////////////////////////////////////

    // // Create GET request for the root path
    // request = sf::Http::Request("/rooms/create");
    // request.setMethod(sf::Http::Request::Get);
    // request.setField("Session-Token", session_token);

    // // Send request
    // response = http.sendRequest(request);

    // // Print status and body
    // std::cout << "Status: " << response.getStatus() << "\n";
    // std::cout << "Body:\n" << response.getBody() << "\n";

    ///////////////////////////////////////////////////////////////
}