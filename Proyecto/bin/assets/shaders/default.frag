#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 vTexCoords;

// -------------------------------------------------------
// Constante: debe coincidir con LightManager::MAX_POINT_LIGHTS
// -------------------------------------------------------
#define MAX_POINT_LIGHTS 8

// -------------------------------------------------------
// Structs
// -------------------------------------------------------
struct Material {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;
};

struct GlobalLight {        // Luz direccional unica (sol)
    vec3  color;
    vec3  direction;
    float intensity;
};

struct PointLight {         // Luces puntuales
    vec3  position;
    vec3  color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

// -------------------------------------------------------
// Uniforms
// -------------------------------------------------------
uniform GlobalLight globalLight;

uniform PointLight  pointLights[MAX_POINT_LIGHTS];
uniform int         numPointLights;

uniform Material    material;
uniform vec4        albedo;
uniform sampler2D   albedoMap;
uniform bool        useAlbedoTex;
uniform vec3        cameraPos;

// -------------------------------------------------------
// Funciones de calculo de luz
// -------------------------------------------------------

vec3 calcGlobalLight(GlobalLight light, vec3 norm, vec3 viewDir, vec3 baseColor) {
    vec3 lightDir = normalize(-light.direction);

    // Ambient
    vec3 ambient = material.ambient * light.color;

    // Diffuse
    float diff   = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * light.color;

    // Specular
    vec3  reflectDir = reflect(-lightDir, norm);
    float spec       = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3  specular   = material.specular * spec * light.color * step(0.0001, diff);

    return (ambient + diffuse + specular) * light.intensity * baseColor;
}

vec3 calcPointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 baseColor) {
    vec3  toLight     = light.position - FragPos;
    vec3  lightDir    = normalize(toLight);
    float dist        = length(toLight);
    float attenuation = 1.0 / (light.constant
                              + light.linear    * dist
                              + light.quadratic * dist * dist);

    // Diffuse
    float diff   = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = diff * material.diffuse * light.color;

    // Specular
    vec3  reflectDir = reflect(-lightDir, norm);
    float spec       = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3  specular   = material.specular * spec * light.color * step(0.0001, diff);

    // No sumamos ambient por punto: el global ya la aporta
    return (diffuse + specular) * light.intensity * attenuation * baseColor;
}

// -------------------------------------------------------
// Main
// -------------------------------------------------------
void main() {
    vec4 baseColor = useAlbedoTex ? texture(albedoMap, vTexCoords) : albedo;
    vec3 norm      = normalize(Normal);
    vec3 viewDir   = normalize(cameraPos - FragPos);

    // Luz direccional global
    vec3 result = calcGlobalLight(globalLight, norm, viewDir, baseColor.rgb);

    // Luces puntuales
    int n = min(numPointLights, MAX_POINT_LIGHTS);
    for (int i = 0; i < n; ++i) {
        result += calcPointLight(pointLights[i], norm, viewDir, baseColor.rgb);
    }

    FragColor = vec4(result, 1.0);
}
