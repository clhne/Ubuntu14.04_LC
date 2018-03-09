project( peopledetect )
find_package( OpenCV REQUIRED )
add_executable( peopledetect peopledetect )
target_link_libraries( peopledetect ${OpenCV_LIBS} )