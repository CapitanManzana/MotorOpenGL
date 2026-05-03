#include "lighting/GlobalLight.h"
#include <serialize/JsonSerializer.h>

namespace cme {
    void GlobalLight::serialize(JsonSerializer& s) const {
        s.write("lightColor", _color);
        s.write("lightDir", _dir);
        s.write("lightIntensity", _intensity);
    }

    void GlobalLight::deserialize(JsonSerializer& s) {
        _color = s.readVec3("lightColor");
        _dir = s.readVec3("lightDir");
        _intensity = s.readFloat("lightIntensity");

        _elevation = asin(_dir.y);
        _azimuth = asin(_dir.x / cos(_elevation));
    }
}