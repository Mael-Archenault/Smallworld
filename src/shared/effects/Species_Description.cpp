#include "effects/Species_Description.h"

namespace effects
{

Species_Description::Species_Description(std::string name, int given_units_number,
                                         int max_units_number)
    : name(name), given_units_number(given_units_number), max_units_number(max_units_number)
{
}

int Species_Description::get_initial_units_number()
{
    return given_units_number;
}

std::string Species_Description::get_name()
{
    return name;
}

void Species_Description::to_json(Json::Value& root)
{
    root["name"]               = name;
    root["given_units_number"] = given_units_number;
    root["max_units_number"]   = max_units_number;
}

void Species_Description::from_json(Json::Value& root)
{
    name               = root["name"].asString();
    given_units_number = root["given_units_number"].asInt();
    max_units_number   = root["max_units_number"].asInt();
}
}  // namespace effects