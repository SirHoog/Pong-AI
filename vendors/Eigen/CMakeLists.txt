# PongAI/vendors/Eigen/CMakeLists.txt

include(FetchContent)

FetchContent_Declare(
	Eigen
	GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
	GIT_TAG master
	OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(Eigen)

target_include_directories(PongAI PRIVATE ${eigen_SOURCE_DIR})