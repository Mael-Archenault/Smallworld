#include "Area.h"
#include "Tribe.h"
#include "Halflings_Species.h"

using namespace state;

Halflings_Species::Halflings_Species():Species_Description("Halflings",6,8){}

void Halflings_Species::apply_conquest_effect  (Area* area){
    if(dens_placed<2){
        area->set_special_tokens(Area_Special_Token::HOLE_IN_THE_GROUND);
        dens_placed++;
    }
}
bool Halflings_Species::is_area_impenetrable(Area* area){
    for (auto token : area->get_special_tokens()) {
        if (token == state::Area_Special_Token::HOLE_IN_THE_GROUND) {
            return true; 
        }
    }
    return false;
}

void Halflings_Species::on_abandon_area(Area* area){
    
}

