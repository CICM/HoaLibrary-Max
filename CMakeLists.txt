cmake_minimum_required(VERSION 3.0)

# Fetch the correct verion of the max-api
message(STATUS "Updating Git Submodules")
execute_process(
	COMMAND				git submodule update --init --recursive
	WORKING_DIRECTORY	"${CMAKE_CURRENT_SOURCE_DIR}"
)

# Misc setup and subroutines
include(${CMAKE_CURRENT_SOURCE_DIR}/source/max-api/script/max-package.cmake)

set(HOALIBRARY_INCLUDES 		${CMAKE_CURRENT_SOURCE_DIR}/source/HoaLibrary/Sources)
set(HOALIBRARY_MAX_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/source/HoaLibrary_Max)
file(GLOB_RECURSE HOALIBRARY_MAX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/source/HoaLibrary_Max/*.cpp)

# Generate a project for every folder in the "source/projects" folder
SUBDIRLIST(PROJECT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source/projects)
foreach (project_dir ${PROJECT_DIRS})
	if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir}/CMakeLists.txt")
		message("Generating: ${project_dir}")
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir})
	endif ()
endforeach ()
