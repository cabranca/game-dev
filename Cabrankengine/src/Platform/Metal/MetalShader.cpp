#include <pch.h>
#include "MetalShader.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/Core/Window.h>
#include <Cabrankengine/Math/MatrixFactory.h>
#include <Platform/Metal/MetalContext.h>
#include <Platform/Metal/MetalRendererAPI.h>

namespace cabrankengine::platform::metal {

	using namespace math;
	using namespace rendering;

	MetalShader::MetalShader(const std::string& filepath) {
		CE_PROFILE_FUNCTION();

		std::filesystem::path path(filepath);
		m_Name = path.stem().string();

		std::string source = readFile(filepath);
		createPipeline(source);
	}

	MetalShader::MetalShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name) {
		CE_PROFILE_FUNCTION();

		// En Metal solemos tener todo en un solo string/archivo,
		// así que concatenamos o asumimos que 'vertexSrc' tiene todo si es un string raw.
		std::string source = vertexSrc + "\n" + fragmentSrc;
		createPipeline(source);
	}

	MetalShader::~MetalShader() {
		CE_PROFILE_FUNCTION();

		if (m_PipelineState)
			m_PipelineState->release();
		if (m_VertexFunction)
			m_VertexFunction->release();
		if (m_FragmentFunction)
			m_FragmentFunction->release();
		if (m_Library)
			m_Library->release();
	}

	void MetalShader::bind() const {
		CE_PROFILE_FUNCTION();
		// ACÁ está el truco: Le decimos a la API estática que use este PSO
		MetalRendererAPI::SetCurrentPipelineState(m_PipelineState);
	}

	void MetalShader::unbind() const {}

	void MetalShader::setInt(const std::string& name, int value) {
		CE_PROFILE_FUNCTION();
	}

	void MetalShader::setIntArray(const std::string& name, uint32_t count, int* values) {
		CE_PROFILE_FUNCTION();
	}

	void MetalShader::setFloat(const std::string& name, float value) {
		CE_PROFILE_FUNCTION();
	}

	void MetalShader::setFloat3(const std::string& name, const Vector3& vector) {
		CE_PROFILE_FUNCTION();
	}

	void MetalShader::setFloat4(const std::string& name, const Vector4& vector) {
		CE_PROFILE_FUNCTION();
	}

	void MetalShader::setMat4(const std::string& name, const Mat4& value) {
		CE_PROFILE_FUNCTION();
	}

	std::string MetalShader::readFile(const std::string& filepath) {
		CE_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		} else {
			CE_CORE_ERROR("Could not open shader file: {0}", filepath);
		}
		return result;
	}

	void MetalShader::createPipeline(const std::string& source) {
		// 1. Obtener Device
		const auto& window = Application::get().getWindow();
		MetalContext* context = static_cast<MetalContext*>(window.getContext());
		MTL::Device* device = context->getDevice();

		NS::Error* error = nullptr;

		// 2. Crear Librería (Compilar Shader)
		NS::String* nsSource = NS::String::string(source.c_str(), NS::StringEncoding::UTF8StringEncoding);
		m_Library = device->newLibrary(nsSource, nullptr, &error);

		if (!m_Library) {
			CE_CORE_ERROR("Error shader: {0}", error->localizedDescription()->utf8String());
			return;
		}

		// 3. Obtener funciones por nombre (Convención: vertex_main y fragment_main)
		// TODO: Podrías parsear el archivo para buscar anotaciones o pasarlo por parámetro
		m_VertexFunction = m_Library->newFunction(NS::String::string("vertex_main", NS::UTF8StringEncoding));
		m_FragmentFunction = m_Library->newFunction(NS::String::string("fragment_main", NS::UTF8StringEncoding));

		if (!m_VertexFunction || !m_FragmentFunction) {
			CE_CORE_ERROR("Error: Could not find 'vertex_main' or 'fragment_main' in shader.");
			return;
		}

		// 4. Configurar Vertex Descriptor (IMPORTANTE: Esto debe coincidir con tu VertexBuffer)
		// Asumimos layout estándar: Buffer 0, Stride dinámico, Atributos float3, float2, etc.
		MTL::VertexDescriptor* vertexDesc = MTL::VertexDescriptor::alloc()->init();

		// Atributo 0: Posición (Float3) - Offset 0
		vertexDesc->attributes()->object(0)->setFormat(MTL::VertexFormatFloat3);
		vertexDesc->attributes()->object(0)->setOffset(0);
		vertexDesc->attributes()->object(0)->setBufferIndex(0);

		// Atributo 1: Color/TexCoord (Float4/Float2) - Offset 12 (3 floats * 4 bytes)
		// Por ahora hardcodeamos para el Hello Triangle que usa Float4 color
		vertexDesc->attributes()->object(1)->setFormat(MTL::VertexFormatFloat4);
		vertexDesc->attributes()->object(1)->setOffset(12);
		vertexDesc->attributes()->object(1)->setBufferIndex(0);

		// Layout Buffer 0
		vertexDesc->layouts()->object(0)->setStride(12 + 16); // 3 floats + 4 floats
		vertexDesc->layouts()->object(0)->setStepRate(1);
		vertexDesc->layouts()->object(0)->setStepFunction(MTL::VertexStepFunctionPerVertex);

		// 5. Configurar Pipeline Descriptor
		MTL::RenderPipelineDescriptor* pipelineDesc = MTL::RenderPipelineDescriptor::alloc()->init();
		pipelineDesc->setVertexFunction(m_VertexFunction);
		pipelineDesc->setFragmentFunction(m_FragmentFunction);
		// pipelineDesc->setVertexDescriptor(vertexDesc); // Comentado hasta que usemos VertexBuffers reales

		// Formato de Color (Debe coincidir con Swapchain)
		pipelineDesc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

		// Configurar Blending (Alpha Blend por defecto)
		// auto colorAttachment = pipelineDesc->colorAttachments()->object(0);
		// colorAttachment->setBlendingEnabled(true);
		// colorAttachment->setRgbBlendOperation(MTL::BlendOperationAdd);
		// colorAttachment->setAlphaBlendOperation(MTL::BlendOperationAdd);
		// colorAttachment->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
		// colorAttachment->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);

		// 6. Crear Estado
		m_PipelineState = device->newRenderPipelineState(pipelineDesc, &error);

		if (!m_PipelineState)
			CE_CORE_ERROR("Error pipeline: {}", error->localizedDescription()->utf8String());

		vertexDesc->release();
		pipelineDesc->release();
		nsSource->release();
	}
} // namespace cabrankengine::platform::metal
