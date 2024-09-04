#pragma once

#include <GameObject.h>

using namespace cabrankengine;

namespace snake {

	class Snake : public GameObject {
	public:
		Snake();
		~Snake();

		void update(float delta);


	private:

	};
}
