cmake_minimum_required(VERSION 3.0)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-api/script/max-pretarget.cmake)

include_directories(
	"${C74_INCLUDES}"
	"${HOALIBRARY_INCLUDES}"
	"${HOALIBRARY_MAX_INCLUDES}"
)

file(GLOB_RECURSE PROJECT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(
	${PROJECT_NAME}
	MODULE
	${HOALIBRARY_MAX_SRC}
	${PROJECT_SRC}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-api/script/max-posttarget.cmake)
