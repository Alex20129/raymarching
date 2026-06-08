#include <cmath>
#include <cstdio>
#include <algorithm>
#include "octree.hpp"

int Octree::SortObjectsByDistance(uint32_t node_index, vector <const Object *> *objects, vector <DistancedObject> &objects_by_disance)
{
	if(objects->empty())
	{
		return(0);
	}
	float NodeBSphereRadius=Vec3f(pNodes[node_index].halfSize, pNodes[node_index].halfSize, pNodes[node_index].halfSize).Length();
	int ObjectsInTouch=0;
	objects_by_disance.clear();
	for(const Object *object : *objects)
	{
		if(!object->Visible())
		{
			continue;
		}
		float Distance=object->GetDistance(pNodes[node_index].center);
		objects_by_disance.push_back({object, Distance});
		if(Distance<NodeBSphereRadius)
		{
			ObjectsInTouch++;
		}
	}
	std::sort(objects_by_disance.begin(), objects_by_disance.end(), ObjectDistanceComparator);
	if(objects_by_disance.front().distance<-NodeBSphereRadius)
	{
		ObjectsInTouch=1;
	}
	return(ObjectsInTouch);
}

void Octree::SplitNode(uint32_t node_index, vector <const Object *> *objects)
{
	float SubNodeHalfSize=pNodes[node_index].halfSize/2.0;
	pNodes.reserve(pNodes.size()+9);
	for(int n=0; n<8; n++)
	{
		OctreeNode newSubNode;
		newSubNode.halfSize=SubNodeHalfSize;
		newSubNode.index=pNodes.size();
		float dx=(n & 1) ? SubNodeHalfSize : -SubNodeHalfSize;
		float dy=(n & 2) ? SubNodeHalfSize : -SubNodeHalfSize;
		float dz=(n & 4) ? SubNodeHalfSize : -SubNodeHalfSize;
		newSubNode.center=Vec3f(
			pNodes[node_index].center.X + dx,
			pNodes[node_index].center.Y + dy,
			pNodes[node_index].center.Z + dz);
		pNodes.push_back(newSubNode);
		pNodes[node_index].branch[n]=newSubNode.index;
		std::vector <DistancedObject> ObjectsOrdered;
		int ObjectsInTouch=SortObjectsByDistance(newSubNode.index, objects, ObjectsOrdered);
		for(int obj=0; obj<4; obj++)
		{
			pNodes[newSubNode.index].objects[obj]=ObjectsOrdered.at(obj).object;
		}
		if(ObjectsInTouch>1 && SubNodeHalfSize>2.0)
		{
			SplitNode(newSubNode.index, objects);
		}
	}
}

Octree::Octree()
{
	pNodes.push_back(OctreeNode());
}

void Octree::Clear()
{
	pNodes.clear();
	pNodes.push_back(OctreeNode());
}

void Octree::Build(vector<const Object *> *objects)
{
	Vec3f globalMaxPos;
	Clear();
	if(objects->empty())
	{
		return;
	}

	for(const Object *obj : *objects)
	{
		if(!obj->Visible())
		{
			continue;
		}
		Vec3f ObjPosition=obj->Position();
		globalMaxPos=Vec3f::Max(globalMaxPos, ObjPosition.Abs());
	}

	if(globalMaxPos.Length()<=0.0)
	{
		return;
	}

	pNodes[0].halfSize=globalMaxPos.Length();
	SplitNode(0, objects);

	fprintf(stderr, "%lu nodes total\n", pNodes.size());
	fprintf(stderr, "Root node size: %f\n", pNodes[0].halfSize*2.0);
}

const OctreeNode *Octree::GetClosestLeafNode(const Vec3f &point) const
{
	uint32_t NodeIndex=0, OctantIndex;
	while(pNodes[NodeIndex].branch[0])
	{
		OctantIndex=0;
		if(point.X > pNodes[NodeIndex].center.X)
		{
			OctantIndex |= 1;
		}
		if(point.Y > pNodes[NodeIndex].center.Y)
		{
			OctantIndex |= 2;
		}
		if(point.Z > pNodes[NodeIndex].center.Z)
		{
			OctantIndex |= 4;
		}
		NodeIndex=pNodes[NodeIndex].branch[OctantIndex];
	}
	return(&pNodes[NodeIndex]);
}
