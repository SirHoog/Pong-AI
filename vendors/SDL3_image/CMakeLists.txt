# PongAI/vendors/SDL3_image/CMakeLists.txt

include(FetchContent)

FetchContent_Declare(
	SDL3_image
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
	GIT_TAG release-3.2.4
)
set(SDLIMAGE_AVIF OFF) # Disables AVIF compression optimizations (I only disabled this because it caused some build errors on my end)
FetchContent_MakeAvailable(SDL3_image)