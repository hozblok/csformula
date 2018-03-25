project(capsys-csformula-test)
#find_package(Boost COMPONENTS unit_test_framework-gcc-mt-1_65_1 REQUIRED)

#file(GLOB TARGET_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
set(TARGET_SRC "csformulatest.cpp")
add_executable(csformulatest ${TARGET_SRC})
target_link_libraries(csformulatest ${Boost_LIBRARIES} csformula)

#add_test(csevaltest csevaltest)
add_test(NAME csformulatest COMMAND csformulatest)
