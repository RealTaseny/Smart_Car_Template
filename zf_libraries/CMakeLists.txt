cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)

aux_source_directory(./src zf_libraries_Srcs)

target_sources(
        ${CMAKE_PROJECT_NAME} PRIVATE ${zf_libraries_Srcs}
)

target_include_directories(
        ${CMAKE_PROJECT_NAME} PRIVATE ./inc
)