/**Copyright (C) Austin Hicks, 2014
This file is part of Libaudioverse, a library for 3D and environmental audio simulation, and is released under the terms of the Gnu General Public License Version 3 or (at your option) any later version.
A copy of the GPL, as well as other important copyright and licensing information, may be found in the file 'LICENSE' in the root of the Libaudioverse repository.  Should this file be missing or unavailable to you, see <http://www.gnu.org/licenses/>.*/
#pragma once
#include "../private/hrtf.hpp"
#include "delayline.hpp"
#include "convolvers.hpp"
#include <memory>

namespace libaudioverse_implementation {

//Implement HRTF panning.

class HrtfPanner {
	public:
	HrtfPanner(int _block_size, float _sr, std::shared_ptr<HrtfData> hrtf);
	~HrtfPanner();
	void pan(float* input, float* left_output, float* right_output);
	void reset();
	void setAzimuth(float angle);
	float getAzimuth();
	void setElevation(float angle);
	float getElevation();
	void setShouldCrossfade(bool cf);
	bool getShouldCrossfade();
	//The threshold after which we will crossfade as (azimuth+elevation) > threshold.
	//Also known as manhattan distance.
	void setCrossfadeThreshold(float threshold);
	float getCrossfadeThreshold();
	private:
	std::shared_ptr<HrtfData> hrtf;
	//Convolvers, current and previous.
	BlockConvolver *left_convolver, *right_convolver, *prev_left_convolver, *prev_right_convolver;
	int block_size;
	int response_length;
	float sr;
	float azimuth = 0, elevation = 0, prev_azimuth = 0, prev_elevation  = 0;
	float crossfade_threshold = 5.0;
	bool should_crossfade = true;
};

}