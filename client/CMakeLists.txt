target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    client.cpp
    client_protocol.cpp
    client_receiver.cpp
    client_sender.cpp
    client_inputhandler.cpp
    PUBLIC
    # .h files
    client.h
    client_protocol.h
    client_receiver.h
    client_sender.h
    client_inputhandler.h
    )

# Path de imagenes
target_compile_definitions(taller_client PRIVATE IMAGE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/imgs/")