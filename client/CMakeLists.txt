target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    client.cpp
    client_protocol.cpp
    client_receiver.cpp
    client_sender.cpp
    PUBLIC
    # .h files
    client.h
    client_protocol.h
    client_receiver.h
    client_sender.h
    )
