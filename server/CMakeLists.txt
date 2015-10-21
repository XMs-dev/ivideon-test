set(PROJECT server)

cmake_minimum_required(VERSION 2.8.11)
project(${PROJECT})

set(CMAKE_INCLUDE_CURRENT_DIR true)

set(${PROJECT}_SRCS FIFO.cpp
                    FileMon.cpp
                    FileStream.cpp
                    LEDFakeDevice.cpp
                    Server.cpp
                    main.cpp)

add_executable(${PROJECT} ${${PROJECT}_SRCS})
target_link_libraries(${PROJECT} )

install(TARGETS ${PROJECT} RUNTIME DESTINATION bin)
