#pragma once

/**
 * Square.h
 *
 * A simple square model.
 * Very similar to Triangle, but renders a square using two triangles.
 */

#include "Model.h"

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

class Square : public Model
{
public:

#ifdef PLATFORM_ANDROID
    explicit Square(AAssetManager* assetMgr);
#else
    Square();
#endif

    ~Square() override;

    void InitModel() override;
    void Render() override;
    void Resize(int width, int height) override;

private:

#ifdef PLATFORM_ANDROID
    AAssetManager* mgr = nullptr;
#endif

    GLuint vao = 0;
    GLuint vboPos = 0;
    GLuint vboColor = 0;

    GLuint programID = 0;

    GLint uRadianAngle = -1;

    float degree = 0.0f;
};