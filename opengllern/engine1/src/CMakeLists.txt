find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

AUX_SOURCE_DIRECTORY(./ SOURCES)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

FILE(GLOB_RECURSE LibFiles "../lib/*.h")

add_executable(${PROJECT_NAME}  ${SOURCES} ${LibFiles})

add_dependencies(${PROJECT_NAME} copy_assets)

target_include_directories(${PROJECT_NAME}  PUBLIC ../lib/ ../inc/)
target_link_libraries(${PROJECT_NAME} glfw OpenGL GLEW)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
