include_directories(${PROJECT_SOURCE_DIR}/include)

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS})

add_library(HCD_Library harrisCornerDetector.cpp)

add_executable( start main.cpp)

target_link_libraries( start HCD_Library ${OpenCV_LIBRARIES})