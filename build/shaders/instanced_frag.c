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

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform material Materials;
uniform directional_light DirectionalLight;
uniform vec3 ViewPosition;

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

void main()
{
    vec3 DiffuseColor = texture(Materials.DiffuseMaps[0], TexCoord).rgb;
    vec3 SpecularColor = vec3(0.01, 0.01, 0.01); // Add a specular map
    vec3 ViewDirection = normalize(ViewPosition - FragPos);
    vec3 SurfaceNormal = normalize(Normal);
    vec3 Result = CalculateDirectionalLight(DirectionalLight, FragPos, SurfaceNormal, 
                                            ViewDirection, DiffuseColor, SpecularColor, 32.0);
    FragColor = vec4(Result, 1.0);
}