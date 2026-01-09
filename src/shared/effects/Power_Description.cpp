
#include "effects/Power_Description.h"

namespace effects
{

Power_Description::Power_Description(std::string name, int given_units_number)
    : name(name), given_units_number(given_units_number)
{
}

int Power_Description::get_initial_units_number()
{
    return given_units_number;
}

std::string Power_Description::get_name()
{
    return name;
}

void Power_Description::to_json(Json::Value& root)
{
    root["name"]               = name;
    root["given_units_number"] = given_units_number;
}

void Power_Description::from_json(Json::Value& root)
{
    name               = root["name"].asString();
    given_units_number = root["given_units_number"].asInt();
}
}  // namespace effects
