# PongAI/vendors/MiniDNN/CMakeLists.txt

include(FetchContent)

FetchContent_Declare(
    MiniDNN
    GIT_REPOSITORY https://github.com/yixuan/MiniDNN.git
    GIT_TAG master
)
FetchContent_MakeAvailable(MiniDNN)

target_include_directories(PongAI PRIVATE ${minidnn_SOURCE_DIR}/include)