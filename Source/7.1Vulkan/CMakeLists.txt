project(Vulkan VERSION 0.5)

set(SDL2_DIR ../../ThirdParty/SDL2-2.26.5/cmake)
find_package(SDL2 REQUIRED)

set(SDL2_image_DIR ../../ThirdParty/SDL2_image-2.6.3/cmake)
find_package(SDL2_image REQUIRED)

# Vulkan
find_package(Vulkan REQUIRED)

file (GLOB SRC_DIR_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

if(SDL2_FOUND AND SDL2_image_FOUND AND Vulkan_FOUND)
  include_directories(${PROJECT_SOURCE_DIR})
  add_executable(${PROJECT_NAME} ${SRC_DIR_FILES})
  target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image Vulkan::Vulkan GameFramework)
  # .dll
  add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL2_image::SDL2_image>  $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )

  # to run
  add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${PROJECT_SOURCE_DIR}/Assets  $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets
  )

  set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )
endif()
