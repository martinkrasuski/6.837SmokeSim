

#ifndef SMOKESYSTEM_H
#define SMOKESYSTEM_H

#include <vector>
#include <vecmath.h>
#include <cstdint>

struct GLProgram;
class SmokeSystem
{
 public:
    SmokeSystem();
    ~SmokeSystem() {}

    void addExternalForces(float stepSize);
    
    void advect(float stepSize);
    
    void diffuse(float stepSize);
    
    void project(flaot stepSize);
    
    void addExternalSources(float stepSize);
    
    void convect(float stepSize);
    
    void diffuseDensity(stepSize);

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

class Camera;
struct GLProgram {
    // constructor
    GLProgram(uint32_t program_light, uint32_t program_color, Camera* camera);

    // Update the model matrix. View and projection matrix
    // are read from the camera.
	void updateModelMatrix(Matrix4f M) const;

    // Update material properties.
    // - The one argument version just sets the diffuse color
    // - With 2-3 arguments, also sets specular color
	void updateMaterial(Vector3f diffuseColor, 
        Vector3f ambientColor = Vector3f(-1, -1, -1),
        Vector3f specularColor = Vector3f(0, 0, 0), 
        float shininess = 1.0f,
        float alpha = 1.0f) const;

    // Update lighting. Sets position and color of a single light source
    // in world space.
	void updateLight(Vector3f pos, Vector3f color = Vector3f(1, 1, 1)) const;

    void enableLighting();
    void disableLighting();

private:
    // member variables
    uint32_t active_program;
    uint32_t program_light;
    uint32_t program_color;
    const Camera* camera;
};
#endif
