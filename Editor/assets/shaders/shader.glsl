#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(binding = 0, std140) uniform Transforms
{
    mat4 u_M;
    mat4 u_V;
    mat4 u_P;
}tranforms;

layout(location = 0) out vec2 v_TexCoord;
layout(location = 1) out vec3 v_WorldPosition;
layout(location = 2) out vec3 v_Normal;

void main()
{
	v_TexCoord = a_TexCoord;
	v_WorldPosition = vec3(tranforms.u_M * vec4(a_Position, 1.0));
	v_Normal = transpose(inverse(mat3(tranforms.u_M))) * a_Normal;
	gl_Position = tranforms.u_P * tranforms.u_V * tranforms.u_M * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) in vec2 v_TexCoord;
layout(location = 1) in vec3 v_WorldPosition;
layout(location = 2) in vec3 v_Normal;

layout(location = 0) out vec4 finalColor;

layout(binding = 1, std140) uniform Light
{
    vec3 u_LightPosition[4];
    vec3 u_LightColor[4];
    float u_LightIntensity[4];
} light;

layout(binding = 2, std140) uniform Material
{
    vec3 u_Albedo;
    float u_Metallic;
    float u_Roughness;
    float u_AmbientOcclusion;

    int u_HasAlbedoMap;
    int u_HasNormalMap;
    int u_HasRoughnessMap;
    int u_HasMetallicMap;
    int u_HasAmbientOcclusionMap;
} material;

layout(binding = 3, std140) uniform Camera
{
    vec3 u_CameraPosition;
} camera;

layout(binding = 0) uniform sampler2D u_AlbedoMap;
layout(binding = 1) uniform sampler2D u_NormalMap;
layout(binding = 2) uniform sampler2D u_RoughnessMap;
layout(binding = 3) uniform sampler2D u_MetallicMap;
layout(binding = 4) uniform sampler2D u_AmbientOcclusionMap;

// Constant for PI
const float PI = 3.14159265359;
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(u_NormalMap, v_TexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(v_WorldPosition);
    vec3 Q2  = dFdy(v_WorldPosition);
    vec2 st1 = dFdx(v_TexCoord);
    vec2 st2 = dFdy(v_TexCoord);

    vec3 N   = normalize(v_Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// Normal Distribution Function (GGX)
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return num / denom;
}

// Geometry function (Schlick-GGX)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    
    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return num / denom;
}

// Smith's method: combines geometry for view and light directions
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

// Fresnel term using Schlick's approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
    vec3 N = normalize(v_Normal);
    if(material.u_HasNormalMap == 1){
        N = getNormalFromMap();
    }
    vec3 albedo = material.u_Albedo;
    if(material.u_HasAlbedoMap == 1){
        albedo = pow(texture(u_AlbedoMap, v_TexCoord).rgb, vec3(2.2));
    }
    float metallic = material.u_Metallic;
    if(material.u_HasMetallicMap == 1){
        metallic = texture(u_MetallicMap, v_TexCoord).r;
    }
    float roughness = material.u_Roughness;
    if(material.u_HasRoughnessMap == 1){
        roughness = texture(u_RoughnessMap, v_TexCoord).r;
    }
    float ambientOcclusion = material.u_AmbientOcclusion;
    if(material.u_HasAmbientOcclusionMap == 1){
        ambientOcclusion = texture(u_AmbientOcclusionMap, v_TexCoord).r;
    }

    // Compute view vector (from fragment to camera)
    vec3 V = normalize(camera.u_CameraPosition - v_WorldPosition);

    // Base reflectance at normal incidence.
    // For dielectrics, a common value is 0.04. Metallic surfaces use the albedo color.
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // Accumulate outgoing radiance from each light
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i)
    {
        // Light direction vector (from fragment to light)
        vec3 L = normalize(light.u_LightPosition[i] - v_WorldPosition);
        // Half-vector between view and light direction
        vec3 H = normalize(V + L);
        // Compute distance and apply quadratic attenuation
        float distance = length(light.u_LightPosition[i] - v_WorldPosition);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = light.u_LightColor[i] * light.u_LightIntensity[i] * attenuation;
        
        // Calculate the cosine of the angle between the normal and light direction
        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0)
        {
            // Compute microfacet distribution, geometry, and Fresnel terms
            float D = DistributionGGX(N, H, roughness);
            float G = GeometrySmith(N, V, L, roughness);
            vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
            
            // Cook-Torrance BRDF: specular reflection component
            vec3 numerator = D * G * F;
            float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL + 0.001;
            vec3 specular = numerator / denominator;
            
            // kS is the specular reflection component (energy conservation)
            vec3 kS = F;
            // kD is the diffuse component (reduced for metallic materials)
            vec3 kD = vec3(1.0) - kS;
            kD *= 1.0 - metallic;
            
            // Lambertian diffuse term
            vec3 diffuse = albedo / PI;
            
            // Add light contribution
            Lo += (kD * diffuse + specular) * radiance * NdotL;
        }
    }
    
    // Ambient lighting term using ambient occlusion
    vec3 ambient = vec3(0.03) * albedo * ambientOcclusion;
    
    // Final color combines direct lighting and ambient light
    vec3 color = ambient + Lo;
    
    // Apply simple HDR tonemapping (Reinhard) and gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));
    
    finalColor = vec4(color, 1.0);
}