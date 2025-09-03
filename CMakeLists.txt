# CMakeList.txt : CMake project for Abomination, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Abomination")

set(CMAKE_CXX_STANDARD 17)

# Set the build output location to a common directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# Include sub-projects.
add_subdirectory ("OpenXLSX")
add_subdirectory ("Functions")
#add_subdirectory ("WindowsFunctions")

# Add source to this project's executable.
add_executable (Abomination "Abomination.cpp" "Abomination.h" "Functions/text_functions.cpp" "Functions/text_functions.h" "Functions/data_functions.cpp" "Functions/data_functions.h" "Functions/file_functions.h" "Functions/file_functions.cpp")

target_link_libraries(Abomination OpenXLSX::OpenXLSX)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Abomination PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
