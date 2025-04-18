cmake_minimum_required(VERSION 3.10)

project(Lizardsss)

# use C++11 standard
set(CMAKE_CXX_STANDARD 11)

# suppress generating ZERO_CHECK project
set(CMAKE_SUPPRESS_REGENERATION true)

# add source files
file(GLOB_RECURSE SOURCE_FILES
${CMAKE_SOURCE_DIR}/src/*.cpp
${CMAKE_SOURCE_DIR}/ThirdParty/imgui/*.cpp)

# add header files
file(GLOB_RECURSE HEADER_FILES
	${CMAKE_SOURCE_DIR}/ThirdParty/imgui/*.h
	${CMAKE_SOURCE_DIR}/src/*.h)

include_directories(${CMAKE_SOURCE_DIR}/ThirdParty/imgui
	${CMAKE_SOURCE_DIR}/ThirdParty/SDL2/include
	${CMAKE_SOURCE_DIR}/ThirdParty/SDL2_image/include
	${CMAKE_SOURCE_DIR}/ThirdParty/SDL2_ttf/include
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})


target_link_libraries(${PROJECT_NAME} 
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SDL2/lib/x64/SDL2.lib
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SDL2/lib/x64/SDL2main.lib
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SDL2_image/lib/x64/SDL2_image.lib
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SDL2_ttf/lib/x64/SDL2_ttf.lib
)


add_custom_command(TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND cd ${CMAKE_SOURCE_DIR}
	COMMAND "${CMAKE_SOURCE_DIR}/copyDLLs.bat"
	COMMENT "Running copyDLLs.bat"
)


set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
