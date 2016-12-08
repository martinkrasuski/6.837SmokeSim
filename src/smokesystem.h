

#ifndef SMOKESYSTEM_H
#define SMOKESYSTEM_H

#include <vector>
#include <vecmath.h>
#include <cstdint>
#include "particlesystem.h"

struct GLProgram;
class SmokeSystem
{
 public:
    SmokeSystem();
    ~SmokeSystem() {};
    
    void swapVelocity();

    void swapDensity();

    int index(int i, int j);

    void addExternalForces(float stepSize);
    
    void advect(float stepSize);
    
    void diffuse(float stepSize);
    
    void project(float stepSize);
    
    void addExternalSources(float stepSize);
    
    void convect(float stepSize);
    
    void diffuseDensity(float stepSize);

    void draw(GLProgram&);
    
 protected:
    std::vector<Vector3f> velocity;
    std::vector<Vector3f> oldVelocity;
    std::vector<Vector3f> density;
    std::vector<Vector3f> oldDensity;
    std::vector<Vector3f> curl;

    float stepSize;
    float visc = 0.0f;
    float diff = 0.0f;
    int n;
    
};

#endif
