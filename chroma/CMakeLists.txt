cmake_minimum_required (VERSION 2.8.11)

project (CHROMA_KEY)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS " 'chroma' will be generated ")
add_executable (chroma chroma.cpp image.cpp)

## Link BOOST
message(STATUS " including boost library")
include_directories(${Boost_INCLUDE_DIR})
find_package(
  Boost 1.40
  COMPONENTS
  program_options
  REQUIRED
)
message(STATUS " linking boost library")
target_link_libraries(chroma ${Boost_LIBRARIES})

## Link GIL / PNG
message(STATUS " including PNG library")
include_directories(${PNG_INCLUDE_DIR})
find_package(
  PNG
  REQUIRED
)
message(STATUS " linking PNG library")
target_link_libraries(chroma ${PNG_LIBRARY})
