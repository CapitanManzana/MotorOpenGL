#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 vTexCoords;

struct Material {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;
};

uniform Material  material;
uniform bool      useAlbedoTex;
uniform vec4      albedo;           // EL color si no se usa una textura
uniform sampler2D albedoMap;        // La textura si se usa

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientStrength;
uniform vec3 cameraPos;

void main() {
    vec4 baseColor = useAlbedoTex ? texture(albedoMap, vTexCoords) : albedo;

    // AMBIENT
    vec3 ambient = material.ambient * lightColor;

    // DIFUSE
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * lightColor;

    // SPECULAR
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = material.specular * spec * lightColor;

    vec3 result =  (ambient + diffuse + specular) * baseColor.rgb;

    FragColor = vec4(result, 1.0);
}