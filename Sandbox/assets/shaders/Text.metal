#include <metal_stdlib>
using namespace metal;

struct VertexOut {
    float4 position [[position]];
    float4 color;
};

// Shader simple con datos hardcoded para probar el pipeline
vertex VertexOut vertex_main(uint vertexID [[vertex_id]]) {
    float2 positions[3] = { {0.0, 0.5}, {0.5, -0.5}, {-0.5, -0.5} };
    float3 colors[3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };

    VertexOut out;
    out.position = float4(positions[vertexID], 0.0, 1.0);
    out.color = float4(colors[vertexID], 1.0);
    return out;
}

fragment float4 fragment_main(VertexOut in [[stage_in]]) {
    return in.color;
}