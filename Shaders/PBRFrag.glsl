//#version 330 core
//
//out vec4 FragColor;
//
//in vec2 TexCoords;
//in vec3 Normal;
//in vec3 WorldPos;
//
//uniform vec3 camPos;
//
//uniform vec3 Albedo;
//uniform float Metallic;
//uniform float Roughness;
//uniform float AO;
//
//uniform vec3 lightPositions[4];
//uniform vec3 lightColors[4];
//
//#define PI 3.14159265359
//
//float NDF(vec3 N, vec3 H, float roughness)
//{
//	float a = roughness * roughness;
//	float a2 = a * a;
//	float NdotH = max(dot(N, H), 0.0);
//	float NdotH2 = NdotH * NdotH;
//	float numerator = a2;
//	float denominator = ((NdotH2)*(a2 - 1.0) + 1.0);
//	denominator = PI * (denominator * denominator);
//
//	return numerator / denominator;
//};
//
//vec3 Fresnel(float HdotV, vec3 F0)
//{
//	return F0 + (1 - F0) * pow(clamp(1 - (HdotV),0.0,1.0), 5.0);
//}
//
//float GGX_sub(vec3 N, vec3 V, float roughness)
//{
//	float r = (roughness + 1.0);
//	float k = (r * r) / 8.0;
//	float NdotV = max(dot(N,V), 0.0);
//	float numerator = NdotV;
//	float denominator = (NdotV) * (1.0 - k) + k;
//
//	return numerator / denominator;
//}
//
//float GGX(vec3 N, vec3 V, vec3 L, float k)
//{
//	float GGV = GGX_sub(N,V,k);
//	float GGL = GGX_sub(N,L,k);
//	return GGV * GGL;
//}
//
//void main()
//{
//	vec3 N = normalize(Normal);
//	vec3 V = normalize(camPos - WorldPos);
//
//	vec3 L;
//	vec3 H;
//
//	float dist = 0.0;
//	float attenuation = 0.0;
//	vec3 radiance = vec3(0.0);
//	
//	vec3 F0 = vec3(0.0);;
//	vec3 F = vec3(0.0);;
//	
//	float D = 0.0;
//	float G = 0.0;
//
//	vec3 numerator = vec3(0.0);
//	float denominator = 0.0;
//	vec3 specular = vec3(0.0);
//
//	vec3 Ks = vec3(0.0);
//	vec3 Kd = vec3(0.0);
//
//	float NdotL = 0.0;
//
//	F0 = vec3(0.04);
//	F0 = mix(F0, Albedo, Metallic);
//	
//	vec3 Lo = vec3(0.0);
//	for(int i = 0; i < 4; ++i)
//	{
//		L = normalize(lightPositions[i] - WorldPos);
//		H = normalize(V + L);
//
//		dist = length(lightPositions[i] - WorldPos);
//		attenuation = 1.0 / (dist * dist);s
//		radiance = lightColors[i] * attenuation;
//
//		D = NDF(N, H, Roughness);
//		G = GGX(N, V, L, Roughness);
//		F = Fresnel(clamp(dot(H,V),0.0,1.0), F0);
//
//		numerator = D * G * F;
//		denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N,L) ,0.0);
//		specular = numerator / max(denominator, 0.0001);
//
//		Ks = F;
//		Kd = vec3(1.0) - Ks;
//
//		Kd *= 1.0 - Metallic;
//
//		NdotL = max(dot(N,L), 0.0);
//		Lo += (Kd * Albedo / PI + specular) * radiance * NdotL;
//	}
//
//	vec3 ambient = vec3(0.03) * Albedo * AO;
//	vec3 color = ambient + Lo;
//
//	color = color / (color + vec3(1.0));
//	color = pow(color, vec3(1.0/2.2));
//
//	FragColor = vec4(color, 1.0);
//
//}

#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// material parameters
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
void main()
{		
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 1; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}