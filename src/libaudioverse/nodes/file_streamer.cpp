/**Copyright (C) Austin Hicks, 2014
This file is part of Libaudioverse, a library for 3D and environmental audio simulation, and is released under the terms of the Gnu General Public License Version 3 or (at your option) any later version.
A copy of the GPL, as well as other important copyright and licensing information, may be found in the file 'LICENSE' in the root of the Libaudioverse repository.  Should this file be missing or unavailable to you, see <http://www.gnu.org/licenses/>.*/
#include <libaudioverse/libaudioverse.h>
#include <libaudioverse/libaudioverse_properties.h>
#include <libaudioverse/nodes/file_streamer.hpp>
#include <libaudioverse/private/node.hpp>
#include <libaudioverse/private/simulation.hpp>
#include <libaudioverse/private/properties.hpp>
#include <libaudioverse/private/macros.hpp>
#include <libaudioverse/private/memory.hpp>
#include <libaudioverse/private/constants.hpp>
#include <string>
#include <math.h>

namespace libaudioverse_implementation {

FileStreamerNode::FileStreamerNode(std::shared_ptr<Simulation> simulation, std::string path): Node(Lav_OBJTYPE_FILE_STREAMER_NODE, simulation, 0, 1),
streamer(path, simulation->getBlockSize(), simulation->getSr()) {
	resize(0, streamer.getChannels());
	appendOutputConnection(0, streamer.getChannels());
	getProperty(Lav_FILE_STREAMER_POSITION).setDoubleRange(0.0, streamer.getDuration());
}

std::shared_ptr<Node> createFileStreamerNode(std::shared_ptr<Simulation> simulation, std::string path) {
	return standardNodeCreation<FileStreamerNode>(simulation, path);
}

void FileStreamerNode::process() {
	if(werePropertiesModified(this, Lav_FILE_STREAMER_POSITION)) streamer.setPosition(getProperty(Lav_BUFFER_POSITION).getDoubleValue());
	if(werePropertiesModified(this, Lav_FILE_STREAMER_LOOPING)) streamer.setIsLooping(getProperty(Lav_FILE_STREAMER_LOOPING).getIntValue() != 0);
	streamer.process(&output_buffers[0]);
	getProperty(Lav_FILE_STREAMER_POSITION).setDoubleValue(streamer.getPosition());
	if(streamer.getEnded()) {
		//We only end if we're silent, which can be computed as a sum of absolute values.
		float sum = 0.0f;
		for(auto &o: output_buffers) {
			for(int i = 0; i < block_size; i++) {
				sum += fabs(o[i]);
			}
		}
		if(sum < 1e-3) {
			//It's quiet enough for us to go ahead and stop it.
			getProperty(Lav_FILE_STREAMER_ENDED).setIntValue(1);
			simulation->enqueueTask([=] () {(*end_callback)();});
		}
	}
	else getProperty(Lav_FILE_STREAMER_ENDED).setIntValue(0);
}

//begin public api
Lav_PUBLIC_FUNCTION LavError Lav_createFileStreamerNode(LavHandle simulationHandle, const char* path, LavHandle* destination) {
	PUB_BEGIN
	auto simulation = incomingObject<Simulation>(simulationHandle);
	LOCK(*simulation);
	auto retval = createFileStreamerNode(simulation, std::string(path));
	*destination = outgoingObject<Node>(retval);
	PUB_END
}

Lav_PUBLIC_FUNCTION LavError Lav_fileStreamerNodeSetEndCallback(LavHandle nodeHandle, LavParameterlessCallback callback, void* userdata) {
	PUB_BEGIN
	auto n = incomingObject<FileStreamerNode>(nodeHandle);
	if(callback) {
		n->end_callback->setCallback(wrapParameterlessCallback(n, callback, userdata));
	}
	else n->end_callback->clear();
	PUB_END
}

}