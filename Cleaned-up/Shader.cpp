#include "Shader.h"

Shader::Shader() 
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);
    const char* vertexCode = vertexString.c_str(); // convert it to c string (const char array)
    const char* fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode, fragmentCode);
}


std::string Shader::ReadFile(const char* fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open())
    {
        printf("Failed to read %s! File doesn't exist.", fileLocation);
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) // if not end of the file
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();

    return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    shaderID = glCreateProgram(); // generate shader program id

    if (!shaderID) {
        printf("Error creating shader program\n");
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    // Error check
    GLint result = 0; // result of the two functions we're about to perform
    GLchar eLog[1024] = { 0 }; // a place to log the error

    glLinkProgram(shaderID); // Create the executables on the graphics card
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // get information

    if (!result) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog); // get the info log for the program, pass it into the log char array eLog
        printf("Error Linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shaderID); // checks if the shader which we've now created is valid in the current context that OpenGL is working in
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result); // get information

    if (!result) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog); // get the info log for the program, pass it into the log char array eLog
        printf("Error Validating program: '%s'\n", eLog);
        return;
    }

    //uniformXMove = glGetUniformLocation(shader, "xMove"); //first one is the shader program
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
}

GLuint Shader::GetProjectionLocation()
{
    return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
    return uniformModel;
}


void Shader::UseShader()
{
    if (shaderID == 0)
    {
        printf("Error in UseShader, ShaderID is 0!");
        return;
    }
    glUseProgram(shaderID);
}

void Shader::ClearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}


void Shader::AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType); // Create an empty shader of type

    const GLchar* theCode[1]; // where we store the code itself
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode); // length of the actual code

    glShaderSource(theShader, 1, theCode, codeLength); // the second parameter is count. You can have multiple strings. We could have lots of chunks of code and then combine them all into this one mass load of source for the code, here though, we've only got one file we're passing in.
    glCompileShader(theShader);

    // Error check
    GLint result = 0;
    GLchar eLog[1024] = { 0 }; // a place to log the error

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result); // get information

    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog); // get the info log for the program, pass it into the log char array eLog
        printf("Error Compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(program, theShader);
}


Shader::~Shader()
{
    ClearShader();
}
