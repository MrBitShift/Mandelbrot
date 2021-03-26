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
    0, 1, 2,
    1, 2, 3
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
    sf::Window window;
    MakeWindow(window);

    glewExperimental = true;
    glewInit();

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);

    std::cout << "Mandelbrot v0.1.0" << std::endl;
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    GLuint vertexShader;
    GLuint fragmentShader;
    std::string vertexSource = ShaderSource(VERTEX_SOURCE);
    std::string fragmentSource = ShaderSource(FRAGMENT_SOURCE);
    std::string compileLog;

    if (!CompileShader(vertexShader, GL_VERTEX_SHADER, vertexSource, compileLog))
    {
        std::cout << "Could not compile \"" VERTEX_SOURCE "\"" << std::endl;
        std::cout << "Log:" << std::endl;
        std::cout << compileLog;
        exit(1);
    }
    compileLog.clear();

    if (!CompileShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentSource, compileLog))
    {
        std::cout << "Could not compile \"" FRAGMENT_SOURCE "\"" << std::endl;
        std::cout << "Log:" << std::endl;
        std::cout << compileLog;
        exit(1);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint coordAttrib = glGetAttribLocation(shaderProgram, "coord");
    glEnableVertexAttribArray(coordAttrib);
    glVertexAttribPointer(coordAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    bool running = true;
    sf::Event windowEvent;
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

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.display();
    }
}