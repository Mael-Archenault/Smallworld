#include <SFML/Network.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    ////////////////////////////////////////////////////////////////
    // Connect to localhost server on port 8888
    sf::Http http("http://localhost", 8888);

    sf::Http::Request request("/connect");
    request.setMethod(sf::Http::Request::Post);

    // Send request
    sf::Http::Response response = http.sendRequest(request);

    // Print status and body
    std::cout << "Session token : " << response.getBody() << "\n";

    std::string session_token = response.getBody();

    ///////////////////////////////////////////////////////////////

    request = sf::Http::Request("/rooms/create");
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    // Send request
    response = http.sendRequest(request);

    // Print status and body
    std::cout << "Created room id: " << response.getBody() << "\n";

    ///////////////////////////////////////////////////////////////

    request = sf::Http::Request("/rooms/join/0");
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    // Send request
    response = http.sendRequest(request);

    // Print status and body
    // std::cout << "Created room id: " << response.getBody() << "\n";
}