set(MYFRAME_MAIN MyMain.cpp)

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-E")

LINK_LIBRARIES(pthread dl rt m)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

ADD_EXECUTABLE(myframe ${MYFRAME_MAIN} ${MYFRAME_SRCS})
target_link_libraries(myframe ${Boost_LIBRARIES})