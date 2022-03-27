cmake_minimum_required(VERSION 2.5)

project(poco-http-server-lab1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(poco-http-server-lab1
    src/handlers/Factory.cpp
    src/handlers/PersonRequest.cpp
    src/main.cpp
    src/Server.cpp
    database/PersonEntity.cpp
)

find_package(Poco REQUIRED Foundation Util Net XML JSON DataMySQL Data)

target_link_libraries(poco-http-server-lab1
    PocoUtil
    PocoNet
    PocoXML
    PocoFoundation
    PocoJSON
    PocoData
    PocoDataMySQL
    ${CMAKE_THREAD_LIBS_INIT}
)
