# Server API Description

## Routes


- /rooms/create | POST (create a room)
- /rooms/join{room_id} | POST (join a room)
- /rooms/start/{room_id} | POST (start a game)
- /rooms/state/{room_id} | GET (get the lobby state)
- /rooms/add_ai/{room_id} | POST (add an ai)
- /rooms/delete_ai/{room_id} | POST (remove an ai)



- /game/command/{room_id} | POST (give a command to a game)
- /game/state/{room_id} | GET (get the state of a game)
- /game/version/{room_id} | GET (get the state version of a game)