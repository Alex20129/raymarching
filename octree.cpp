#include <cmath>
#include <cstdio>
#include <algorithm>
#include "octree.hpp"

int Octree::SortObjectsByDistance(uint64_t node_index, vector <const Object *> *objects, vector <DistancedObject> &objects_by_disance)
{
	if(objects->empty())
	{
		return(0);
	}
	double NodeBSphereRadius=Vec3d(pNodes[node_index].halfSize, pNodes[node_index].halfSize, pNodes[node_index].halfSize).Length();
	int ObjectsInTouch=0;
	objects_by_disance.clear();
	for(const Object *object : *objects)
	{
		if(!object->Visible())
		{
			continue;
		}
		double Distance=object->GetDistance(pNodes[node_index].center);
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

void Octree::SplitNode(uint64_t node_index, vector <const Object *> *objects)
{
	double SubNodeHalfSize=pNodes[node_index].halfSize/2.0;
	if(pNodeSizeMin>pNodes[node_index].halfSize)
	{
		pNodeSizeMin=pNodes[node_index].halfSize;
	}
	pNodes.reserve(pNodes.size()+9);
	for(int n=0; n<8; n++)
	{
		OctreeNode newSubNode;
		newSubNode.halfSize=SubNodeHalfSize;
		newSubNode.index=pNodes.size();

		double dx=(n & 1) ? SubNodeHalfSize : -SubNodeHalfSize;
		double dy=(n & 2) ? SubNodeHalfSize : -SubNodeHalfSize;
		double dz=(n & 4) ? SubNodeHalfSize : -SubNodeHalfSize;

		newSubNode.center=Vec3d(
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
	pNodeSizeMin=0.0;
	pNodes.push_back(OctreeNode());
}

void Octree::Clear()
{
	pNodeSizeMin=0.0;
	pNodes.clear();
	pNodes.push_back(OctreeNode());
}

void Octree::Build(vector<const Object *> *objects)
{
	fprintf(stderr, "Octree::Build\n");
	Vec3d globalMaxPos;

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
		Vec3d ObjPosition=obj->Position();
		globalMaxPos=Vec3d::Max(globalMaxPos, ObjPosition.Abs());
	}

	if(globalMaxPos.Length()<=0.0)
	{
		return;
	}

	pNodes[0].halfSize=globalMaxPos.Length();
	pNodeSizeMin=pNodes[0].halfSize*2.0;

	SplitNode(0UL, objects);

	fprintf(stderr, "%lu nodes total\n", pNodes.size());
	fprintf(stderr, "Root node size: %lf\n", pNodes[0].halfSize*2.0);
	fprintf(stderr, "Smallest node size: %lf\n", pNodeSizeMin);
}

const OctreeNode *Octree::GetClosestLeafNode(Vec3d point) const
{
	uint64_t NodeIndex=0, OctantIndex;
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
