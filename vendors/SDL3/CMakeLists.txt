# PongAI/vendors/SDL3/CMakeLists.txt

include(FetchContent)

FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG release-3.2.16
	OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3)

target_include_directories(PongAI PUBLIC
    ${SDL3_SOURCE_DIR}/include
)