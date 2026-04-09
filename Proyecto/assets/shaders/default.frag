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

struct GlobalLight {
    vec3 color;
    vec3 direction;
    float intensity;
};

uniform GlobalLight globalLight;

uniform Material  material;
uniform vec4      albedo;           // EL color si no se usa una textura
uniform sampler2D albedoMap;        // La textura si se usa

uniform bool useAlbedoTex;

uniform vec3 cameraPos;

void main() {
    vec4 baseColor = useAlbedoTex ? texture(albedoMap, vTexCoords) : albedo;

    vec3 result;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 lightDir = normalize(-globalLight.direction);
    vec3 norm     = normalize(Normal);

    // AMBIENT
    vec3 ambient = material.ambient * globalLight.color;

    // DIFFUSE
    //vec3 lightDir = normalize(lightPos - FragPos);
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = diff * material.diffuse * globalLight.color;

    // SPECULAR
    //vec3 viewDir    = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular   = material.specular * spec * globalLight.color * step(0.0001, diff);

    result = (ambient + diffuse + specular) * globalLight.intensity * baseColor.rgb;

    FragColor = vec4(result, 1.0);
}