#include "clothsystem.h"
#include "camera.h"
#include "vertexrecorder.h"
#include <iostream>

 // your system should at least contain 8x8 particles.
const int W = 8;
const int H = 8;

ClothSystem::ClothSystem()
{
    // TODO 5. Initialize m_vVecState with cloth particles. 
    // You can again use rand_uniform(lo, hi) to make things a bit more interesting
    m_g = -2.0f;
    drag = .4f;
    Vector3f clothTopLeft = Vector3f(0, 0, 0);
    float d = 0.2f;
    for(int i = 0; i < H; i++) {
	for(int j = 0; j < W; j++) {
	    particles.push_back(clothTopLeft + j * d * Vector3f(1, 0, 0) - i * d * Vector3f(0, 1, 0));
	}
    }
    for(int i = 0; i < particles.size(); i++) {
	m_vVecState.push_back(particles[i]);
	m_vVecState.push_back(Vector3f(0, 0, 0));
    }
    std::vector<float> lengthK;
    lengthK.push_back(d);
    lengthK.push_back(25.0f);
    // structural springs
    for(int i = 0; i < H - 1; i++) {
	for(int j = 0; j < W - 1; j++) {
	    std::vector<int> points;
	    points.push_back(indexOf(i, j));
	    points.push_back(indexOf(i, j + 1));
	    structSprings.push_back(points);
	    structSpringsLengthK.push_back(lengthK);
	    std::vector<int> nextPoints;
	    nextPoints.push_back(indexOf(i, j));
	    nextPoints.push_back(indexOf(i + 1, j));
	    structSprings.push_back(nextPoints);
	    structSpringsLengthK.push_back(lengthK);
	}
    }
    for(int i = 0; i < H - 1; i++) {
	std::vector<int> points;
	points.push_back(indexOf(i, W - 1));
	points.push_back(indexOf(i + 1, W - 1));
	structSprings.push_back(points);
	structSpringsLengthK.push_back(lengthK);
    }

    for(int i =0; i < W - 1; i++) {
	std::vector<int> points;
	points.push_back(indexOf(H - 1, i));
	points.push_back(indexOf(H - 1, i + 1));
	structSprings.push_back(points);
	structSpringsLengthK.push_back(lengthK);
    }

    // shear springs
    std::vector<float> shearLength;
    shearLength.push_back(d * 1.5);
    shearLength.push_back(25.0f);
    for(int i = 0; i < H; i++) {
	for(int j = 0; j < W; j++) {
	    if(i + 1 < H && j + 1 < W) {
		std::vector<int> points;
		points.push_back(indexOf(i, j));
		points.push_back(indexOf(i + 1, j + 1));
		shearSprings.push_back(points);
		shearSpringsLengthK.push_back(shearLength);
	    }
	    if(i - 1 >= 0 && j + 1 < W) {
		std::vector<int> newPoints;
		newPoints.push_back(indexOf(i, j));
		newPoints.push_back(indexOf(i - 1, j + 1));
		shearSprings.push_back(newPoints);
		shearSpringsLengthK.push_back(shearLength);
	    }
	}
    }

    // flex springs
    std::vector<float> flexLength;
    flexLength.push_back(d * 2.0f);
    flexLength.push_back(25.0f);
    for(int i = 0; i < H; i++){
	for(int j = 0; j < W; j++) {
	    if(i + 2 < H) {
		std::vector<int> points;
		points.push_back(indexOf(i, j));
		points.push_back(indexOf(i + 2, j));
		flexSprings.push_back(points);
                flexSpringsLengthK.push_back(flexLength);
	    }
	    if(j + 2 < W) {
		std::vector<int> nextPoints;
		nextPoints.push_back(indexOf(i, j));
		nextPoints.push_back(indexOf(i, j + 2));
		flexSprings.push_back(nextPoints);
                flexSpringsLengthK.push_back(flexLength);
	    }
	}
    }
}

int ClothSystem::indexOf(int i, int j) {
    return i * W + j;
}

