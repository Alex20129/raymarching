#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <vector>
#include "basicObjects.hpp"

struct OctreeNode
{
	uint64_t index=0;
	uint64_t branch[8]={0, 0, 0, 0, 0, 0, 0, 0};
	Vec3d center;
	double halfSize=0.0;
	const Object *objects[4]={nullptr, nullptr, nullptr, nullptr};
	bool IsLeaf() const;
};

class Octree
{
	double pNodeSizeMin;
	vector <OctreeNode> pNodes;
	struct DistancedObject
	{
		const Object *object;
		double distance;
	};
	static bool ObjectDistanceComparator(const DistancedObject &a, const DistancedObject &b)
	{
		return(a.distance < b.distance);
	};
	int SortObjectsByDistance(uint64_t node_index, vector <const Object *> *objects, vector<DistancedObject> &objects_by_disance);
	void SplitNode(uint64_t node_index, vector <const Object *> *objects);
public:
	Octree();
	void Clear();
	void Build(vector<const Object *> *objects);
	const OctreeNode *GetClosestLeafNode(Vec3d point) const;
};

#endif // OCTREE_HPP
