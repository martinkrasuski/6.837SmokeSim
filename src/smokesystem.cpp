#include "smokesystem.h"

#include <cassert>
#include "camera.h"
#include "vertexrecorder.h"
#include <iostream>

SmokeSystem::SmokeSystem() {
    n = 10;
    stepSize = .1;
}

void SmokeSystem::swapVelocity() {
    std::vector<Vector3f> temp = velocity;
    velocity = oldVelocity;
    oldVelocity = temp;
}

void SmokeSystem::swapDensity() {
    std::vector<Vector3f> temp = density;
    density = oldDensity;
    oldDensity = temp;
}

int SmokeSystem::index(int i, int j) {
    return i + (n *j);
}


void SmokeSystem::draw(GLProgram& gl)
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
