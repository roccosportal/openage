include_directories(/usr/include/freetype2)
#link_directories(${CMAKEDEMO_BINARY_DIR}/src)

find_package(OpenGL)
find_package(GLEW)
#findpackage sdl2

add_executable(${PROJECT_NAME}
	main.cpp
	openage/main.cpp
	openage/init.cpp
	openage/callbacks.cpp
	openage/gamestate.cpp
	openage/objects/building.cpp
	openage/objects/terrain.cpp
	engine/engine.cpp
	engine/callbacks.cpp
	engine/init.cpp
	engine/font.cpp
	engine/terrain.cpp
	engine/terrain_chunk.cpp
	engine/texture.cpp
	engine/input.cpp
	engine/shader/shader.cpp
	engine/shader/program.cpp
	engine/coord/chunk.cpp
	engine/coord/tile.cpp
	engine/coord/tile3.cpp
	engine/coord/phys2.cpp
	engine/coord/phys3.cpp
	engine/coord/camgame.cpp
	engine/coord/camhud.cpp
	engine/coord/window.cpp
	engine/coord/vec2.cpp
	engine/coord/vec2f.cpp
	engine/coord/vec3.cpp
	engine/coord/vec3f.cpp
	engine/log.cpp
	engine/util/color.cpp
	engine/util/error.cpp
	engine/util/file.cpp
	engine/util/fps.cpp
	engine/util/misc.cpp
	engine/util/strings.cpp
	engine/util/timer.cpp
	engine/util/unicode.cpp
	engine/console/callbacks.cpp
	engine/console/console.cpp
	engine/console/init.cpp
)

target_link_libraries(${PROJECT_NAME}
	m
	SDL2
	SDL2_image
	${OPENGL_LIBRARY}
	${GLEW_LIBRARIES}
	ftgl
	fontconfig
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
