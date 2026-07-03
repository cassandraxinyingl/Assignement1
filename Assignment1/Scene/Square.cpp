#define LOG_TAG "Square"

#include "Square.h"
#include "ShaderHelper.h"

#include <cmath>

// ---------------------------------------------------------------------------
// Vertex Data
// ---------------------------------------------------------------------------

static const GLfloat kPositions[] =
        {
                0.55f,  0.40f,   // Top Left
                0.55f,  0.00f,   // Bottom Left
                0.95f,  0.00f,   // Bottom Right
                0.95f,  0.40f    // Top Right
        };

static const GLfloat kColors[] =
        {
                1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 0.0f
        };

// ---------------------------------------------------------------------------
// Constructors
// ---------------------------------------------------------------------------

#ifdef PLATFORM_ANDROID

Square::Square(AAssetManager* assetMgr)
        : mgr(assetMgr)
{
}

#else

Square::Square() = default;

#endif

Square::~Square()
{
    glDeleteBuffers(1, &vboColor);
    glDeleteBuffers(1, &vboPos);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(programID);
}
// ---------------------------------------------------------------------------
// Initialize
// ---------------------------------------------------------------------------

void Square::InitModel()
{
    LOGI("Square::InitModel");

#ifdef PLATFORM_ANDROID

    programID = ShaderHelper::buildProgramFromAssets(
            mgr,
            "shader/BlueTriangleVertex.glsl",
            "shader/BlueTriangleFragment.glsl");

#else

    programID = ShaderHelper::buildProgramFromFile(
        "BlueTriangleVertex.glsl",
        "BlueTriangleFragment.glsl");

#endif

    if (!programID)
    {
        LOGE("Failed to create shader program.");
        return;
    }

    uRadianAngle = glGetUniformLocation(programID, "RadianAngle");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Position buffer
    glGenBuffers(1, &vboPos);
    glBindBuffer(GL_ARRAY_BUFFER, vboPos);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(kPositions),
                 kPositions,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr);

    // Colour buffer
    glGenBuffers(1, &vboColor);
    glBindBuffer(GL_ARRAY_BUFFER, vboColor);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(kColors),
                 kColors,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    LOGI("Square initialized successfully.");
}
// ---------------------------------------------------------------------------
// Render
// ---------------------------------------------------------------------------

void Square::Render()
{
    glUseProgram(programID);

    // Keep the square stationary
    glUniform1f(uRadianAngle, 0.0f);

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
}

// ---------------------------------------------------------------------------
// Resize
// ---------------------------------------------------------------------------

void Square::Resize(int width, int height)
{
    glViewport(0, 0, width, height);
}