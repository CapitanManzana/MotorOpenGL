#version 330 core

in vec3 WorldPos;
out vec4 FragColor;

uniform vec3 cameraPos; // Pásale la posición de la cámara desde C++

void main() {
    // 1. Tamaño de cada celda y grosor de las líneas
    float smallGridSize = 1.0;
    float bigGridSize = 10.0;

    float yStartFade = 20.0;
    float yEndFade = 30.0;
    float thickness = 0.01; // Grosor de las líneas (0.03 = 3% de la celda)
    float scaleGrid = 10.0; // Por cuanto se multiplica
    
    // Coordenadas locales de la celda actual (van de 0.0 a 1.0)
    vec2 smallCoord = fract(WorldPos.xz / smallGridSize);
    vec2 bigCoord = fract(WorldPos.xz / bigGridSize);

    float smallLineAlpha = 1 - smoothstep(yStartFade, yEndFade, cameraPos.y) + 0.1;
    float slineX = (smallCoord.x < thickness || smallCoord.x > 1.0 - thickness) ? smallLineAlpha : 0.0;
    float slineZ = (smallCoord.y < thickness || smallCoord.y > 1.0 - thickness) ? smallLineAlpha : 0.0;
    
    float bigLineAlpha = smoothstep(yStartFade, yEndFade, cameraPos.y) + 0.1;
    float blineX = (bigCoord.x < thickness || bigCoord.x > 1.0 - thickness) ? bigLineAlpha : 0.0;
    float blineZ = (bigCoord.y < thickness || bigCoord.y > 1.0 - thickness) ? bigLineAlpha : 0.0;

    float isSmallLine = max(slineX, slineZ);
    float isBigLine = max(blineX, blineZ);
    
    // 3. Color base de la cuadrícula
    vec4 smallGridColor = vec4(0.8, 0.8, 0.8, isSmallLine); 
    vec4 bigGridColor = vec4(0.8, 0.8, 0.8, isBigLine); 

    // 4. Efecto Fade (Obligatorio para que no se vea feo a lo lejos)
    float d = distance(cameraPos, WorldPos);
    // Smoothstep difumina suavemente. A 10.0 de distancia empieza a borrarse, a 60.0 desaparece.
    // (Ajusta estos valores según la escala de tu motor)
    float fade = 1.0 - smoothstep(50.0, 110.0, d);

    FragColor = vec4(smallGridColor.rgb + bigGridColor.rgb, (smallGridColor.a + bigGridColor.a) * fade);
    
    // Descartamos los píxeles vacíos para optimizar a tope
    if(FragColor.a < 0.01) {
        discard; 
    }
}