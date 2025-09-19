
cmake_minimum_required(VERSION 3.31)

set (ui-lib-src
        src/ui/services/UISystemsService.cpp
        src/ui/systems/UIInputSystem.cpp
        src/ui/systems/UIRenderSystem.cpp
        src/ui/systems/UISystems.cpp
)

set (ui-lib-modules
        src/ui/Button.cppm
        src/ui/ImageButton.cppm
        src/ui/services/UIInputSystemService.cppm
        src/ui/services/UIRenderSystemService.cppm
        src/ui/services/UISystemsService.cppm
        src/ui/systems/UIInputSystem.cppm
        src/ui/systems/UIRenderSystem.cppm
        src/ui/systems/UISystems.cppm
)

add_library(ui-lib STATIC ${ui-lib-src} ${ui-lib-modules})

target_sources(ui-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${ui-lib-modules})
target_compile_features(ui-lib PUBLIC cxx_std_23)
target_link_libraries(ui-lib PUBLIC core-lib gfx-lib input-lib)
target_include_directories(ui-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

#add_subdirectory(tests)
