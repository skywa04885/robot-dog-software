#include "Program.h"

Program::Program(uint32_t program) noexcept:
  m_Program(program)
{}

Program Program::create(const char *fragment, const char *vertex) {
  auto readShader = [](const char *shader) {
    std::ifstream file(shader);
    if (!file.is_open()) {
      throw std::runtime_error(std::string("Failed to open shader: ") + shader);
    }

    std::string str;

    file.seekg(0, std::ios::end);
    str.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(file)), 
      std::istreambuf_iterator<char>());

    file.close();

    return str;
  };

  auto compileShader = [](GLenum type, const std::string &raw) {
    uint32_t shader = glCreateShader(type);

    // Sets the source code for the shader to compile, this will
    //  be the raw GLSL shader code.
    const char *rawCStr = raw.c_str();
    glShaderSource(shader, 1, &rawCStr, NULL);

    // Compiles the shader.
    glCompileShader(shader);

    // Gets the compilation status code, if it failed we will try
    //  to retreive the error message supplied by OpenGL.
    int32_t success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      // Gets the length of the info log.
      int32_t l;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &l);

      // Allocates an error buffer of the specified size,
      //  after which we get the info log.
      std::cout << l << std::endl;
      char *error = new char[l];
      glGetShaderInfoLog(shader, l, NULL, error);

      // Creates and throws the runtime error indicating the
      //  compilation failure.
      std::string errorStr(error, l);
      delete error;
      throw std::runtime_error(std::string("Failed to compile shader: ") + errorStr);
    }

    return shader;
  };

  //
  // Compiles the shaders and creates the program.
  //

  // Reads the raw shaders from the files.
  std::string fragRaw = readShader(fragment);
  std::string vertexRaw = readShader(vertex);
  
  // Compiles the shaders.
  uint32_t fragShader = compileShader(GL_FRAGMENT_SHADER, fragRaw);
  uint32_t vertexShader = compileShader(GL_VERTEX_SHADER, vertexRaw);

  // Create the program.
  uint32_t program = glCreateProgram();
  glAttachShader(program, fragShader),
  glAttachShader(program, vertexShader);
  glLinkProgram(program);

  // Deletes the shaders, since they've been linked
  //  into a program.
  glDeleteShader(fragShader);
  glDeleteShader(vertexShader);

  // Checks if any errors occured while linking the program
  //  if so get the error log, and throw error.
  int32_t success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    // Gets the length of the info log, so we later
    //  can allocate the required amount of memory.
    int32_t l;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &l);

    // Allocates the specified amount of memory so we can
    //  retreive the info log.
    char *error = new char[l];
    glGetProgramInfoLog(program, l, NULL, error);

    // Creates and throws the error to indicate something
    //  went wrong when linking.
    std::string errorStr(error, l);
    delete error;
    throw std::runtime_error(std::string("Failed to link program: ") + errorStr);
  }

  // Returns the result program
  return Program(program);
}