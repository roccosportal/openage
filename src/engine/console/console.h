#ifndef _ENGINE_CONSOLE_CONSOLE_H_
#define _ENGINE_CONSOLE_CONSOLE_H_

#include <vector>

#include "../coord/camhud.h"
#include "../util/color.h"
#include "../font.h"

namespace engine {
namespace console {

/**
 * is the console currently visible?
 */
extern bool visible;

extern coord::camhud topleft;
extern coord::camhud bottomright;

extern util::col bgcol, fgcol;
extern std::vector<const char *> messages;
extern Font *font;

extern int spacing;

/**
 * prints the given text on the console
 */
void write(const char *text);

} //namespace console
} //namespace engine

#endif //_ENGINE_CONSOLE_CONSOLE_H_
