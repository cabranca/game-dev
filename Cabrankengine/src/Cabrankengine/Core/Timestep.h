#pragma once

namespace cabrankengine {

	class Timestep {
	public:
		Timestep(float time = 0.0f)
			: m_Time(time) {}

		float getSeconds() const { return m_Time; }
		
		float getMilliseconds() const { return m_Time * 1000.0f; }
		
		operator float() const { return m_Time; }
	
	private:
		float m_Time;
	};
}
