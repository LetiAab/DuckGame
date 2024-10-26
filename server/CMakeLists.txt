target_sources(taller_server
    PRIVATE
    # .cpp files
    main.cpp
    server.cpp
    acceptor.cpp
    server_protocol.cpp
    lobby.cpp
    match.cpp
    player.cpp
    lobby_receiver.cpp
    receiver.cpp
    sender.cpp
    
    PUBLIC
    # .h files
    server.h
    acceptor.h
    server_protocol.h
    lobby.h
    match.h
    player.h
    lobby_receiver.h
    receiver.h
    sender.h
    )
