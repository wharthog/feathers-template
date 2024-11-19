cmake_minimum_required(VERSION 3.16.0)
project(feathers-cpp-client)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-g -Wall -O3 -fno-strict-aliasing -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG)

find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)
find_package(TBB REQUIRED)
find_package(Boost 1.71 REQUIRED COMPONENTS system thread)

include_directories(
        include
        include/pnt
        include/pnt/common
        include/pnt/common/exchange
        include/pnt/common/feathers
        include/pnt/common/feathers/client
        include/pnt/common/nbbo
        ${CMAKE_CURRENT_BINARY_DIR}
        ${Boost_INCLUDE_DIRS}
        ${Protobuf_INCLUDE_DIRS}
)

add_executable(feathers-cpp-client feathersClientTest.cpp

)

target_link_libraries(feathers-cpp-client
        /Users/jakebattista/Downloads/feathers-cpp-client/lib/libfeathersclient.a
        protobuf::libprotobuf
        ${LIBPQXX_LIBRARIES}
        ${Boost_LIBRARIES}
        Threads::Threads
        TBB::tbb
)