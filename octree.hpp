#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <cfloat>
#include <vector>
#include "basicObjects.hpp"

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
	uint64_t pNodesEmpty;
	std::vector<OctreeNode *> pNodes;
	void SplitNode(OctreeNode *node, vector<const Object *> *objects);
public:
	Octree();
	~Octree();
	void Clear();
	void Build(vector<const Object *> *objects);
	OctreeNode *GetClosestLeafNode(Vec3d point) const;
	OctreeNode *GetNode(uint64_t node_index) const;
};

#endif // OCTREE_HPP
