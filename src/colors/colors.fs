#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct pointLight {
        vec3 position;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        float constant;
        float linear;
        float quadratic;
};
#define NO_OF_POINT_LIGHTS 1

struct globalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
in vec3 lightColor;

uniform pointLight pointLights[NO_OF_POINT_LIGHTS];
uniform vec3 viewPos;
uniform Material material;
uniform globalLight globallight;

vec3 calcGlobalLight(globalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  

void main()
{
// properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result;
    // phase 1: Directional lighting
    result = calcGlobalLight(globallight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < NO_OF_POINT_LIGHTS; i++)
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // phase 3: Spot light
    //result += calcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
} 

vec3 calcGlobalLight(globalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
} 

vec3 calcPointLight(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 
