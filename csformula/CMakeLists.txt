project(capsys-csformula)

add_library(csformula SHARED "cseval.cpp" "csformula.cpp")
target_link_libraries(csformula ${Boost_LIBRARIES})
target_include_directories(csformula PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
