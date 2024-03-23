#include "include.h"
#include "shader.hpp"

#define INITIAL_WIDTH 920
#define INITIAL_HEIGHT 480

void SetResolutionniform(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    glUniform2f(glGetUniformLocation(reinterpret_cast<uint32_t>(glfwGetWindowUserPointer(window)), "resolution"), width, height);

    cout << "resolution set to: " << width << " x " << height << endl;
}

void SetMouseUniform(GLFWwindow* window)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    int height;
    glfwGetWindowSize(window, nullptr, &height);

    glUniform2f(glGetUniformLocation(reinterpret_cast<uint32_t>(glfwGetWindowUserPointer(window)), "mouse"), x, height - y);
}

int main()
{
    GLFWwindow* window;
    if (!glfwInit())
    {
        cout << "error to glfw" << endl;

        system("pause");

        glfwTerminate();
        return 0;
    }

    window = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "previewer", NULL, NULL);
    if (!window)
    {
        cout << "error to create window" << endl;

        system("pause");

        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "error to glad" << endl;

        system("pause");

        glfwTerminate();
        return 0;
    }

    float vertices[] = { -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0 };

    GLuint vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    string vertex = ReadFile("vertex.glsl");
    string fragment = ReadFile("fragment.glsl");

    uint32_t shader = CreateShader(vertex, fragment);
    glUseProgram(shader);

    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(shader));
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { SetResolutionniform(window, width, height); });
    glUniform2f(glGetUniformLocation(shader, "resolution"), INITIAL_WIDTH, INITIAL_HEIGHT);

    cout << "render " << glGetString(GL_RENDERER) << endl;
    cout << "vendor " << glGetString(GL_VENDOR) << endl;
    cout << "opengl version " << glGetString(GL_VERSION) << endl;
    cout << "glsl version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    while (!glfwWindowShouldClose(window))
    {
        SetMouseUniform(window);
        glUniform1f(glGetUniformLocation(shader, "time"), glfwGetTime());

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    system("pause");

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}