#include "pendulumsystem.h"

#include <cassert>
#include "camera.h"
#include "vertexrecorder.h"
#include <iostream>
// TODO adjust to number of particles.
const int NUM_PARTICLES = 4;

PendulumSystem::PendulumSystem()
{

    // TODO 4.2 Add particles for simple pendulum

    // TODO 4.3 Extend to multiple particles

    // To add a bit of randomness, use e.g.
    // float f = rand_uniform(-0.5f, 0.5f);
    // in your initial conditions.
    //float f = rand_uniform(-0.5f, 0.5f);
    m_g = -2.0f;
    drag = 1.0f;
    for(int i = 0; i < NUM_PARTICLES - 1; i++) {
	std::vector<int> points;
	points.push_back(i);
	points.push_back(i + 1);
	springs.push_back(points);
	std::vector<float> lengthK;
	lengthK.push_back(1.0f);
	lengthK.push_back(1000.0f);
	springsLengthK.push_back(lengthK);
	fixedParticles.push_back(false);
    }
    fixedParticles.push_back(true);
    std::cout << "fixed part " << fixedParticles.size() << std::endl;
    for(int i = 0; i < NUM_PARTICLES; i++) {
	float f = rand_uniform(-0.5f, 0.5f); 
	Vector3f v = Vector3f(1.0 * f, -1.0 * f, 2.0 * f);
	particles.push_back(v);
    }
    /*points.push_back(0);
    points.push_back(1);
    springs.push_back(points);
    std::vector<float> lengthK;
    lengthK.push_back(1.0f);
    lengthK.push_back(20.0f);
    springsLengthK.push_back(lengthK);
    particles.push_back(Vector3f(-1, 0, 0));
    particles.push_back(Vector3f(-2, 0, .5));
    fixedParticles.push_back(false);
    fixedParticles.push_back(true);*/
    for(int i = 0; i < particles.size(); i++) {
	m_vVecState.push_back(particles[i]);
	m_vVecState.push_back(Vector3f(0, 0, 0));
    }
}


std::vector<Vector3f> PendulumSystem::evalF(std::vector<Vector3f> state)
{
    std::vector<Vector3f> f(state.size());
    // TODO 4.1: implement evalF
    //  - gravity
    //  - viscous drag
    //  - springs
    for(int i = 0; i < particles.size(); i++) {
	f[2 * i] = state[2 * i + 1];
	Vector3f result = state[2 * i + 1];
	Vector3f force = Vector3f(0, m_g, 0) - drag * state[2 * i + 1];
	f[2 * i + 1] = force;
    }
    for(int i = 0; i < springs.size(); i++) {
	Vector3f p0 = state[2 * springs[i][0]];
	Vector3f p1 = state[2 * springs[i][1]];

	float restLength = springsLengthK[i][0];
	float K = springsLengthK[i][1];
	Vector3f d = p0 - p1;
	Vector3f springForce = -1 * K * (d.abs() - restLength)*(d/d.abs());
	f[2 * springs[i][0] + 1] += springForce;
	f[2 * springs[i][1] + 1] -= springForce;
    }

    for(int i = 0; i < fixedParticles.size(); i++) {
	if(fixedParticles[i]) {
	    f[2 * i + 1] = Vector3f(0, 0, 0);
	}
    }
    return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw(GLProgram& gl)
{
    const Vector3f PENDULUM_COLOR(0.73f, 0.0f, 0.83f);
    gl.updateMaterial(PENDULUM_COLOR);

    // TODO 4.2, 4.3

    // example code. Replace with your own drawing  code
    //gl.updateModelMatrix(Matrix4f::translation(Vector3f(-0.5, 1.0, 0)));
    //drawSphere(0.075f, 10, 10);
    for(int i = 0; i < particles.size(); i++) {
	gl.updateModelMatrix(Matrix4f::translation(getState()[2 * i]));
	drawSphere(0.075f, 10, 10);
    }
}
