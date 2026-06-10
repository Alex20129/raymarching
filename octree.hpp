#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <vector>
#include "basicObjects.hpp"

struct OctreeNode
{
	static constexpr uint32_t OBJECTS_PER_NODE=4u;
	uint32_t branch[8]={0, 0, 0, 0, 0, 0, 0, 0};
	uint32_t index=0;
	Vec3f center;
	float halfSize=0.0;
	const Object *objects[OBJECTS_PER_NODE]={nullptr, nullptr, nullptr, nullptr};
	bool Contains(const Vec3f &point) const;
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
	const OctreeNode *GetClosestLeafNode(const Vec3f &point) const;
};

#endif // OCTREE_HPP
