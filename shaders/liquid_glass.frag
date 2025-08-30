#version 450 core

// Liquid Glass Fragment Shader
// Ultra-realistic glass rendering with advanced effects

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragTangent;
layout(location = 4) in vec3 fragBitangent;
layout(location = 5) in vec3 viewPos;
layout(location = 6) in vec3 lightPos;
layout(location = 7) in vec4 fragPosLightSpace;
layout(location = 8) in float waveHeight;
layout(location = 9) in vec3 surfaceVelocity;

layout(set = 0, binding = 1) uniform GlassProperties {
    float refractionIndex;
    float thickness;
    float clarity;
    float iridescence;
    float surfaceTension;
    float causticsStrength;
    float dispersion;
    float absorption;
    vec3 tint;
    float roughness;
    float metallic;
    float subsurface;
} glass;

layout(set = 0, binding = 2) uniform sampler2D normalMap;
layout(set = 0, binding = 3) uniform sampler2D heightMap;
layout(set = 0, binding = 4) uniform sampler2D environmentMap;
layout(set = 0, binding = 5) uniform sampler2D causticsMap;
layout(set = 0, binding = 6) uniform sampler2D noiseTexture;
layout(set = 0, binding = 7) uniform sampler2D shadowMap;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 fragNormalOut;
layout(location = 2) out vec4 fragMaterial;
layout(location = 3) out vec2 fragMotionVector;
layout(location = 4) out vec4 fragVolumetric;

const float PI = 3.14159265359;

// Advanced noise functions
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

