target_sources(taller_server
    PRIVATE
    # .cpp files
    main.cpp
    server.cpp
    acceptor.cpp
    server_protocol.cpp
    lobby/lobby.cpp
    match.cpp
    player.cpp
    lobby/lobby_receiver.cpp
    lobby/lobby_player.cpp
    receiver.cpp
    sender.cpp
    level_manager.cpp
    game/game.cpp
    game/game_queue_monitor.cpp
    game/game_map.cpp
    game/duck.cpp
    game/item.cpp
    game/spawn_place.cpp
    game/box.cpp
    game/round_manager.cpp
    guns/bullet.cpp
    guns/weapon.cpp
    guns/pew_pew_laser.cpp
    guns/laser.cpp
    guns/laser_rifle.cpp
    guns/bouncing_laser.cpp
    guns/ak_47.cpp
    guns/duel_pistol.cpp
    guns/cowboy_pistol.cpp
    guns/magnum.cpp
    guns/shotgun.cpp
    guns/sniper.cpp
    guns/grenade.cpp
    guns/grenade_fragment.cpp
    guns/banana.cpp
    armor/armor.cpp
    armor/helmet.cpp
    #guns/gun.cpp
    guns/projectile.cpp
    executables/start/move_left_command.cpp
    executables/start/move_right_command.cpp
    executables/start/move_down_command.cpp
    executables/start/move_up_command.cpp
    executables/start/look_up_command.cpp
    executables/stop/stop_move_command.cpp
    executables/stop/stop_move_right_command.cpp
    executables/stop/stop_move_left_command.cpp
    executables/stop/stop_jump_command.cpp
    executables/stop/stop_move_down_command.cpp
    executables/stop/stop_look_up_command.cpp
    executables/guns_commands/shoot_command.cpp
    executables/guns_commands/take_item_command.cpp
    executables/guns_commands/drop_weapon_command.cpp

    
    PUBLIC
    # .h files
    server.h
    acceptor.h
    server_protocol.h
    lobby/lobby.h
    match.h
    player.h
    lobby/lobby_receiver.h
    lobby/lobby_player.h
    receiver.h
    sender.h
    level_manager.h
    game/game.h
    game/game_queue_monitor.h
    game/game_map.h
    game/duck.h
    game/item.h
    game/spawn_place.h
    game/box.h
    game/round_manager.h
    guns/bullet.h
    guns/weapon.h
    guns/pew_pew_laser.h
    guns/laser.h
    guns/laser_rifle.h
    guns/bouncing_laser.h
    guns/ak_47.h
    guns/duel_pistol.h
    guns/cowboy_pistol.h
    guns/magnum.h
    guns/shotgun.h
    guns/sniper.h
    guns/grenade.h
    guns/grenade_fragment.h
    guns/banana.h
    armor/armor.h
    armor/helmet.h
    #guns/gun.h
    guns/projectile.h
    executables/executable.h
    executables/commands.h
    executables/start/move_left_command.h
    executables/start/move_right_command.h
    executables/start/move_down_command.h
    executables/start/move_up_command.h
    executables/start/look_up_command.h
    executables/stop/stop_move_command.h
    executables/stop/stop_move_right_command.h
    executables/stop/stop_move_left_command.h
    executables/stop/stop_jump_command.h
    executables/stop/stop_move_down_command.h
    executables/stop/stop_look_up_command.h
    executables/guns_commands/shoot_command.h
    executables/guns_commands/take_item_command.h
    executables/guns_commands/drop_weapon_command.h



    )
