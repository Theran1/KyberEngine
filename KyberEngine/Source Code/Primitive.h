#pragma once
#include "glmath.h"
#include "Color.h"
#include "Globals.h"

#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128

enum class MeshTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_Pyramid
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Initialize();

	void Render(bool checkersTexture) const;

	uint CreateCheckerTexture();

public:
	// Vertex Buffer Object (VBO)
	uint VBO = 0;
	uint numVertices = 0;
	float* vertices = nullptr;

	// Vertex Array Object (VAO)
	//uint VAO = 0;

	// Element Buffer Object (EBO)
	uint EBO = 0;
	uint numIndices = 0;
	uint* indices = nullptr;

	// Normals
	uint normalsBuffer = 0;
	float* normals = nullptr;

	// Textures Coords
	uint textureBuffer = 0;
	float* textureCoords = nullptr;
	uint textureID = 0;

	// Checker texture
	uint checkerTextureID = 0;
};

class Primitive
{
public:

	Primitive();
	virtual ~Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;

	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);

	MeshTypes		GetType() const;

public:
	Color color;
	mat4x4 transform;
	bool axis, wire;

protected:
	MeshTypes type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube();
	Cube(float sizeX, float sizeY, float sizeZ);

	void InnerRender() const;

public:
	vec3 size;
};

// ============================================
class Sphere : public Primitive
{
public:
	Sphere();
	Sphere(float radius);

	void InnerRender() const;

public:
	float radius;
};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder();
	Cylinder(float radius, float height);

	void InnerRender() const;

public:
	float radius;
	float height;
};

// ============================================
class Line : public Primitive
{
public:
	Line();
	Line(float x, float y, float z);

	void InnerRender() const;

public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class Plane : public Primitive
{
public:
	Plane();
	Plane(float x, float y, float z, float d);

	void InnerRender() const;

public:
	vec3 normal;
	float constant;
};

class Pyramid : public Primitive
{
public:
	Pyramid();
	Pyramid(float baseX, float baseZ, float height);

	void InnerRender() const;

public:
	vec2 base;
	float height;
};