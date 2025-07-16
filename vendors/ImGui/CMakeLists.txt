# PongAI/vendors/ImGui/CMakeLists.txt

include(FetchContent)

FetchContent_Declare(
	ImGui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG docking
)
FetchContent_Declare(
	ImPlot
	GIT_REPOSITORY https://github.com/epezent/implot.git
	GIT_TAG master
)
FetchContent_MakeAvailable(ImGui ImPlot)

target_sources(PongAI PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp

    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp

    # ImPlot
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
    ${implot_SOURCE_DIR}/implot_demo.cpp
)

target_include_directories(PongAI PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${implot_SOURCE_DIR}
)