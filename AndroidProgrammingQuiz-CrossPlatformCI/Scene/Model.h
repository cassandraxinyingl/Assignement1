#pragma once

//==============================================================
// Model
// Base interface for every renderable object in the scene.
//==============================================================

#include "Platform.h"

class Model
{
public:
    Model() = default;
    virtual ~Model() = default;

    // Called once when the model is created
    virtual void InitModel() = 0;

    // Called every frame
    virtual void Render() = 0;

    // Called whenever the window/surface changes size
    virtual void Resize(int width, int height)
    {
        // Default implementation does nothing here
    }
};