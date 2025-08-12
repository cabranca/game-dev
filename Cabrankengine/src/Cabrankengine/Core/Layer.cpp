#include <Cabrankengine/Core/Layer.h>

namespace cabrankengine {
	
	Layer::Layer(std::string debugName) : m_DebugName(std::move(debugName)) {} // TODO: is there a copy here?
	
	Layer::~Layer() {}
}