float noise2D(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    
    vec2 u = f * f * (3.0 - 2.0 * f);
    
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

// Fresnel calculation with complex IOR
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// Advanced glass refraction with dispersion
vec3 calculateRefraction(vec3 viewDir, vec3 normal, float ior, float dispersion) {
    vec3 refractionDir = vec3(0.0);
    
    // Calculate refraction for RGB channels separately for dispersion
    float iorR = ior + dispersion * 0.02;
    float iorG = ior;
    float iorB = ior - dispersion * 0.02;
    
    vec3 refractR = refract(viewDir, normal, 1.0 / iorR);
    vec3 refractG = refract(viewDir, normal, 1.0 / iorG);
    vec3 refractB = refract(viewDir, normal, 1.0 / iorB);
    
    // Sample environment with slight offset for each channel
    vec3 envR = texture(environmentMap, (refractR.xy + 1.0) * 0.5).rgb;
    vec3 envG = texture(environmentMap, (refractG.xy + 1.0) * 0.5).rgb;
    vec3 envB = texture(environmentMap, (refractB.xy + 1.0) * 0.5).rgb;
    
    return vec3(envR.r, envG.g, envB.b);
}

// Subsurface scattering for thick glass
vec3 calculateSubsurface(vec3 lightDir, vec3 viewDir, vec3 normal, float thickness) {
    float subsurfaceStrength = glass.subsurface;
    
    // Calculate light penetration
    float penetration = max(0.0, dot(-lightDir, normal));
    
    // Beer's law for light absorption
    float absorption = exp(-glass.absorption * thickness);
    
    // Subsurface color contribution
    vec3 subsurfaceColor = glass.tint * penetration * absorption * subsurfaceStrength;
    
    return subsurfaceColor;
}

// Caustics calculation
vec3 calculateCaustics(vec3 worldPos, vec3 normal) {
    // Sample caustics texture with animated coordinates
    vec2 causticsCoord1 = worldPos.xz * 0.1 + surfaceVelocity.xz * 0.5;
    vec2 causticsCoord2 = worldPos.xz * 0.15 + surfaceVelocity.xz * 0.3;
    
    float caustics1 = texture(causticsMap, causticsCoord1).r;
    float caustics2 = texture(causticsMap, causticsCoord2).g;
    
    float causticsIntensity = (caustics1 + caustics2) * 0.5;
    causticsIntensity = pow(causticsIntensity, 2.0) * glass.causticsStrength;
    
    // Fade caustics based on surface angle
    float angleFade = max(0.0, dot(normal, vec3(0.0, 1.0, 0.0)));
    causticsIntensity *= angleFade;
    
    return vec3(causticsIntensity) * glass.tint;
}

// Iridescence calculation
vec3 calculateIridescence(vec3 viewDir, vec3 normal, float iridescenceFactor) {
    float cosTheta = abs(dot(viewDir, normal));
    
    // Thin film interference
    float phase = 2.0 * PI * glass.thickness * cosTheta;
    
    // RGB wavelengths (approximate)
    float wavelengthR = 700e-9;
    float wavelengthG = 530e-9;
    float wavelengthB = 440e-9;
    
    float interferenceR = sin(phase / wavelengthR);
    float interferenceG = sin(phase / wavelengthG);
    float interferenceB = sin(phase / wavelengthB);
    
    vec3 iridescenceColor = vec3(interferenceR, interferenceG, interferenceB);
    iridescenceColor = iridescenceColor * 0.5 + 0.5; // Normalize to [0,1]
    
    return mix(vec3(1.0), iridescenceColor, iridescenceFactor);
}

// Volume rendering for thick glass
vec4 calculateVolumetricEffects(vec3 rayStart, vec3 rayDir, float maxDist) {
    vec4 volumetricColor = vec4(0.0);
    float stepSize = maxDist / 32.0; // 32 samples
    
    for (int i = 0; i < 32; i++) {
        float t = float(i) * stepSize;
        vec3 samplePos = rayStart + rayDir * t;
        
        // Sample density (could be based on noise or procedural patterns)
        float density = noise2D(samplePos.xz * 0.5) * 0.1;
        
        // Light contribution
        vec3 lightContrib = glass.tint * density * stepSize;
        
        // Accumulate color
        volumetricColor.rgb += lightContrib * (1.0 - volumetricColor.a);
        volumetricColor.a += density * stepSize;
        
        if (volumetricColor.a >= 1.0) break;
    }
    
    return volumetricColor;
}

// Advanced lighting model for glass
vec3 calculateGlassLighting(vec3 viewDir, vec3 normal, vec3 lightDir, vec3 lightColor) {
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    // Fresnel for glass
    vec3 F0 = vec3(0.04); // Glass F0
    vec3 fresnel = fresnelSchlickRoughness(max(dot(halfwayDir, viewDir), 0.0), F0, glass.roughness);
    
    // Reflection
    vec3 reflectDir = reflect(-viewDir, normal);
    vec3 reflectionColor = texture(environmentMap, (reflectDir.xy + 1.0) * 0.5).rgb;
    
    // Refraction with dispersion
    vec3 refractionColor = calculateRefraction(viewDir, normal, glass.refractionIndex, glass.dispersion);
    
    // Mix reflection and refraction based on Fresnel
    vec3 glassColor = mix(refractionColor, reflectionColor, fresnel);
    
    // Apply glass tint
    glassColor *= glass.tint;
    
    // Add subsurface scattering
    vec3 subsurface = calculateSubsurface(lightDir, viewDir, normal, glass.thickness);
    glassColor += subsurface;
    
    // Add caustics
    vec3 caustics = calculateCaustics(fragPos, normal);
    glassColor += caustics;
    
    // Apply iridescence
    vec3 iridescence = calculateIridescence(viewDir, normal, glass.iridescence);
    glassColor *= iridescence;
    
    return glassColor;
}

void main() {
    // Calculate view direction
    vec3 viewDir = normalize(viewPos - fragPos);
    
    // Sample and apply normal map
    vec3 normalMapSample = texture(normalMap, fragTexCoord).rgb * 2.0 - 1.0;
    
    // Create TBN matrix
    mat3 TBN = mat3(normalize(fragTangent), normalize(fragBitangent), normalize(fragNormal));
    vec3 normal = normalize(TBN * normalMapSample);
    
    // Light direction (simplified for demo)
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 lightColor = vec3(1.0);
    
    // Calculate glass lighting
    vec3 glassColor = calculateGlassLighting(viewDir, normal, lightDir, lightColor);
    
    // Calculate glass transparency
    float fresnel = fresnelSchlick(max(dot(normal, viewDir), 0.0), vec3(0.04)).r;
    float alpha = mix(glass.clarity, 1.0, fresnel);
    
    // Apply thickness-based transparency
    float thicknessAlpha = exp(-glass.absorption * glass.thickness);
    alpha *= thicknessAlpha;
    
    // Calculate volumetric effects for thick glass
    vec4 volumetric = calculateVolumetricEffects(fragPos, -viewDir, glass.thickness);
    
    // Final glass color
    fragColor = vec4(glassColor, alpha);
    
    // Output normal for deferred rendering
    fragNormalOut = vec4(normal * 0.5 + 0.5, 1.0);
    
    // Output material properties
    fragMaterial = vec4(glass.roughness, glass.metallic, glass.refractionIndex / 3.0, glass.thickness);
    
    // Calculate motion vectors
    vec2 currentScreenPos = gl_FragCoord.xy / textureSize(normalMap, 0);
    vec2 previousScreenPos = currentScreenPos - surfaceVelocity.xy * 0.01;
    fragMotionVector = currentScreenPos - previousScreenPos;
    
    // Output volumetric information
    fragVolumetric = volumetric;
}
