#ifndef _ENGINE_INIT_H_
#define _ENGINE_INIT_H_

namespace engine {

/**
engine initialization method.
opens a window and initializes the OpenGL context.
*/
void init(const char *windowtitle);

/**
engine de-initialization method.
call to cleanly shutdown the application.
*/
void destroy();

} //namespace engine

#endif //_ENGINE_INIT_H_
