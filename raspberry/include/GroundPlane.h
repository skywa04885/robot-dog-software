/*
  Copyright Luke A.C.A. Rieff 2020 - All Rights Reserved
*/

#pragma once

#include <vector>
#include <cstdint>

#include <glm/glm.hpp>

class GroundPlane {
public:
    GroundPlane(size_t w, size_t h, float s) noexcept;

    void draw();
private:
    std::vector<glm::vec3> m_Vector;
};