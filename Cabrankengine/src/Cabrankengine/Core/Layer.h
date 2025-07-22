#pragma once

#include <Cabrankengine/Core/Core.h>
#include <Cabrankengine/Events/Event.h>

namespace cabrankengine {
	
	class CABRANKENGINE_API Layer {
	public:
		Layer(const std::string& name = "Layer"); // TODO: Make explicit?
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDettach() {}
		virtual void onUpdate() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}

