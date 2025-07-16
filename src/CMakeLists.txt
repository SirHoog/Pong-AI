# PongAI/src/CMakeLists.txt

add_executable(PongAI
	AI/Bot.cpp

	Game/Paddle.cpp
	Game/Ball.cpp
	Game/Game.cpp

	UI/Window.cpp
	UI/MainWindow.cpp
	UI/GameWindow.cpp
	UI/PlotWindow.cpp
	UI/UI.cpp
	
	Application.cpp
	main.cpp
)

set_target_properties(PongAI PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/src/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/src/$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/src/$<CONFIG>"
)

target_link_libraries(PongAI PRIVATE
	SDL3::SDL3
	SDL3_image::SDL3_image
	# No ImGui or ImPlot here: Headers & Source Only Library
	Eigen3::Eigen
	# No MiniDNN here either: Headers Only library
)

# Provide absolute path to resource directories
target_compile_definitions(PongAI PRIVATE RES_DIR="${PROJECT_SOURCE_DIR}/res")
target_compile_definitions(PongAI PRIVATE TEX_DIR="${PROJECT_SOURCE_DIR}/res/textures")
target_compile_definitions(PongAI PRIVATE NN_DIR="${PROJECT_SOURCE_DIR}/res/NNs")

# Copy SDL3.dll and SDL3_image.dll after build (Windows only)
if(WIN32)
    add_custom_command(TARGET PongAI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:PongAI>
    )
	add_custom_command(TARGET PongAI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3_image::SDL3_image>
        $<TARGET_FILE_DIR:PongAI>
    )
endif()