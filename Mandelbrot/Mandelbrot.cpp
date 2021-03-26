#include "Mandelbrot.hpp"
#include "Window.hpp"

#define VERTEX_SOURCE "vertex-shader.vert"
#define FRAGMENT_SOURCE "fragment-shader.frag"

namespace chrono = std::chrono;

float vertices[] =
{
    -1.0,  1.0,
     1.0,  1.0,
    -1.0, -1.0,
     1.0, -1.0
};

int elements[] =
{
    1, 2, 3,
    2, 3, 4
};

std::string ShaderSource(std::string filepath)
{
    std::ifstream file(filepath);
    if (!file)
    {
        std::cerr << "Could not open \"" << filepath << "\"";
        throw 1;
    }
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return source;
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, 
    const GLchar* message, const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

bool CompileShader(GLuint& shader, GLenum type, std::string source, std::string& log)
{
    const char* const sourceC = source.c_str();
    char* logC;
    int length[] = { source.length() };
    GLint isCompiled;
    GLint logLength;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &sourceC, length);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        logC = (char*)malloc(logLength);
        if (logC == NULL)
            throw 1;
        glGetShaderInfoLog(shader, logLength, &logLength, logC);
        log.clear();
        log.append(logC);
        free(logC);
    }

    return isCompiled;
}

int main()
{
    int64_t fpsLimit = 16666;

    std::cout << "Mandelbrot v0.0.0" << std::endl;

    sf::Window window;
    MakeWindow(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    std::cout << "vao" << std::endl;
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    std::cout << "vbo" << std::endl;
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    std::cout << "ebo" << std::endl;
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    std::cout << "15" << std::endl;
    std::string compileLog = "";
    GLuint vertexShader;
    GLuint fragmentShader;
    std::string vertexSource = ShaderSource(VERTEX_SOURCE);
    std::string fragmentSource = ShaderSource(FRAGMENT_SOURCE);

    if (!CompileShader(vertexShader, GL_VERTEX_SHADER, vertexSource, compileLog))
    {
        std::cerr << "Could not compile \"" << VERTEX_SOURCE << "\"" << std::endl;
        std::cerr << "Log:" << std::endl;
        std::cerr << compileLog << std::endl;
        throw 1;
    }
    if (!CompileShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentSource, compileLog))
    {
        std::cerr << "Could not compile \"" << FRAGMENT_SOURCE << "\"" << std::endl;
        std::cerr << "Log:" << std::endl;
        std::cerr << compileLog << std::endl;
        throw 1;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindAttribLocation(shaderProgram, 0, "coord");

    glLinkProgram(shaderProgram);

    int IsLinked;
    int logLen;
    char* shaderProgramInfoLog;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &IsLinked);
    if (IsLinked == GL_FALSE)
    {
        /* Noticed that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv. */
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLen);

        /* The maxLength includes the NULL character */
        shaderProgramInfoLog = (char*)malloc(logLen);

        /* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
        glGetProgramInfoLog(shaderProgram, logLen, &logLen, shaderProgramInfoLog);

        /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
        /* In this simple program, we'll just leave */
        std::cout << shaderProgramInfoLog << std::endl;
        free(shaderProgramInfoLog);
        return 1;
    }

    glUseProgram(shaderProgram);

    sf::Event windowEvent;
    bool running = true;

    while (running)
    {
        while (window.pollEvent(windowEvent))
        {
            switch (windowEvent.type)
            {
            case sf::Event::Closed:
                running = false;
                break;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 2, GL_UNSIGNED_INT, 0);

        window.display();
    }
}
