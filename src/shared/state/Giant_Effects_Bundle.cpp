//
// Created by vboxuser on 10/17/25.
//

#include "Giant_Effects_Bundle.h"

state::Giant_Effects_Bundle::Giant_Effects_Bundle() = default;
void state::Giant_Effects_Bundle::apply_first_round_effect (){}
void state::Giant_Effects_Bundle::apply_conquest_effect (){}
void state::Giant_Effects_Bundle::apply_lose_effect (){}
int state::Giant_Effects_Bundle::get_bonus_rewards (state::Area& area){return 0;}