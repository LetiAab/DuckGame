target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    client.cpp
    client_receiver.cpp
    client_sender.cpp
    client_inputhandler.cpp
    client_protocol.cpp
    client_game_map.cpp
    sdl_handler.cpp
        sdl_texturehandler.cpp
        sdl_eventprocessor.cpp
        game_initializer.cpp
        sdl_renderermanager.cpp
        sdl_screenmanager.cpp
        sdl_audiomanager.cpp
        camera.cpp
    PUBLIC
    # .h files
    client.h
    client_receiver.h
    client_sender.h
    client_inputhandler.h
    client_protocol.h
    sdl_handler.h
    client_game_map.h
        sdl_texturehandler.h
        sdl_eventprocessor.h
        game_state.h
        game_initializer.h
        sdl_renderermanager.h
        sdl_screenmanager.h
        sdl_audiomanager.h
        camera.h
    )

# Path de imagenes
target_compile_definitions(taller_client PRIVATE IMAGE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/imgs/")

#Path de fuentes
target_compile_definitions(taller_client PRIVATE FONT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/fonts/")

#Path de audios
target_compile_definitions(taller_client PRIVATE AUDIO_PATH="${CMAKE_CURRENT_SOURCE_DIR}/audio/")

