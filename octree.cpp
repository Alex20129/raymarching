#include <cmath>
#include <cstdio>
#include "octree.hpp"

bool OctreeNode::IsLeaf() const
{
	return(!branch[0] && !branch[1] && !branch[2] && !branch[3] &&
		!branch[4] && !branch[5] && !branch[6] && !branch[7]);
}

enum class IntersectionType
{
	NoIntersection,
	PartialIntersection,
	FullIntersection,
};

static IntersectionType NodeIntersectsWithObjectSDF(const OctreeNode *node, const Object *object)
{
	IntersectionType result=IntersectionType::NoIntersection;
	Vec3d NodeCenter=node->center;
	double NodeHalfSize=node->halfSize;

	double CenterToSurface=object->GetDistance(NodeCenter);
	if (CenterToSurface < NodeHalfSize)
	{
		result=IntersectionType::PartialIntersection;
	}

	double NodeBSRadius=Vec3d(NodeHalfSize, NodeHalfSize, NodeHalfSize).Length();
	if (CenterToSurface < -NodeBSRadius)
	{
		return IntersectionType::FullIntersection;
	}

	int SamplesCollected=0;
	if (object->GetDistance(NodeCenter+Vec3d(NodeHalfSize, 0, 0)) < 0.0)
	{
		SamplesCollected++;
	}
	if (object->GetDistance(NodeCenter+Vec3d(-NodeHalfSize, 0, 0)) < 0.0)
	{
		SamplesCollected++;
	}
	if (object->GetDistance(NodeCenter+Vec3d(0, NodeHalfSize, 0)) < 0.0)
	{
		SamplesCollected++;
	}
	if (object->GetDistance(NodeCenter+Vec3d(0, -NodeHalfSize, 0)) < 0.0)
	{
		SamplesCollected++;
	}
	if (object->GetDistance(NodeCenter+Vec3d(0, 0, NodeHalfSize)) < 0.0)
	{
		SamplesCollected++;
	}
	if (object->GetDistance(NodeCenter+Vec3d(0, 0, -NodeHalfSize)) < 0.0)
	{
		SamplesCollected++;
	}
	for (int n=0; n<8; n++)
	{
		double dx=(n & 1) ? NodeHalfSize : -NodeHalfSize;
		double dy=(n & 2) ? NodeHalfSize : -NodeHalfSize;
		double dz=(n & 4) ? NodeHalfSize : -NodeHalfSize;
		if (object->GetDistance(NodeCenter+Vec3d(dx, dy, dz)) < 0.0)
		{
			SamplesCollected++;
		}
	}
	if(SamplesCollected==14)
	{
		result=IntersectionType::FullIntersection;
	}

	return result;
}

static void checkForObjectsNearby(OctreeNode *node, vector <const Object *> *objects, std::vector<const Object *> &objects_found)
{
	for(const Object *object : *objects)
	{
		if(!object->Visible())
		{
			continue;
		}
		switch(NodeIntersectsWithObjectSDF(node, object))
		{
			case IntersectionType::NoIntersection:
			{
				continue;
			}break;
			case IntersectionType::PartialIntersection:
			{
				objects_found.push_back(object);
			}break;
			case IntersectionType::FullIntersection:
			{
				objects_found.clear();
				objects_found.push_back(object);
				break;
			}break;
		}
	}
}

void Octree::SplitNode(OctreeNode *node, vector<const Object *> *objects)
{
	double SubNodeHalfSize=node->halfSize/2.0;
	uint64_t ParentNodeIndex=node->index;
	if(SubNodeHalfSize<1.0)
	{
		return;
	}
	pNodeSizeMin=SubNodeHalfSize*2.0;
	for(int n=0; n<8; n++)
	{
		OctreeNode *newSubNode=new OctreeNode;
		newSubNode->halfSize=SubNodeHalfSize;
		newSubNode->index=pNodes.size();
		newSubNode->parentNodeIndex=ParentNodeIndex;

		double dx=(n & 1) ? SubNodeHalfSize : -SubNodeHalfSize;
		double dy=(n & 2) ? SubNodeHalfSize : -SubNodeHalfSize;
		double dz=(n & 4) ? SubNodeHalfSize : -SubNodeHalfSize;

		newSubNode->center=Vec3d(
				node->center.X + dx,
				node->center.Y + dy,
				node->center.Z + dz);

		pNodes.push_back(newSubNode);
		node->branch[n]=newSubNode->index;

		std::vector<const Object *> ObjectsFound;
		checkForObjectsNearby(newSubNode, objects, ObjectsFound);

		if(ObjectsFound.empty())
		{
			continue;
		}
		else if(ObjectsFound.size()==1)
		{
			newSubNode->object=ObjectsFound.front();
			pNodesWithObjects++;
			continue;
		}
		else
		{
			newSubNode->object=ObjectsFound.back();
			pNodesWithObjects++;
			SplitNode(newSubNode, objects);
		}
	}
}

Octree::Octree()
{
	pNodeSizeMin=0.0;
	pNodesWithObjects=0;
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
	pNodesWithObjects=0;
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

	for (const Object *obj : *objects)
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
	fprintf(stderr, "%lu nodes with objects\n", pNodesWithObjects);
	fprintf(stderr, "Root node size: %lf\n", rootNode->halfSize*2.0);
	fprintf(stderr, "Smallest node size: %lf\n", pNodeSizeMin);
}

OctreeNode *Octree::GetClosestLeafNode(Vec3d point) const
{
	OctreeNode *Node=pNodes[0];
	while (!Node->IsLeaf())
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
		Node=pNodes.at(Node->branch[octant_index]);
	}
	return(Node);
}

OctreeNode *Octree::GetNode(uint64_t node_index) const
{
	if(node_index>=pNodes.size())
	{
		node_index=pNodes.size()-1;
	}
	return(pNodes[node_index]);
}
