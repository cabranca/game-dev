#include <metal_stdlib>
using namespace metal;

// Must match the BufferLayout in Renderer2D:
// { Float3 "a_Position", Float4 "a_Color", Float2 "a_TexCoord", Float "a_TexIndex", Float "a_TilingFactor" }
struct VertexIn {
    float3 position    [[attribute(0)]];
    float4 color       [[attribute(1)]];
    float2 texCoord    [[attribute(2)]];
    float  texIndex    [[attribute(3)]];
    float  tilingFactor [[attribute(4)]];
};

struct VertexOut {
    float4 position     [[position]];
    float4 color;
    float2 texCoord;
    float  texIndex;
    float  tilingFactor;
};

// Uniform buffer at index 30 to avoid collision with vertex buffers at 0, 1, 2...
struct Uniforms {
    float4x4 viewProjection;
};

vertex VertexOut vertex_main(VertexIn in [[stage_in]],
                             constant Uniforms& uniforms [[buffer(30)]]) {
    VertexOut out;
    out.position     = uniforms.viewProjection * float4(in.position, 1.0);
    out.color        = in.color;
    out.texCoord     = in.texCoord;
    out.texIndex     = in.texIndex;
    out.tilingFactor = in.tilingFactor;
    return out;
}

fragment float4 fragment_main(VertexOut in [[stage_in]],
                              array<texture2d<float>, 32> textures [[texture(0)]]) {
    constexpr sampler texSampler(mag_filter::linear, min_filter::linear,
                                 address::repeat);
    int idx = int(in.texIndex);
    float4 texColor = textures[idx].sample(texSampler, in.texCoord * in.tilingFactor);
    return texColor * in.color;
}
