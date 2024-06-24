cmake_minimum_required(VERSION 3.16.3)
project(MagiskHluda)

set(CMAKE_CXX_STANDARD 20)


add_executable(MagiskHluda
        main.cpp
        utils.cpp
)

# Set the target architecture for Visual Studio (Win64)
if (MSVC)
    set_target_properties(MagiskHluda PROPERTIES
            VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE x64
    )
endif ()

# Find the required packages
find_package(restclient-cpp CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

# Link only utils.cpp to the required libraries
target_include_directories(MagiskHluda PUBLIC ${resclient-cpp_INCLUDE_DIRS} ${rapidjson_INCLUDE_DIRS})
target_link_libraries(MagiskHluda PUBLIC restclient-cpp rapidjson)
