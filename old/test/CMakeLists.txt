testexec(lexer lexer.cpp)
testlib(lexer lumos-s)

testexec(num num.cpp)
testlib(num lumos-s)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(LLVM_LIBS support core)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_CXXFLAGS}")

add_executable(lumosc lumosc.cpp)
target_link_libraries(lumosc lumos-s ${LLVM_LIBS})
