add_library(BoxMoverLib OBJECT game.cpp)
target_include_directories(BoxMoverLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(BoxMoverLib -lpthread -lgcov --coverage)
aux_source_directory(. DIR_SRCS)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")
add_executable (BoxMover ${DIR_SRCS})
target_compile_features(BoxMover PRIVATE cxx_std_17)

if (WIN32)
    target_link_libraries(BoxMover PRIVATE 
        mingw32 
        ${CMAKE_SOURCE_DIR}/lib/SDL2/win/libSDL2main.a
        ${CMAKE_SOURCE_DIR}/lib/SDL2/win/SDL2.dll
    )
    configure_file(${CMAKE_SOURCE_DIR}/lib/SDL2/win/SDL2.dll SDL2.dll COPYONLY)
endif (WIN32)

if (UNIX)
    target_link_libraries(BoxMover PRIVATE 
        ${CMAKE_SOURCE_DIR}/lib/SDL2/unix/libSDL2main
        ${CMAKE_SOURCE_DIR}/lib/SDL2/unix/libSDL2
    )
endif (UNIX)

configure_file(${CMAKE_SOURCE_DIR}/etc/stage.txt stage.txt COPYONLY)