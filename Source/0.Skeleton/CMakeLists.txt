project(Skeleton VERSION 0.5)

set(SDL2_DIR ../../ThirdParty/SDL2-2.26.5/cmake)
find_package(SDL2 REQUIRED)

if(SDL2_FOUND)
  include_directories(${PROJECT_SOURCE_DIR})
  add_executable(${PROJECT_NAME} Main.cpp Game.cpp)
  target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2)
endif()
