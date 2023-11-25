if (NOT WIN32)
	cmake_minimum_required(VERSION 3.16)
	project(editor VERSION 0.0.1 LANGUAGES C CXX)

	set(CMAKE_CXX_COMPILER /usr/bin/g++)
	set(CMAKE_CXX_FLAGS "-std=c++17 -D_XOPEN_SOURCE_EXTENDED")
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
	message("Make build output path is /bin")

	add_compile_definitions(VIEW_WINDOW_TITLE="View window title")

	include_directories(
		"include/"
	)
	link_directories(
		"lib/"
	)

	add_executable(
		cgu 

		src/cmd/main.cpp

		src/internal/core/command/decode/decode.cpp
		src/internal/core/command/encode/encode.cpp
		src/internal/core/command/view/view.cpp
		src/internal/core/command/help/help.cpp

		src/internal/core/command/validator/validator.cpp

		src/internal/core/command/handler/handler.cpp

		src/internal/core/io/io.cpp
		src/internal/core/window/window.cpp

		src/internal/tool/tool.cpp
	)

	if(APPLE)
		find_package(SDL2 REQUIRED COMPONENTS SDL2)
		target_link_libraries(cgu PRIVATE SDL2::SDL2)
	elseif(WIN32)
		target_link_libraries(
			cgu 

			${CMAKE_SOURCE_DIR}/lib/sdl2/windows/libSDL2.a
			${CMAKE_SOURCE_DIR}/lib/sdl2/windows/libSDL2main.a
		)
	elseif(UNIX)

	endif()

	if (NOT WIN32)
		install(TARGETS cgu DESTINATION /usr/local/bin)
	endif()
else()
	message("You mustn't use the worst os in the world to install this perfect application")
endif()
