#version 330 core

struct Material
{
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
};

struct directional_light
{
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

struct point_light
{
    vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    
    // Attenuation 
    float Constant;
    float Linear;
    float Quadratic;
};

struct spot_light
{
    vec3 Position;
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    
    // Smoothing boundary
    float NearRadius;
    float FarRadius;
};

vec3 CalculateSpotLight(spot_light Light, vec3 FragmentPosition, vec3 Normal, 
                        vec3 ViewDirection, vec3 DiffuseColor, vec3 SpecularColor);

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform spot_light SpotLight;

// Point light (add to struct?)
uniform float constant;
uniform float linear;
uniform float quadratic;


void main()
{
    vec3 ViewDirection = normalize(viewPos - FragPos);
    vec3 DiffuseColor = texture(material.diffuseMap, TexCoord).rgb;
    vec3 SpecularColor = texture(material.specularMap, TexCoord).rgb;
    vec3 SurfaceNormal = normalize(Normal);
    
    vec3 SpotLightComponent = CalculateSpotLight(SpotLight, FragPos, SurfaceNormal, ViewDirection, DiffuseColor, SpecularColor);
    
    FragColor = vec4(SpotLightComponent, 1.0);
}

vec3 CalculateSpotLight(spot_light Light, vec3 FragmentPosition, vec3 SurfaceNormal, 
                        vec3 ViewDirection, vec3 DiffuseColor, vec3 SpecularColor)
{
    vec3 LightToFrag = normalize(Light.Position - FragmentPosition);
    
    float Theta = dot(LightToFrag, normalize(-Light.Direction));
    float Epsilon = (Light.NearRadius - Light.FarRadius);
    float Intensity = clamp((Theta - Light.FarRadius) / Epsilon, 0.0, 1.0);
    
    vec3 AmbientComponent = Light.Ambient * DiffuseColor;
    
    vec3 LightDirection = normalize(Light.Position - FragmentPosition);
    float AngleOfIncidence = max(dot(SurfaceNormal, LightDirection), 0);
    vec3 DiffuseComponent = Light.Diffuse * AngleOfIncidence * Intensity * DiffuseColor;
    
    vec3 ReflectionDirection = reflect(-Light.Direction, SurfaceNormal);
    vec3 AngleToReflectedLight = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), Light.Specular);
    vec3 SpecularComponent = Light.Specular * AngleToReflectedLight * Intensity * SpecularColor;
    
    return AmbientComponent + DiffuseComponent + SpecularComponent;
}

vec3 CalculateDirectionalLight(directional_light Light, vec3 FragmentPosition, vec3 ViewDirection, vec3 DiffuseColor, vec3 SpecularColor)
{
    vec3 AmbientComponent = Light.Ambient * DiffuseColor;
    
    vec3 LightDirection = normalize(-Light.Direction);
    float AngleOfIncidence = max(dot(SurfaceNormal, LightDirection), 0.0); // diffuse
    vec3 DiffuseComponent = Light.Diffuse * AngleOfIncidence * DiffuseColor;
    
    // Angle between our POV and the angle of reflection I think...
    vec3 ReflectionDirection = reflect(-LightDirection, SurfaceNormal);
    float AngleToReflectedLight = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), Light.Specular);
    vec3 SpecularComponent = Light.Specular * AngleToReflectedLight * SpecularColor;
    
    return AmbientComponent + DiffuseComponent + SpecularComponent;
}

vec3 CalculatePointLight(point_light Light, vec3 FragmentPosition, vec3 ViewDirection, vec3 DiffuseColor, vec3 SpecularColor)
{
    
    float DistanceToFragment = length(Light.Position - FragmentPosition);
    float Attenuation = 1.0 / (Light.Constant + 
                               (Light.Linear * DistanceToFragment) + 
                               (Light.Quadratic * DistanceToFragment * DistanceToFragment));
    
    vec3 AmbientComponent = Light.Ambient * Attenuation * DiffuseColor;
    
    vec3 LightToFrag = normalize(Light.Position - FragmentPosition);
    float AngleOfIncidence = max(dot(SurfaceNormal, LightToFrag), 0);
    vec3 DiffuseComponent = Light.Diffuse * Attenuation * DiffuseColor;
    
    vec3 ReflectionDirection = reflect(-Light.Direction, SurfaceNormal);
    vec3 AngleToReflectedLight = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), Light.Specular);
    vec3 SpecularComponent = Light.Specular * AngleToReflectedLight * Attenuation * SpecularColor;
    
    return AmbientComponent + DiffuseComponent + SpecularComponent;
}