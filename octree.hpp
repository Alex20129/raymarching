#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <vector>
#include "basicObjects.hpp"

struct OctreeNode
{
	uint32_t branch[8]={0, 0, 0, 0, 0, 0, 0, 0};
	uint32_t index=0;
	Vec3f center;
	float halfSize=0.0;
	const Object *objects[4]={nullptr, nullptr, nullptr, nullptr};
};

class Octree
{
	vector <OctreeNode> pNodes;
	struct DistancedObject
	{
		const Object *object;
		float distance;
	};
	static bool ObjectDistanceComparator(const DistancedObject &a, const DistancedObject &b)
	{
		return(a.distance < b.distance);
	};
	int SortObjectsByDistance(uint32_t node_index, vector <const Object *> *objects, vector<DistancedObject> &objects_by_disance);
	void SplitNode(uint32_t node_index, vector <const Object *> *objects);
public:
	Octree();
	void Clear();
	void Build(vector<const Object *> *objects);
	const OctreeNode *GetClosestLeafNode(Vec3f point) const;
};

#endif // OCTREE_HPP
