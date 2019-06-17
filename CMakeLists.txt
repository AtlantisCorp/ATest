# Creates the library into the local 'build' directory and installs it in the default
# installation directory. 

cmake_minimum_required(VERSION 3.12)
project(ATest LANGUAGES CXX)

file(GLOB sources src/*.cpp)
file(GLOB includes includes/*.h)

add_library(atest SHARED ${sources} ${includes})

target_include_directories(atest PRIVATE "includes")

set_target_properties(atest PROPERTIES	
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build"
	CXX_STANDARD 17)

set_target_properties(atest PROPERTIES
		FRAMEWORK TRUE 
		FRAMEWORK_VERSION A
		MACOSX_FRAMEWORK_IDENTIFIER com.atl.atest
		VERSION 1.0.0
		SOVERSION 1.0.0
		XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "luk2010")
		
set_property(TARGET atest PROPERTY PUBLIC_HEADER ${includes})