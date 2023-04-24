cmake_minimum_required(VERSION 3.22.1)
project(low_esotericism)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -O3 -ggdb")

include(FetchContent)

set(DOWNLOAD_EXTRACT_TIMESTAMP "true")
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(low_esotericism main.cpp task.cpp task.h kernels.cpp kernels.h cmd_args_parser.cpp cmd_args_parser.h input_request.h solver.cpp solver.h progress_counter.cpp progress_counter.h)

target_link_libraries(low_esotericism PRIVATE nlohmann_json::nlohmann_json)
