#include "callbacks.h"

#include <stdlib.h>

#include "gamestate.h"
#include "../engine/engine.h"
#include "../engine/log.h"
#include "../engine/coord/window.h"
#include "../engine/coord/camgame.h"
#include "../engine/coord/camhud.h"
#include "../engine/coord/phys3.h"
#include "../engine/coord/tile.h"
#include "../engine/coord/tile3.h"
#include "../engine/coord/vec2f.h"
#include "../engine/util/misc.h"
#include "../engine/input.h"

using namespace engine;

namespace openage {

bool input_handler(SDL_Event *e) {
	switch (e->type) {

	case SDL_QUIT:
		engine::running = false;
		break;

	case SDL_MOUSEBUTTONDOWN: {
		using namespace coord;

		//a mouse button was pressed...
		//subtract value from window height to get position relative to lower right (0,0).
		coord::window mousepos_window {(pixel_t) e->button.x, (pixel_t) e->button.y};
		coord::camgame mousepos_camgame = mousepos_window.to_camgame();
		//TODO once the terrain elevation milestone is implemented, use a method
		//more suitable for converting camgame to phys3
		coord::phys3 mousepos_phys3 = mousepos_camgame.to_phys3();
		coord::tile mousepos_tile = mousepos_phys3.to_tile3().to_tile();

		if (e->button.button == SDL_BUTTON_LEFT) {
			log::dbg("LMB [window]:    x %9hd y %9hd",
			         mousepos_window.x,
			         mousepos_window.y);
			log::dbg("LMB [camgame]:   x %9hd y %9hd",
			         mousepos_camgame.x,
			         mousepos_camgame.y);
			log::dbg("LMB [phys3]:     NE %8.3f SE %8.3f UP %8.3f",
			         ((float) mousepos_phys3.ne) / phys_per_tile,
			         ((float) mousepos_phys3.se) / phys_per_tile,
			         ((float) mousepos_phys3.up) / phys_per_tile);
			log::dbg("LMB [tile]:      NE %8ld SE %8ld",
			         mousepos_tile.ne,
			         mousepos_tile.se);
			terrain->set_tile(mousepos_tile, editor_current_terrain);
		}
		else if (e->button.button == SDL_BUTTON_RIGHT) {
			//check whether an building already exists at this pos
			bool found = false;

			for(unsigned i = 0; i < buildings.size(); i++) {
				if (buildings[i].pos == mousepos_tile) {
					buildings.erase(buildings.begin() + i);
					found = true;
					break;
				}
			}

			if(!found) {
				building newbuilding;
				newbuilding.player = 1 + (buildings.size() % 8);
				newbuilding.pos = mousepos_tile;
				newbuilding.tex = university;
				buildings.push_back(newbuilding);
			}
		}

		}
		break;
	case SDL_MOUSEWHEEL:
		editor_current_terrain = util::mod<ssize_t>(editor_current_terrain + e->wheel.y, terrain_texture_count);
		break;

	case SDL_KEYUP:
		switch (((SDL_KeyboardEvent *) e)->keysym.sym) {
		case SDLK_ESCAPE:
			//stop the game
			engine::running = false;
			break;
		}

		break;
	case SDL_KEYDOWN:
		switch (((SDL_KeyboardEvent *) e)->keysym.sym) {
		case SDLK_SPACE:
			terrain->blending_enabled = !terrain->blending_enabled;
			break;
		}

		break;
	}

	return true;
}

void move_camera() {
	//read camera movement input keys, and move camera
	//accordingly.

	//camera movement speed, in pixels per millisecond
	//one pixel per millisecond equals 14.3 tiles/second
	float cam_movement_speed_keyboard = 0.5;

	coord::vec2f cam_movement {0.0, 0.0};

	if (engine::input::is_down(SDLK_LEFT)) {
		cam_movement.x -= cam_movement_speed_keyboard;
	}
	if (engine::input::is_down(SDLK_RIGHT)) {
		cam_movement.x += cam_movement_speed_keyboard;
	}
	if (engine::input::is_down(SDLK_DOWN)) {
		cam_movement.y -= cam_movement_speed_keyboard;
	}
	if (engine::input::is_down(SDLK_UP)) {
		cam_movement.y += cam_movement_speed_keyboard;
	}

	cam_movement *= (float) engine::fpscounter->msec_lastframe;

	//calculate camera position delta from velocity and frame duration
	coord::camgame_delta cam_delta;
	cam_delta.x = cam_movement.x;
	cam_delta.y = cam_movement.y;

	//update camera phys position
	engine::camgame_phys += cam_delta.to_phys3();
}


bool on_engine_tick() {
	move_camera();

	return true;
}

bool draw_method() {
	//draw gaben, our great and holy protector, bringer of the half-life 3.
	gaben->draw(coord::camgame {0, 0});

	//draw terrain
	terrain->draw();

	//draw each building
	for(auto &building : buildings){
		building.tex->draw(building.pos.to_tile3().to_phys3().to_camgame(), false, 0, building.player);
	}

	return true;
}

bool hud_draw_method() {
	//draw the currently selected editor texture tile
	terrain->texture(editor_current_terrain)->draw(coord::window{63, 84}.to_camhud());

	return true;
}

} //namespace openage
