cmake_minimum_required(VERSION 2.8.3)


add_definitions(-O3 )
 

project(cybermotor_ctrl)


include_directories(
  include 
  src
)


file(GLOB_RECURSE src
     src/*.cpp
     )

add_executable(${PROJECT_NAME}
   ${src} )


target_link_libraries(${PROJECT_NAME}  
    pthread
)

add_executable(generate_cmd generate_cmd/generate_cmd.cpp src/math_ops.cpp)
target_link_libraries(generate_cmd  
    pthread
)

