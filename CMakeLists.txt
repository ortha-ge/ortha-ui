
cmake_minimum_required(VERSION 3.31)

set (ortha-ui-src
        src/ui/services/UISystemsService.cpp
        src/ui/systems/UIInputSystem.cpp
        src/ui/systems/UIRenderSystem.cpp
        src/ui/systems/UISystems.cpp
)

set (ortha-ui-modules
        src/ui/Button.cppm
        src/ui/ImageButton.cppm
        src/ui/services/UIInputSystemService.cppm
        src/ui/services/UIRenderSystemService.cppm
        src/ui/services/UISystemsService.cppm
        src/ui/systems/UIInputSystem.cppm
        src/ui/systems/UIRenderSystem.cppm
        src/ui/systems/UISystems.cppm
)

add_library(ortha-ui STATIC ${ortha-ui-src} ${ortha-ui-modules})

target_sources(ortha-ui PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${ortha-ui-modules})
target_compile_features(ortha-ui PUBLIC cxx_std_23)
target_link_libraries(ortha-ui PUBLIC ortha-core ortha-gfx ortha-input)
target_include_directories(ortha-ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

#add_subdirectory(tests)
