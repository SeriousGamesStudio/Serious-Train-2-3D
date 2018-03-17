#ifndef _H_MESHRENDERER_H_
#define _H_MESHRENDERER_H_
#include "Component.h"
#include "Ogre.h"
#include <string>

using namespace Ogre;
using namespace std;
class MeshRenderer_c:
	public Component
{
public:

	enum Type {
	PLANE
};

struct ConstructorInfo
{
	Type type;
	string name;
	const string resourceGM;
	Vector3 dir;
	int offset;
	Real height;
	Real width;
	int rows;
	int cols;
	bool normals;
	Vector3 textCoords;
	Vector3 algo;
};
	MeshRenderer_c(ConstructorInfo info, m_Entity* m_entity, SceneNode* scnNode);
	~MeshRenderer_c();

private:
	MeshPtr mesh_;
	
};
#endif // !_H_MESHRENDERER_H_

