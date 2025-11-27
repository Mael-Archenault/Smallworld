#include "Power_Description.h"
#include "Tribe.h"

using namespace state;

Power_Description::Power_Description(std::string name, int given_units_number)
: name(name), given_units_number(given_units_number){}

int Power_Description::get_initial_units_number(){
      return given_units_number;
}

std::string Power_Description::get_name(){
    return name;
}
