target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    PUBLIC
    # .h files
    )

# Path de imagenes
target_compile_definitions(taller_client PRIVATE IMAGE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/imgs/")