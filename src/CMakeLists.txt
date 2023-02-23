add_library(BoxMoverLib OBJECT game.cpp)
target_include_directories(BoxMoverLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(BoxMoverLib -lpthread -lgcov --coverage)

aux_source_directory(. DIR_SRCS)
add_executable (BoxMover ${DIR_SRCS})
target_compile_features(BoxMover PRIVATE cxx_std_17)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")

if (WIN32)
    target_link_libraries(BoxMover PRIVATE 
        mingw32 
        ${CMAKE_SOURCE_DIR}/lib/SDL2/win/libSDL2main.a
        ${CMAKE_SOURCE_DIR}/lib/SDL2/win/SDL2.dll
        ${CMAKE_SOURCE_DIR}/lib/SDL2/win/libSDL2_ttf.dll.a        
    )
    set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -mwindows")
    set(CMAKE_EXE_LINKER_FLAGS "-static")        
    configure_file(${CMAKE_SOURCE_DIR}/lib/SDL2/win/SDL2.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SDL2.dll COPYONLY)
    configure_file(${CMAKE_SOURCE_DIR}/lib/SDL2/win/SDL2_ttf.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SDL2_ttf.dll COPYONLY)
endif (WIN32)

if (UNIX)
    INCLUDE(FindPkgConfig)
    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
    SET(CMAKE_CXX_STANDARD 17)
    SET(CMAKE_CXX_STANDARD_REQUIRED ON)
    target_link_libraries(BoxMover PRIVATE ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES})
endif (UNIX)

configure_file(${CMAKE_SOURCE_DIR}/etc/stage.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/stage.txt COPYONLY)
file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/)

