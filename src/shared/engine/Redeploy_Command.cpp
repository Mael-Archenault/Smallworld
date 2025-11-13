#include "engine.h"
#include "state.h"

namespace engine{

    const int Command::id = 2;

    Redeploy_Command::Redeploy_Command(int player_id,
                                    int area_id,
                                    int added_units):

                                    Command(player_id),
                                    area_id(area_id),
                                    added_units(added_units){}



    void Redeploy_Command::execute(state::Game_State& state){
        state.redeploy_units(player_id,
                        area_id,
                        added_units);
    };  


}