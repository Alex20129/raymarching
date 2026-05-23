#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <cfloat>
#include <vector>
#include "basicObjects.hpp"

struct OctreeNode
{
	uint64_t index=0;
	uint64_t branch[8]={0, 0, 0, 0, 0, 0, 0, 0};
	Vec3d center;
	double halfSize=0.0;
	const Object *objectA=nullptr;
	const Object *objectB=nullptr;
	bool IsLeaf() const;
};

class Octree
{
	double pNodeSizeMin;
	vector <OctreeNode *> pNodes;
	int SortObjectsByDistance(const OctreeNode *node, vector <const Object *> *objects, vector <const Object *> &objects_by_disance);
	void SplitNode(OctreeNode *node, vector <const Object *> *objects);
	struct DistancedObject
	{
		const Object *object;
		double distance;
	};
	static bool ObjectDistanceComparator(const DistancedObject &a, const DistancedObject &b)
	{
		return(a.distance < b.distance);
	};
public:
	Octree();
	~Octree();
	void Clear();
	void Build(vector<const Object *> *objects);
	const OctreeNode *GetClosestLeafNode(Vec3d point) const;
};

#endif // OCTREE_HPP
