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
    receiver.cpp
    sender.cpp
    game/game.cpp
    lobby/lobby_player.cpp
    game/game_queue_monitor.cpp
    game/game_map.cpp
    executables/move_left_command.cpp
    executables/move_right_command.cpp
    executables/move_down_command.cpp
    executables/move_up_command.cpp


    
    PUBLIC
    # .h files
    server.h
    acceptor.h
    server_protocol.h
    lobby/lobby.h
    match.h
    player.h
    lobby/lobby_receiver.h
    receiver.h
    sender.h
    game/game.h
    lobby/lobby_player.h
    game/game_queue_monitor.h
    executables/executable.h
    game/game_map.h
    executables/move_left_command.h
    executables/move_right_command.h
    executables/move_down_command.h
    executables/move_up_command.h
    )
