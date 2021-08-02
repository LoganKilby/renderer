#version 330 core

struct material
{
    sampler2D DiffuseMaps[40];
    sampler2D SpecularMaps[40];
    float Shininess;
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
    
    // Attenuation 
    float Constant;
    float Linear;
    float Quadratic;
};

vec3 CalculateSpotLight(spot_light Light, vec3 FragmentPosition, vec3 Normal, 
                        vec3 ViewDirection, vec3 DiffuseColor, vec3 SpecularColor, float Shininess);
vec3 CalculateDirectionalLight(directional_light Light, vec3 FragmentPosition, vec3 SurfaceNormal, 
                               vec3 ViewDirection, vec3 DiffuseColor, vec3 SpecularColor, float Shininess);
vec3 CalculatePointLight(point_light Light, vec3 FragmentPosition, vec3 SurfaceNormal,
                         vec3 ViewDirection, vec3 DiffuseColor, vec3 SpecularColor, float Shininess);

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 ViewPosition;
uniform material CrateMaterial;
uniform spot_light SpotLight;
uniform point_light PointLights[4];
uniform directional_light DirectionalLight;

void main()
{
    vec3 ViewDirection = normalize(ViewPosition - FragPos);
    vec3 DiffuseColor = texture(CrateMaterial.DiffuseMaps[0], TexCoord).rgb;
    vec3 SpecularColor = texture(CrateMaterial.SpecularMaps[0], TexCoord).rgb;
    vec3 SurfaceNormal = normalize(Normal);
    
    vec3 Result = CalculateDirectionalLight(DirectionalLight, FragPos, SurfaceNormal, 
                                            ViewDirection, DiffuseColor, SpecularColor,
                                            32.0);
    
    FragColor = vec4(Result, 1.0);
}

vec3 CalculateSpotLight(spot_light Light, vec3 FragmentPosition, vec3 SurfaceNormal, 
                        vec3 ViewDirection, vec3 DiffuseColor, vec3 SpecularColor, float Shininess)
{
    vec3 LightToFrag = normalize(Light.Position - FragmentPosition);
    // Diffuse shading
    float AngleOfIncidence = max(dot(SurfaceNormal, LightToFrag), 0.0);
    // Specular shading
    vec3 ReflectionDirection = reflect(-LightToFrag, SurfaceNormal);
    float AngleToReflectedLight = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), Shininess);
    
    // Attenuation
    float DistanceToFragment = length(Light.Position - FragmentPosition);
    float Attenuation = 1.0 / ((Light.Constant) + 
                               (Light.Linear * DistanceToFragment) + 
                               (Light.Quadratic * (DistanceToFragment * DistanceToFragment)));
    
    // Spotlight
    float Theta = dot(LightToFrag, normalize(-Light.Direction));
    float Epsilon = (Light.NearRadius - Light.FarRadius);
    float Intensity = clamp((Theta - Light.FarRadius) / Epsilon, 0.0, 1.0);
    
    vec3 AmbientComponent = Light.Ambient * DiffuseColor;
    
    vec3 DiffuseComponent = Light.Diffuse * AngleOfIncidence * DiffuseColor;
    
    vec3 SpecularComponent = Light.Specular * AngleToReflectedLight * SpecularColor;
    
    AmbientComponent *= Attenuation;
    DiffuseComponent *= Attenuation;
    SpecularComponent *= Attenuation;
    
    DiffuseComponent *= Intensity;
    SpecularComponent *= Intensity;
    
    return AmbientComponent + DiffuseComponent + SpecularComponent;
}

vec3 CalculateDirectionalLight(directional_light Light, vec3 FragmentPosition, vec3 SurfaceNormal, 
                               vec3 ViewDirection, vec3 DiffuseColor, vec3 SpecularColor, float Shininess)
{
    vec3 AmbientComponent = Light.Ambient * DiffuseColor;
    
    vec3 LightDirection = normalize(-Light.Direction);
    float AngleOfIncidence = max(dot(SurfaceNormal, LightDirection), 0.0); // diffuse
    vec3 DiffuseComponent = Light.Diffuse * AngleOfIncidence * DiffuseColor;
    
    vec3 ReflectionDirection = reflect(-LightDirection, SurfaceNormal);
    float AngleToReflectedLight = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), Shininess);
    vec3 SpecularComponent = Light.Specular * AngleToReflectedLight * SpecularColor;
    
    return AmbientComponent + DiffuseComponent + SpecularComponent;
}

vec3 CalculatePointLight(point_light Light, vec3 FragmentPosition, vec3 SurfaceNormal,
                         vec3 ViewDirection, vec3 DiffuseColor, vec3 SpecularColor, float Shininess)
{
    
    float DistanceToFragment = length(Light.Position - FragmentPosition);
    float Attenuation = 1.0 / (Light.Constant + 
                               (Light.Linear * DistanceToFragment) + 
                               (Light.Quadratic * DistanceToFragment * DistanceToFragment));
    
    vec3 AmbientComponent = Light.Ambient * Attenuation * DiffuseColor;
    
    vec3 LightToFrag = normalize(Light.Position - FragmentPosition);
    float AngleOfIncidence = max(dot(SurfaceNormal, LightToFrag), 0);
    vec3 DiffuseComponent = Light.Diffuse * Attenuation * DiffuseColor;
    
    vec3 ReflectionDirection = reflect(-LightToFrag, SurfaceNormal);
    float AngleToReflectedLight = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), Shininess);
    vec3 SpecularComponent = Light.Specular * AngleToReflectedLight * Attenuation * SpecularColor;
    
    return AmbientComponent + DiffuseComponent + SpecularComponent;
}