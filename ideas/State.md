# Game_State


From what I understood, the State that we are building is meant to have a reference in the Engine.
It means that everything that we will do to the state will be done "through" the Game_State class. It is like an "entry point" to the game state. We're basically making and "API" to the state of the game.

## Problem encountered
I thought about a lot of things, and what I discovered is that we cannot make a thing such as Game_State.play_round(player) and that because of one simple thing: a round contains several player interactions, however the game state is not meant to handle player inputs, it's the engine role.

## Solutions

So I thought about more general functions that does not involve the players interaction. (see methods in the Game_State class)

I also added ids to most of the classes, that way the engine will be able to target specific objects without "playing" with the reference

That's basically what I did to make this new version of the state diagram


## Method Pipe

Most of the functions inside of the Game_State class are some sort of pipeline to the actual data.

For example, the method Game_State.gather_free_units() works like this:  
- it is called in the engine, giving player_id, tribe_id
- the Game_State class will look for its list of players, find the matching id and call Player.gather_free_units(tribe_id)
- the Player class will look for its list of tribe, find the matching id and call tribe.gather_free_units()
- the tribe will iterate through all of its owned areas, and call area.gather_free_units()
- finally, the area.will call population.gather_free_units() to modify the number of units on the population
- the last function will return the number of gathered units. This number will be used as the return value for area.gather_free_units(). That way we will be able to modify the actual "free_units_number" variable in the Tribe class

Overall this function was called from the Game_State class, but traveled throughout almost the entire network of classes. I think this is the general way of doing a state class and this is what I did to make all methods of all classes.
