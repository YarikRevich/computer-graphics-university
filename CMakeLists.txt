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

		src/internal/command/decode/decode.cpp
		src/internal/command/encode/encode.cpp
		src/internal/command/view/view.cpp
		src/internal/command/help/help.cpp

		src/internal/command/validator/validator.cpp

		src/internal/command/handler/handler.cpp

		src/internal/processor/processor.cpp
		src/internal/io/io.cpp
		src/internal/window/window.cpp
	)

	if(APPLE)
		find_package(SDL2 REQUIRED COMPONENTS SDL2)
		target_link_libraries(cgu PRIVATE SDL2::SDL2)

		find_package(SDL2_image REQUIRED COMPONENTS SDL2_image)
		target_link_libraries(cgu PRIVATE SDL2_image::SDL2_image)
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
