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

//uniform sampler2D containerTexture;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // Ambient                                     trucating the texture (vec4) to a vec3
    vec3 ambientComponent = light.ambient * texture(material.diffuseMap, TexCoord).rgb;
    
    // Diffuse
    vec3 surfaceNormal = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragPos);
    float angleOfIncidence = max(dot(surfaceNormal, lightDirection), 0.0);
    vec3 diffuseComponent = angleOfIncidence * light.diffuse * texture(material.diffuseMap, TexCoord).rgb;
    
    // Specular
    // 32 corresponds to the amount of light reflected, how glossy the object is
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectionDirection = reflect(-lightDirection, surfaceNormal);
    float angleOfReflection = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specularComponent = texture(material.specularMap, TexCoord).rgb * angleOfReflection * light.specular;
    
    FragColor = vec4(ambientComponent + diffuseComponent + specularComponent, 1.0);
}