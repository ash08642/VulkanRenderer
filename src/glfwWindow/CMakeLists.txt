add_library(Window OBJECT
    Window.cpp
)
#target_include_directories(Window PRIVATE window)
target_link_libraries(Window glfw3)
