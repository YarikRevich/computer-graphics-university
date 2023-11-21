if (NOT WIN32)
	cmake_minimum_required(VERSION 3.16)
	project(editor VERSION 0.0.1 LANGUAGES C CXX)

	set(CMAKE_CXX_COMPILER /usr/bin/g++)
	set(CMAKE_CXX_FLAGS "-std=c++17 -D_XOPEN_SOURCE_EXTENDED")
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
	message("Make build output path is /bin")

	include_directories(
		"include/"
	)
	link_directories(
		"lib/"
	)

	add_executable(
		cgu 

		src/cmd/main.cpp
	)

	if(NOT APPLE)
		target_link_libraries(cgu stdc++fs)
	else()
		IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm64")
			target_link_libraries(
				cgu 
	
				${CMAKE_SOURCE_DIR}/lib/sdl2/arm64/libSDL2.a
			)
		ELSE()
			target_link_libraries(
				cgu 
	
				${CMAKE_SOURCE_DIR}/lib/sdl2/x86/libSDL2.a
			)
		ENDIF()
	endif()

	if (NOT WIN32)
		install(TARGETS cgu DESTINATION /usr/local/bin)
	endif()
else()
	message("You mustn't use the worst os in the world to install this perfect application")
endif()
