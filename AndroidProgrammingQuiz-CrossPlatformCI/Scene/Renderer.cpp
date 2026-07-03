//==============================================================
// Renderer.cpp
// Controls the lifetime and rendering of all models.
//==============================================================

#include "Renderer.h"

#include "Triangle.h"
#include "Square.h"

Renderer& Renderer::Instance()
{
    static Renderer instance;
    return instance;
}

Renderer::~Renderer()
{
    clearModels();
}

#ifdef PLATFORM_ANDROID
void Renderer::setAssetManager(AAssetManager* mgr)
{
    assetMgr = mgr;
}
#endif

//==============================================================
// Create all models
//==============================================================

void Renderer::createModels()
{
#ifdef PLATFORM_ANDROID

    models.push_back(new Triangle(assetMgr));
    models.push_back(new Square(assetMgr));

#else

    models.push_back(new Triangle());
    models.push_back(new Square());

#endif
}

//==============================================================
// Initialize all models
//==============================================================

void Renderer::initializeModels()
{
    for (Model* model : models)
    {
        if (model)
            model->InitModel();
    }
}

//==============================================================
// Delete all models
//==============================================================

void Renderer::clearModels()
{
    for (Model* model : models)
    {
        delete model;
    }

    models.clear();
}

//==============================================================
// Initialize renderer
//==============================================================

bool Renderer::initializeRenderer()
{
    createModels();
    initializeModels();

    return true;
}

//==============================================================
// Resize
//==============================================================

void Renderer::resize(int width, int height)
{
    for (Model* model : models)
    {
        if (model)
            model->Resize(width, height);
    }
}

//==============================================================
// Render
//==============================================================

void Renderer::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (Model* model : models)
    {
        if (model)
            model->Render();
    }
}