target_sources(taller_editor
    PRIVATE
    # .cpp files
    main.cpp
    level_editor.cpp
    PUBLIC
    # .h files
    level_editor.h
    )

# Path de imagenes
target_compile_definitions(taller_client PRIVATE IMAGE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/imgs/")
