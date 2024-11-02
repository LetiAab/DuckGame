target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    client.cpp
    client_receiver.cpp
    client_sender.cpp
    client_inputhandler.cpp
    client_protocol.cpp
    sdl_handler.cpp
    PUBLIC
    # .h files
    client.h
    client_receiver.h
    client_sender.h
    client_inputhandler.h
    client_protocol.h
    sdl_handler.h
    )

# Path de imagenes
target_compile_definitions(taller_client PRIVATE IMAGE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/imgs/")