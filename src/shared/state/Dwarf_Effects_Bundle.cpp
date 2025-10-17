//
// Created by vboxuser on 10/17/25.
//
#include "Dwarf_Effects_Bundle.h"

state::Dwarf_Effects_Bundle::Dwarf_Effects_Bundle() = default;
void state::Dwarf_Effects_Bundle::apply_first_round_effect (){}
void state::Dwarf_Effects_Bundle::apply_conquest_effect (){}
void state::Dwarf_Effects_Bundle::apply_lose_effect (){}
int state::Dwarf_Effects_Bundle::get_bonus_rewards (state::Area& area){return 0;}