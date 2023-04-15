project(GameFramework VERSION 0.5)

file (GLOB SRC_DIR_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include/**)
add_library(${PROJECT_NAME} STATIC ${SRC_DIR_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/include
)
