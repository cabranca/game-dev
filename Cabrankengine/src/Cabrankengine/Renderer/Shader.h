#pragma once

#include <string>
#include <Cabrankengine/Core/Core.h>

namespace cabrankengine {

	class Shader {
		public:
			virtual ~Shader() = default;

			virtual void bind() const = 0;
			virtual void unbind() const = 0;

			static Ref<Shader> create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}