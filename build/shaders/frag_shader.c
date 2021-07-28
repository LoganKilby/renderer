#version 330 core

struct Material
{
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
};

struct Light
{
    vec3 position; // Camera pos
    vec3 direction; // Camera front vector
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

// Point light (add to struct?)
uniform float constant;
uniform float linear;
uniform float quadratic;
uniform float spotlightBoundary;
uniform float spotlightBoundaryEnd;

void main()
{
    vec3 lightToFrag = normalize(light.position - FragPos);
    
    float theta = dot(lightToFrag, normalize(-light.direction));
    float epsilon = (spotlightBoundary - spotlightBoundaryEnd);
    float intensity = clamp((theta - spotlightBoundaryEnd) / epsilon, 0.0, 1.0);
    
    // Ambient                              trucating the texture (vec4) to a vec3 (rgb)
    vec3 ambientComponent = light.ambient * texture(material.diffuseMap, TexCoord).rgb;
    
    // Diffuse
    vec3 surfaceNormal = normalize(Normal);
    float angleOfIncidence = max(dot(surfaceNormal, lightToFrag), 0.0);
    vec3 diffuseComponent = angleOfIncidence * light.diffuse * texture(material.diffuseMap, TexCoord).rgb;
    
    // Specular
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectionDirection = reflect(-lightToFrag, surfaceNormal);
    float angleOfReflection = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specularComponent = texture(material.specularMap, TexCoord).rgb * angleOfReflection * light.specular;
    
    float pLightDistance = length(light.position - FragPos);
    float attenuation = 1.0 / (constant + linear * pLightDistance + quadratic * (pLightDistance * pLightDistance));
    
    diffuseComponent *= intensity;
    specularComponent *= intensity;
    
    ambientComponent *= attenuation;
    diffuseComponent *= attenuation;
    specularComponent *= attenuation;
    
    FragColor = vec4(ambientComponent + diffuseComponent + specularComponent, 1.0);
}