std::vector<Vector3f> ClothSystem::evalF(std::vector<Vector3f> state)
{
    std::vector<Vector3f> f(state.size());
    // TODO 5. implement evalF
    // - gravity
    // - viscous drag
    // - structural springs
    // - shear springs
    // - flexion springs
    for(int i = 0; i < particles.size(); i++) {
        f[2 * i] = state[2 * i + 1];
        Vector3f result = state[2 * i + 1];
        Vector3f force = Vector3f(0, m_g, 0) - drag * state[2 * i + 1];
        f[2 * i + 1] = force;
    }

    // structural springs
    for(int i = 0; i < structSprings.size(); i++) {
        Vector3f p0 = state[2 * structSprings[i][0]];
        Vector3f p1 = state[2 * structSprings[i][1]];

        float restLength = structSpringsLengthK[i][0];
	float K = structSpringsLengthK[i][1];
	Vector3f d = p0 - p1;
        Vector3f springForce = -1 * K * (d.abs() - restLength)*(d/d.abs());
        f[2 * structSprings[i][0] + 1] += springForce;
        f[2 * structSprings[i][1] + 1] -= springForce;
    }

    // shear springs
    for(int i = 0; i < shearSprings.size(); i++) {
	Vector3f p0 = state[2 * shearSprings[i][0]];
	Vector3f p1 = state[2 * shearSprings[i][1]];

        float restLength = shearSpringsLengthK[i][0];
        float K = shearSpringsLengthK[i][1];
        Vector3f d = p0 - p1;
        Vector3f springForce = -1 * K * (d.abs() - restLength)*(d/d.abs());
        f[2 * shearSprings[i][0] + 1] += springForce;
        f[2 * shearSprings[i][1] + 1] -= springForce;
    }
    // flex springs
    for(int i = 0; i < flexSprings.size(); i++) {
	Vector3f p0 = state[2 * flexSprings[i][0]];
	Vector3f p1 = state[2 * flexSprings[i][1]];
	float restLength = flexSpringsLengthK[i][0];
        float K = flexSpringsLengthK[i][1];
        Vector3f d = p0 - p1;
        Vector3f springForce = -1 * K * (d.abs() - restLength)*(d/d.abs());
        f[2 * flexSprings[i][0] + 1] += springForce;
        f[2 * flexSprings[i][1] + 1] -= springForce;
    }

    f[1] = Vector3f(0, 0, 0);
    f[2 * (W - 1) + 1] = Vector3f(0, 0, 0);
    return f;
}


void ClothSystem::draw(GLProgram& gl)
{
    //TODO 5: render the system 
    //         - ie draw the particles as little spheres
    //         - or draw the springs as little lines or cylinders
    //         - or draw wireframe mesh

    const Vector3f CLOTH_COLOR(0.9f, 0.9f, 0.9f);
    gl.updateMaterial(CLOTH_COLOR);

    // EXAMPLE for how to render cloth particles.
    //  - you should replace this code.
    for(int i = 0; i < particles.size(); i++) {
	gl.updateModelMatrix(Matrix4f::translation(getState()[2 * i]));
        drawSphere(0.075f, 10, 10);
    }
    // EXAMPLE: This shows you how to render lines to debug the spring system.
    //
    //          You should replace this code.
    //
    //          Since lines don't have a clearly defined normal, we can't use
    //          a regular lighting model.
    //          GLprogram has a "color only" mode, where illumination
    //          is disabled, and you specify color directly as vertex attribute.
    //          Note: enableLighting/disableLighting invalidates uniforms,
    //          so you'll have to update the transformation/material parameters
    //          after a mode change.

    gl.disableLighting();                                                                    
    gl.updateModelMatrix(Matrix4f::identity()); // update uniforms after mode change
    VertexRecorder rec;
    for(int i = 0; i < structSprings.size(); i++) {
	Vector3f s1 = m_vVecState[2 * structSprings[i][0]];
	Vector3f s2 = m_vVecState[2 * structSprings[i][1]];
	rec.record(s1, CLOTH_COLOR);
	rec.record(s2, CLOTH_COLOR);
    }

    for(int i = 0; i < shearSprings.size(); i++) {
	Vector3f s1 = m_vVecState[2 * shearSprings[i][0]];
        Vector3f s2 = m_vVecState[2 * shearSprings[i][1]];
        rec.record(s1, CLOTH_COLOR);
        rec.record(s2, CLOTH_COLOR);
    }
    glLineWidth(3.0f);
    rec.draw(GL_LINES);                                  
    gl.enableLighting(); // reset to default lighting model  
    // EXAMPLE END
}

