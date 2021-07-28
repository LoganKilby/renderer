#version 330 core

struct Material
{
    sampler2D diffuseMap;
    sampler2D specularMap;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
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

void main()
{
    // Ambient                                     trucating the texture (vec4) to a vec3
    vec3 ambientComponent = light.ambient * texture(material.diffuseMap, TexCoord).rgb;
    
    // Diffuse
    vec3 surfaceNormal = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragPos);
    //vec3 lightDirection = normalize(-dirLightDirection); (Directional Light)
    float angleOfIncidence = max(dot(surfaceNormal, lightDirection), 0.0);
    vec3 diffuseComponent = angleOfIncidence * light.diffuse * texture(material.diffuseMap, TexCoord).rgb;
    
    // Specular
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectionDirection = reflect(-lightDirection, surfaceNormal);
    float angleOfReflection = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specularComponent = texture(material.specularMap, TexCoord).rgb * angleOfReflection * light.specular;
    
    float pLightDistance = length(light.position - FragPos);
    float attenuation = 1.0 / (constant + (linear * pLightDistance) + (quadratic * (pLightDistance * pLightDistance)));
    
    ambientComponent *= attenuation;
    diffuseComponent *= attenuation;
    specularComponent *= attenuation;
    
    FragColor = vec4(ambientComponent + diffuseComponent + specularComponent, 1.0);
}