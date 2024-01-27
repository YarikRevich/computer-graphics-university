cmake_minimum_required(VERSION 3.16)
project(editor VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "-std=c++17 -D_XOPEN_SOURCE_EXTENDED")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
message("Make build output path is /bin")

# Describes general viewport properties.
add_compile_definitions(VIEW_WINDOW_TITLE="View window title")

# Describes internal CGU algorithm functionality.
add_compile_definitions(BIT_NUM_MAX=128)
add_compile_definitions(NATIVE_SHIFT=1)
add_compile_definitions(MEDIAN_CUT_BATCH=7)

add_compile_definitions(BIT_NUM_PER_PIXEL=8)

# Describes CGU metadata properties.
add_compile_definitions(METADATA_FIELDS_NUM=4)

# Describes pre-defined errors used for functionality validation.
add_compile_definitions(BIT_SIZE_MAX_EXCEPTION="Given image has too much unique color bits")
add_compile_definitions(BIT_SIZE_MIN_EXCEPTION="Given image has too small amount of unique color bits. Please use another conversion type")
add_compile_definitions(FILE_NOT_COMPATIBLE_EXCEPTION="Given file is not compatible with CGU codec.")
add_compile_definitions(METADATA_RETRIEVAL_EXCEPTION="Error happened during metadata retrieval. Please try to switch 'optimal' flag")

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

	src/internal/state/state.cpp
	src/internal/tools/tools.cpp
	src/internal/logger/logger.cpp
	src/internal/converter/converter.cpp
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
	target_link_libraries(
		cgu 

		${CMAKE_SOURCE_DIR}/lib/hwy/linux/libhwy.so.1
		${CMAKE_SOURCE_DIR}/lib/jxl/linux/libjxl.so.0.8
		${CMAKE_SOURCE_DIR}/lib/sdl2/linux/libSDL2.so
		${CMAKE_SOURCE_DIR}/lib/sdl2_image/linux/libSDL2_image-2.0.so.0
	)
endif()

if (NOT WIN32)
	install(TARGETS cgu DESTINATION /usr/local/bin)
endif()
