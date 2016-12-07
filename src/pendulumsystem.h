#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vector>

#include "particlesystem.h"

class PendulumSystem : public ParticleSystem
{
public:
    PendulumSystem();

    std::vector<Vector3f> evalF(std::vector<Vector3f> state) override;
    void draw(GLProgram&);

    // inherits 
    // std::vector<Vector3f> m_vVecState;
 protected:
    float m_g;
    float drag;
    // array of springs where for each entry the spring is from point = entry[0] to point = entry[1]
    std::vector<std::vector<int>> springs;
    // array of springs where for each entry the spring has rest length = entry[0] and constant = entry[1]
    std::vector<std::vector<float>> springsLengthK;
    
    std::vector<Vector3f> particles;
    std::vector<bool> fixedParticles;
};

#endif
