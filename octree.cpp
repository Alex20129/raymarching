#include <cmath>
#include <cstdio>
#include <algorithm>
#include "octree.hpp"

bool OctreeNode::IsLeaf() const
{
	return(!branch[0] && !branch[1] && !branch[2] && !branch[3] &&
		!branch[4] && !branch[5] && !branch[6] && !branch[7]);
}

int Octree::SortObjectsByDistance(OctreeNode *node, vector <const Object *> *objects, vector <const Object *> &objects_by_disance)
{
	if(objects->empty())
	{
		return(0);
	}
	const Vec3d NodeCenter=node->center;
	double NodeBSphereRadius=Vec3d(node->halfSize, node->halfSize, node->halfSize).Length();
	int ObjectsInTouch=0;
	vector <DistancedObject> DistancedObjectsList;
	for(const Object *object : *objects)
	{
		if(!object->Visible())
		{
			continue;
		}
		double Distance=object->GetDistance(NodeCenter);
		DistancedObjectsList.push_back({object, Distance});
		if(Distance<NodeBSphereRadius)
		{
			ObjectsInTouch++;
		}
	}
	objects_by_disance.clear();
	objects_by_disance.reserve(DistancedObjectsList.size());
	std::sort(DistancedObjectsList.begin(), DistancedObjectsList.end(), ObjectDistanceComparator);
	for(const DistancedObject &DistancedObject : DistancedObjectsList)
	{
		objects_by_disance.push_back(DistancedObject.object);
	}
	return(ObjectsInTouch);
}

void Octree::SplitNode(OctreeNode *node, vector <const Object *> *objects)
{
	double SubNodeHalfSize=node->halfSize/2.0;
	pNodeSizeMin=SubNodeHalfSize*2.0;
	for(int n=0; n<8; n++)
	{
		OctreeNode *newSubNode=new OctreeNode;
		newSubNode->halfSize=SubNodeHalfSize;
		newSubNode->index=pNodes.size();

		double dx=(n & 1) ? SubNodeHalfSize : -SubNodeHalfSize;
		double dy=(n & 2) ? SubNodeHalfSize : -SubNodeHalfSize;
		double dz=(n & 4) ? SubNodeHalfSize : -SubNodeHalfSize;

		newSubNode->center=Vec3d(
				node->center.X + dx,
				node->center.Y + dy,
				node->center.Z + dz);

		pNodes.push_back(newSubNode);
		node->branch[n]=newSubNode->index;

		std::vector <const Object *> ObjectsOrdered;
		int ObjectsInTouch=SortObjectsByDistance(newSubNode, objects, ObjectsOrdered);

		if(ObjectsInTouch>1)
		{
			if(SubNodeHalfSize>1.0)
			{
				SplitNode(newSubNode, objects);
			}
			else
			{
				newSubNode->objectA=ObjectsOrdered.at(0);
				newSubNode->objectB=ObjectsOrdered.at(1);
			}
		}
		else
		{
			newSubNode->objectA=ObjectsOrdered.at(0);
			newSubNode->objectB=ObjectsOrdered.at(1);
		}
	}
}

Octree::Octree()
{
	pNodeSizeMin=0.0;
	OctreeNode *rootNode=new OctreeNode;
	pNodes.push_back(rootNode);
}

Octree::~Octree()
{
	while(pNodes.size())
	{
		OctreeNode *node=pNodes.back();
		pNodes.pop_back();
		delete node;
	}
}

void Octree::Clear()
{
	while(pNodes.size()>1)
	{
		OctreeNode *node=pNodes.back();
		pNodes.pop_back();
		delete node;
	}
	pNodeSizeMin=0.0;
}

void Octree::Build(vector<const Object *> *objects)
{
	fprintf(stderr, "Octree::Build\n");
	Vec3d globalMaxPos;

	Clear();
	OctreeNode *rootNode=pNodes.front();

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

	rootNode->halfSize=globalMaxPos.Length();

	SplitNode(rootNode, objects);

	fprintf(stderr, "%lu nodes total\n", pNodes.size());
	fprintf(stderr, "Root node size: %lf\n", rootNode->halfSize*2.0);
	fprintf(stderr, "Smallest node size: %lf\n", pNodeSizeMin);
}

const OctreeNode *Octree::GetClosestLeafNode(Vec3d point) const
{
	const OctreeNode *Node=pNodes[0];
	while(!Node->IsLeaf())
	{
		int octant_index=0;
		if(point.X >= Node->center.X)
		{
			octant_index |= 1;
		}
		if(point.Y >= Node->center.Y)
		{
			octant_index |= 2;
		}
		if(point.Z >= Node->center.Z)
		{
			octant_index |= 4;
		}
		Node=pNodes[Node->branch[octant_index]];
	}
	return(Node);
}
