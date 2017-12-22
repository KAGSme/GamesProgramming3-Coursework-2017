#ifndef _MODEL_H
#define _MODEL_H

#include "Common.h"
#include "Vertex.h"
#include <fbxsdk.h>

struct Sphere
{
	vec3 pos;
	float rad;

	Sphere Transform(const mat4& matrix)
	{
		Sphere ret;
		ret.pos = vec3(matrix * vec4(pos, 1));
		ret.rad = rad * glm::max(matrix[0][0], glm::max(matrix[1][1], matrix[2][2]));
		return ret;
	}
};

class Model
{
private:
	GLuint vao, vbo, ebo;
	vector<Vertex> *vertices;
	vector<int> *indices;

	bool usesBoundSphere = false;
	Sphere boundSphere = { vec3(0, 0, 0), 0 };

	void GenerateBoundSphere();

	GLuint vertUsageFlag, indUsageFlag;

	bool loadFBXFromFile(const string& fileName);
	void processNode(FbxNode *node, int level);
	void processAttrib(FbxNodeAttribute *attrib, int level);
	void processMesh(FbxMesh *mesh, int level);
	void processMeshTextCoords(FbxMesh *mesh, Vertex *verts, int numVerts);
	void processMeshNormals(FbxMesh *mesh, Vertex *verts, int count);
	//vec3 processMeshTangent(FbxMesh *mesh, int vertsIndex);
	//vec3 processMeshBinormal(FbxMesh *mesh, int vertsIndex);
	vec3 getTangent(vec3 normal);
	vec3 getBinormal(vec3 normal, vec3 tangent);
public:
	//creates a generic model - requires attrib bindinding!
	Model();
	//creates a model from file - attribs already bound
	Model(const string& fileName);
	~Model();

	//calculate the per-vertex normals
	void Normalize();

	GLuint Get() { return vao; }
	int GetVertCount() { return vertices->size(); }
	int GetIndCount() { return indices->size(); }

	void SetBoundSphereUse(bool flag) { usesBoundSphere = flag; }
	bool UsesBoundSphereTest() { return usesBoundSphere; }
	Sphere GetBoundingSphere(const mat4& transform) { return boundSphere.Transform(transform); }
	vec3 GetCenter() { return boundSphere.pos; }

	//takes ownership of verts! releases memory on it's own! call FlushBuffers afterwards!
	void SetVertices(vector<Vertex> *verts, GLuint flag, bool deletePrev);
	//takes ownership of ints! releases memory on it's own! call FlushBuffers afterwards!
	void SetIndices(vector<int> *indcs, GLuint flag, bool deletePrev);
	//call after vertices and indices are set
	void FlushBuffers();

	void SetUpAttrib(int index, int count, int type, size_t offset);
	//instancing related - divisor = 0 means grab new data after each vertex,
	//divisor = N means grab new data after N instances processed.
	void SetAttribDivisor(int index, int divisor);
};

#endif