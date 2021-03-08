/*
  Copyright Luke A.C.A. Rieff 2020 - All Rights Reserved
*/

#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>

class Program {
public:
    Program(uint32_t program) noexcept;

    static Program create(const char *fragment, const char *vertex);
private:
    uint32_t m_Program;
};
