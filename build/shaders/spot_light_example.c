#version 330 core

struct material
{
    sampler2D DiffuseMap;
    sampler2D SpecularMap;
    float Shininess;
};

struct spot_light
{
    vec3 Position; // Camera pos
    vec3 Direction; // Camera front vector
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    
    float Constant;
    float Linear;
    float Quadratic;
    
    float NearRadius;
    float FarRadius;
};

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 ViewPosition;
uniform material Material;
uniform spot_light SpotLight;

// Point light (add to struct?)

void main()
{
    
    // Ambient                                  trucating the texture (vec4) to a vec3 (rgb)
    vec3 ambientComponent = SpotLight.Ambient * texture(Material.DiffuseMap, TexCoord).rgb;
    
    vec3 lightToFrag = normalize(SpotLight.Position - FragPos);
    vec3 surfaceNormal = normalize(Normal);
    
    // Diffuse
    float angleOfIncidence = max(dot(surfaceNormal, lightToFrag), 0.0);
    vec3 diffuseComponent = angleOfIncidence * SpotLight.Diffuse * texture(Material.DiffuseMap, TexCoord).rgb;
    
    // Specular
    vec3 viewDirection = normalize(ViewPosition - FragPos);
    vec3 reflectionDirection = reflect(-lightToFrag, surfaceNormal);
    float angleOfReflection = pow(max(dot(viewDirection, reflectionDirection), 0.0), Material.Shininess);
    vec3 specularComponent = texture(Material.SpecularMap, TexCoord).rgb * angleOfReflection * SpotLight.Specular;
    
    float theta = dot(lightToFrag, normalize(-SpotLight.Direction));
    float epsilon = (SpotLight.NearRadius - SpotLight.FarRadius);
    float intensity = clamp((theta - SpotLight.FarRadius) / epsilon, 0.0, 1.0);
    diffuseComponent *= intensity;
    specularComponent *= intensity;
    
    float pLightDistance = length(SpotLight.Position - FragPos);
    float attenuation = 1.0 / (SpotLight.Constant + SpotLight.Linear * pLightDistance + SpotLight.Quadratic * (pLightDistance * pLightDistance));
    ambientComponent *= attenuation;
    diffuseComponent *= attenuation;
    specularComponent *= attenuation;
    
    FragColor = vec4(ambientComponent + diffuseComponent + specularComponent, 1.0);
}