#version 330 core

in vec3 WorldPos;
out vec4 FragColor;

uniform vec3 cameraPos;

// Dibuja una línea de grid con antialiasing usando fwidth
float gridLine(vec2 coord, float gridSize, float thickness) {
    vec2 wrapped = fract(coord / gridSize);
    vec2 fw = fwidth(coord / gridSize); // Derivada para antialiasing
    vec2 line = smoothstep(fw * 0.5, fw * (0.5 + thickness), min(wrapped, 1.0 - wrapped));
    return 1.0 - min(line.x, line.y);
}

void main() {
    float smallGridSize = 1.0;
    float bigGridSize   = 10.0;

    float fadeStart     = 40.0;
    float fadeEnd       = 120.0;

    // --- Líneas del grid con antialiasing ---
    float tSmall = 0.8; // Grosor relativo líneas pequeñas
    float tBig   = 1.2; // Grosor relativo líneas grandes

    float smallLine = gridLine(WorldPos.xz, smallGridSize, tSmall);
    float bigLine   = gridLine(WorldPos.xz, bigGridSize, tBig);

    // Transición suave entre grid pequeño y grande según altura cámara
    float lodBlend = smoothstep(20.0, 35.0, cameraPos.y);
    float gridAlpha = mix(smallLine, bigLine, lodBlend);
    gridAlpha = max(smallLine * (1.0 - lodBlend), bigLine);

    // --- Ejes X (rojo) y Z (azul) como Unity ---
    vec2 fw = fwidth(WorldPos.xz);
    float axisThickness = 1.5;

    float axisX = 1.0 - smoothstep(fw.y * 0.5, fw.y * axisThickness, abs(WorldPos.z));
    float axisZ = 1.0 - smoothstep(fw.x * 0.5, fw.x * axisThickness, abs(WorldPos.x));

    // --- Color base del grid ---
    vec3 gridColor = vec3(0.45, 0.45, 0.45);

    vec3 finalColor = gridColor;
    float finalAlpha = gridAlpha * 0.8;

    // Sobreescribir con color del eje si estamos en él
    if (axisZ > 0.1) {
        finalColor = mix(finalColor, vec3(0.2, 0.35, 1.0), axisZ); // Eje X → azul (como Unity)
        finalAlpha = max(finalAlpha, axisZ);
    }
    if (axisX > 0.1) {
        finalColor = mix(finalColor, vec3(1.0, 0.2, 0.25), axisX); // Eje Z → rojo (como Unity)
        finalAlpha = max(finalAlpha, axisX);
    }

    // --- Fade por distancia ---
    float dist = distance(vec2(cameraPos.x, cameraPos.z), WorldPos.xz);
    float fade = 1.0 - smoothstep(fadeStart, fadeEnd, dist);

    finalAlpha *= fade;

    FragColor = vec4(finalColor, finalAlpha);

    if (FragColor.a < 0.005) discard;
}
