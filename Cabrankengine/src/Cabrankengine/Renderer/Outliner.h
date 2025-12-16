#pragma once

#include <pch.h>

#include <Cabrankengine/Math/Mat4.h>

namespace cabrankengine::rendering {

	class Shader;
	class VertexArray;

	class Outliner {
	  public:
		static void init();
		static void drawOutline(const Ref<Shader>& originalShader, const Ref<VertexArray>& vertexArray, const math::Color& outlineColor, const math::Mat4& transform);

	  private:
		static inline Ref<Shader> m_Shader = nullptr;
	};
} // namespace cabrankengine::rendering