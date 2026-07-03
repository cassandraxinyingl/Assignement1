#pragma once

/**
 * ShaderHelper.h
 *
 * Helper class for compiling and linking OpenGL shaders.
 * Supports:
 *  - Building shaders from inline source strings.
 *  - Loading shader files from disk (Desktop/Web).
 *  - Loading shader files from APK assets (Android).
 */

#include "Platform.h"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

class ShaderHelper
{
public:

    //==========================================================
    // Compile a shader
    //==========================================================
    static GLuint compileShader(GLenum type, const char* source)
    {
        GLuint shader = glCreateShader(type);

        if (!shader)
        {
            LOGE("Failed to create shader.");
            return 0;
        }

        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint success = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            std::string log(length, '\0');
            glGetShaderInfoLog(shader, length, nullptr, &log[0]);

            LOGE("%s", log.c_str());

            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    //==========================================================
    // Link vertex and fragment shaders
    //==========================================================
    static GLuint linkProgram(GLuint vertex, GLuint fragment)
    {
        GLuint program = glCreateProgram();

        glAttachShader(program, vertex);
        glAttachShader(program, fragment);

        glLinkProgram(program);

        GLint success = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success)
        {
            GLint length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

            std::string log(length, '\0');
            glGetProgramInfoLog(program, length, nullptr, &log[0]);

            LOGE("%s", log.c_str());

            glDeleteProgram(program);
            return 0;
        }

        glDetachShader(program, vertex);
        glDetachShader(program, fragment);

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return program;
    }

    //==========================================================
    // Method 1:
    // Build directly from shader source strings
    //==========================================================
    static GLuint buildProgram(const char* vertexSource,
                               const char* fragmentSource)
    {
        GLuint vert = compileShader(GL_VERTEX_SHADER, vertexSource);

        if (!vert)
            return 0;

        GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        if (!frag)
        {
            glDeleteShader(vert);
            return 0;
        }

        return linkProgram(vert, frag);
    }

#ifdef PLATFORM_ANDROID

    //==========================================================
    // Load shader source from APK assets
    //==========================================================
    static std::string loadAsset(AAssetManager* mgr,
                                 const char* path)
    {
        if (!mgr)
        {
            LOGE("AssetManager is null.");
            return "";
        }

        AAsset* asset =
                AAssetManager_open(mgr,
                                   path,
                                   AASSET_MODE_BUFFER);

        if (!asset)
        {
            LOGE("Unable to open asset: %s", path);
            return "";
        }

        size_t size = AAsset_getLength(asset);

        std::string text(size, '\0');

        AAsset_read(asset, &text[0], size);

        AAsset_close(asset);

        return text;
    }

    //==========================================================
    // Build program from APK assets
    //==========================================================
    static GLuint buildProgramFromAssets(
            AAssetManager* mgr,
            const char* vertPath,
            const char* fragPath)
    {
        std::string vert = loadAsset(mgr, vertPath);
        std::string frag = loadAsset(mgr, fragPath);

        if (vert.empty() || frag.empty())
        {
            LOGE("Failed to load shader assets.");
            return 0;
        }

        return buildProgram(
                vert.c_str(),
                frag.c_str());
    }

#else

    //==========================================================
    // Load shader source from Desktop/Web file
    //==========================================================
    static std::string loadFile(const char* filename)
    {
        std::vector<std::string> paths =
        {
            std::string("assets/shader/") + filename,
            filename,
            std::string("shader/") + filename,
            std::string("assets/") + filename
        };

        for (const auto& path : paths)
        {
            std::ifstream file(path);

            if (file.is_open())
            {
                std::stringstream ss;
                ss << file.rdbuf();

                return ss.str();
            }
        }

        LOGE("Shader file not found: %s", filename);

        return "";
    }

    //==========================================================
    // Build program from Desktop/Web files
    //==========================================================
    static GLuint buildProgramFromFile(
        const char* vertFile,
        const char* fragFile)
    {
        std::string vert = loadFile(vertFile);
        std::string frag = loadFile(fragFile);

        if (vert.empty() || frag.empty())
        {
            LOGE("Failed to load shader files.");
            return 0;
        }

        return buildProgram(
            vert.c_str(),
            frag.c_str());
    }

#endif
};