#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vector>

#include "particlesystem.h"

class ClothSystem : public ParticleSystem
{
    ///ADD MORE FUNCTION AND FIELDS HERE
public:
    ClothSystem();

    // evalF is called by the integrator at least once per time step
    std::vector<Vector3f> evalF(std::vector<Vector3f> state) override;

    int indexOf(int i,int j);

    // draw is called once per frame
    void draw(GLProgram& ctx);
    
    // inherits
    // std::vector<Vector3f> m_vVecState;
 protected:
    float m_g;
    float drag;
    std::vector<Vector3f> particles;
    
    std::vector<std::vector<int>> structSprings;
    std::vector<std::vector<float>> structSpringsLengthK;

    std::vector<std::vector<int>> shearSprings;
    std::vector<std::vector<float>> shearSpringsLengthK;

    std::vector<std::vector<int>> flexSprings;
    std::vector<std::vector<float>> flexSpringsLengthK;
};


#endif
