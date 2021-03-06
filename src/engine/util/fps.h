#ifndef _ENGINE_UTIL_FPS_H_
#define _ENGINE_UTIL_FPS_H_

namespace engine {
namespace util {

class FrameCounter {
public:
	/** to be called each time a frame has been completed */
	void frame();

	/** auto-updated to always contain the current FPS value */
	float fps;

	/** contains the number of completed frames */
	unsigned count;

	/** milliseconds used for the last frame */
	unsigned int msec_lastframe;

private:
	float frame_count_weighted;
	float frame_length_sum_weighted;
	int lastframe_timestamp;
};

} //namespace util
} //namespace engine

#endif //_ENGINE_UTIL_FPS_H_
