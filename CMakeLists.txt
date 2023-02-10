cmake_minimum_required(VERSION 3.21)
project(yuca-game LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" OFF)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Download SFML
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

# Download Box2D
FetchContent_Declare(
  box2d
  GIT_REPOSITORY https://github.com/erincatto/box2d.git
  GIT_TAG main
)
FetchContent_MakeAvailable(box2d)

# Define the options for Box2D (not working?)
add_definitions(-DBOX2D_BUILD_TESTBED=OFF -DBOX2D_BUILD_UNIT_TESTS=OFF)

# Add the Box2D headers to the include directories
include_directories(${box2d_SOURCE_DIR}/include/Box2D)

# Add the source files
file(GLOB_RECURSE SRC_FILES_PROJECT "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE SRC_FILES_ENGINE "${PROJECT_SOURCE_DIR}/engine/src/*.cpp")

# Add the include directories
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/engine/include")

# Add the executable target
add_executable(yuca-game ${SRC_FILES_ENGINE} ${SRC_FILES_PROJECT})

# Link SFML and Box2D to the target
target_link_libraries(yuca-game PRIVATE sfml-audio sfml-graphics sfml-window sfml-system box2d)
target_compile_features(yuca-game PRIVATE cxx_std_17)

# Copy shared libraries on Windows
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET yuca-game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:yuca-game> $<TARGET_FILE_DIR:yuca-game> COMMAND_EXPAND_LISTS)
endif()

# Install the target
install(TARGETS yuca-game)

