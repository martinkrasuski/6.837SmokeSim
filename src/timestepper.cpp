#include "timestepper.h"

#include <cstdio>
#include <iostream>
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
   //TODO: See handout 3.1 
    std::vector<Vector3f> der = particleSystem->evalF(particleSystem->getState());
    std::vector<Vector3f> result;
    for(int i = 0; i < particleSystem->getState().size(); i++) {
	result.push_back(particleSystem->getState()[i] + stepSize * der[i]);
    }
    particleSystem->setState(result);
}

void Trapezoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
   //TODO: See handout 3.1 
    std::vector<Vector3f> f0 = particleSystem->evalF(particleSystem->getState());
    std::vector<Vector3f> f1_i;
    for(int i = 0; i < particleSystem->getState().size(); i++) {
	f1_i.push_back(particleSystem->getState()[i] + stepSize * f0[i]);
    }
    std::vector<Vector3f> f1 = particleSystem->evalF(f1_i);
    std::vector<Vector3f> X;
    for(int i = 0; i < particleSystem->getState().size(); i++) {
	X.push_back(particleSystem->getState()[i] + stepSize/2.0 * (f0[i] + f1[i]));
    }
    particleSystem->setState(X);
}


void RK4::takeStep(ParticleSystem* particleSystem, float stepSize)
{
   //TODO: See handout 4.4
    std::vector<Vector3f> k1 = particleSystem->evalF(particleSystem->getState());
    std::vector<Vector3f> k2_i;
    std::vector<Vector3f> vec = particleSystem->getState();
    for(int i = 0; i < vec.size(); i++) {
	k2_i.push_back(vec[i] + stepSize/2.0 * k1[i]);
    }
    std::vector<Vector3f> k2 = particleSystem->evalF(k2_i);
    std::vector<Vector3f> k3_i;
    for(int i = 0; i < vec.size(); i++) {
	k3_i.push_back(vec[i] + stepSize/2.0 * k2[i]);
    }
    std::vector<Vector3f> k3 = particleSystem->evalF(k3_i);
    std::vector<Vector3f> k4_i;
    for(int i = 0; i < vec.size(); i++) {
	k4_i.push_back(vec[i] + stepSize * k3[i]);
    }
    std::vector<Vector3f> k4 = particleSystem->evalF(k4_i);
    std::vector<Vector3f> xn_1;
    for(int i = 0; i < vec.size(); i++) {
	xn_1.push_back(vec[i] + stepSize/6.0 * (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]));
    }
    particleSystem->setState(xn_1);
}

