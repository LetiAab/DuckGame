target_sources(taller_server
    PRIVATE
    # .cpp files
    main.cpp
    server.cpp
    acceptor.cpp
    server_protocol.cpp
    PUBLIC
    # .h files
    server.h
    acceptor.h
    server_protocol.h
    )
