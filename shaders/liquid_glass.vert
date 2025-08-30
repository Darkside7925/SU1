#version 450 core

// Liquid Glass Vertex Shader
// Ultra-realistic glass rendering with fluid dynamics

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inTangent;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 normalMatrix;
    float time;
    float waveAmplitude;
    float waveFrequency;
    float glassThickness;
    vec3 cameraPos;
    vec3 lightPos;
    float refractionIndex;
    float surfaceTension;
} ubo;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 fragTangent;
layout(location = 4) out vec3 fragBitangent;
layout(location = 5) out vec3 viewPos;
layout(location = 6) out vec3 lightPos;
layout(location = 7) out vec4 fragPosLightSpace;
layout(location = 8) out float waveHeight;
layout(location = 9) out vec3 surfaceVelocity;

// Noise functions for realistic glass surface
float noise3D(vec3 p) {
    return fract(sin(dot(p, vec3(12.9898, 78.233, 54.53))) * 43758.5453);
}

float fractalNoise(vec3 p) {
    float result = 0.0;
    float amplitude = 1.0;
    float frequency = 1.0;
    
    for (int i = 0; i < 6; i++) {
        result += amplitude * (noise3D(p * frequency) * 2.0 - 1.0);
        amplitude *= 0.5;
        frequency *= 2.0;
    }
    
    return result;
}

// Fluid dynamics simulation for glass surface
vec3 calculateFluidDisplacement(vec3 pos) {
    float t = ubo.time;
    
    // Primary wave motion
    float wave1 = sin(pos.x * ubo.waveFrequency + t * 2.0) * ubo.waveAmplitude;
    float wave2 = cos(pos.z * ubo.waveFrequency * 1.5 + t * 1.5) * ubo.waveAmplitude * 0.7;
    
    // Secondary ripples
    float ripple1 = sin(pos.x * ubo.waveFrequency * 3.0 + pos.z * ubo.waveFrequency * 2.0 + t * 4.0) * ubo.waveAmplitude * 0.3;
    float ripple2 = cos(pos.x * ubo.waveFrequency * 2.5 + pos.z * ubo.waveFrequency * 3.5 + t * 3.0) * ubo.waveAmplitude * 0.2;
    
    // Noise-based micro-displacement
    vec3 noisePos = pos * 5.0 + vec3(t * 0.5);
    float noise = fractalNoise(noisePos) * ubo.waveAmplitude * 0.1;
    
    // Combine all displacement components
    float totalDisplacement = wave1 + wave2 + ripple1 + ripple2 + noise;
    
    // Calculate surface normal perturbation
    float dx = sin((pos.x + 0.01) * ubo.waveFrequency + t * 2.0) * ubo.waveAmplitude - wave1;
    float dz = cos((pos.z + 0.01) * ubo.waveFrequency * 1.5 + t * 1.5) * ubo.waveAmplitude * 0.7 - wave2;
    
    return vec3(dx * 100.0, totalDisplacement, dz * 100.0);
}

// Surface tension effects
vec3 applySurfaceTension(vec3 pos, vec3 normal) {
    float tension = ubo.surfaceTension;
    
    // Calculate curvature approximation
    vec3 offset = 0.01 * normal;
    vec3 pos1 = pos + offset;
    vec3 pos2 = pos - offset;
    
    vec3 disp1 = calculateFluidDisplacement(pos1);
    vec3 disp2 = calculateFluidDisplacement(pos2);
    
    float curvature = length(disp1 - disp2) / 0.02;
    
    // Apply surface tension force
    vec3 tensionForce = normal * tension * curvature * 0.1;
    
    return tensionForce;
}

// Glass thickness variation
float calculateThickness(vec3 pos) {
    float baseThickness = ubo.glassThickness;
    
    // Add variation based on position and time
    float variation = fractalNoise(pos * 2.0 + vec3(ubo.time * 0.2)) * 0.3 + 0.7;
    
    return baseThickness * variation;
}

void main() {
    // Apply fluid displacement to vertex position
    vec3 displacement = calculateFluidDisplacement(inPosition);
    vec3 modifiedPos = inPosition + displacement * 0.01; // Scale down displacement
    
    // Calculate surface tension effects
    vec3 tensionEffect = applySurfaceTension(inPosition, inNormal);
    modifiedPos += tensionEffect;
    
    // Transform position
    vec4 worldPos = ubo.model * vec4(modifiedPos, 1.0);
    fragPos = worldPos.xyz;
    
    // Calculate modified normal with fluid dynamics
    vec3 pos1 = inPosition + vec3(0.01, 0.0, 0.0);
    vec3 pos2 = inPosition + vec3(0.0, 0.0, 0.01);
    vec3 disp1 = calculateFluidDisplacement(pos1);
    vec3 disp2 = calculateFluidDisplacement(pos2);
    
    vec3 tangentDisp = normalize(disp1 - displacement);
    vec3 bitangentDisp = normalize(disp2 - displacement);
    vec3 modifiedNormal = normalize(cross(tangentDisp, bitangentDisp));
    
    // Blend with original normal
    modifiedNormal = normalize(mix(inNormal, modifiedNormal, 0.5));
    
    // Transform normal
    fragNormal = normalize(mat3(ubo.normalMatrix) * modifiedNormal);
    
    // Calculate tangent and bitangent
    fragTangent = normalize(mat3(ubo.model) * inTangent);
    fragBitangent = normalize(cross(fragNormal, fragTangent));
    
    // Pass through texture coordinates
    fragTexCoord = inTexCoord;
    
    // Pass camera and light positions
    viewPos = ubo.cameraPos;
    lightPos = ubo.lightPos;
    
    // Calculate wave height for thickness variation
    waveHeight = displacement.y;
    
    // Calculate surface velocity for motion blur and foam effects
    vec3 prevDisplacement = calculateFluidDisplacement(inPosition - vec3(0.016)); // Assume 60fps
    surfaceVelocity = (displacement - prevDisplacement) / 0.016;
    
    // Light space position for shadow mapping
    mat4 lightSpaceMatrix = mat4(1.0); // Would be passed as uniform in real implementation
    fragPosLightSpace = lightSpaceMatrix * worldPos;
    
    // Final position
    gl_Position = ubo.proj * ubo.view * worldPos;
}
