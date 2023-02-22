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
        ${CMAKE_SOURCE_DIR}/lib/SDL2/win/libSDL2_ttf.dll.a        
    )
    configure_file(${CMAKE_SOURCE_DIR}/lib/SDL2/win/SDL2.dll SDL2.dll COPYONLY)
    configure_file(${CMAKE_SOURCE_DIR}/lib/SDL2/win/SDL2_ttf.dll SDL2_ttf.dll COPYONLY)    
endif (WIN32)

if (UNIX)
    find_package(SDL2 REQUIRED SDL2)
    target_link_libraries(BoxMover PRIVATE ${SDL2_LIBRARIES})
endif (UNIX)

configure_file(${CMAKE_SOURCE_DIR}/etc/stage.txt stage.txt COPYONLY)
file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)

