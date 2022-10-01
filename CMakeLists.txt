project(FeatureMatching)
find_package(OpenCV REQUIRED)
add_executable(main Features2D_Homography.cpp)
target_link_libraries(main ${OpenCV_LIBS})

INSTALL(DIRECTORY images DESTINATION build)