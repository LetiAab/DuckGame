target_sources(taller_common
    PRIVATE
    # .cpp files
    foo.cpp
    liberror.cpp
    resolver.cpp
    resolvererror.cpp
    socket.cpp
    PUBLIC
    # .h files
    foo.h
    liberror.h
    resolver.h
    resolvererror.h
    socket.h
    queue.h
    thread.h
    lobby_message.h
    lobby_command.h
    )
