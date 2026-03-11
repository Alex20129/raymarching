#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <cfloat>
#include "basicObjects.hpp"

#define DEFAULT_SCREEN_WIDTH	2048
#define DEFAULT_SCREEN_HEIGHT	1536

struct OctreeNode
{
	uint64_t index=0;
	uint64_t parentNodeIndex=0;
	uint64_t branch[8]={0,0,0,0,0,0,0,0};
	Vec3d center;
	double halfSize=0.0;
	const Object *object=nullptr;
	bool IsLeaf() const;
};

class Octree
{
	double pNodeSizeMin;
	uint64_t pNodesWithObjects;
	std::vector<OctreeNode *> pNodes;
	void SplitNode(OctreeNode *node, vector<const Object *> *objects);
public:
	Octree();
	~Octree();
	void Clear();
	void Build(vector<const Object *> *objects);
	OctreeNode *GetClosestLeafNode(Vec3d point) const;
	OctreeNode *GetRootNode() const;
};

#endif // OCTREE_HPP
