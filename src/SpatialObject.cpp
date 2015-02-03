#include "SpatialObject.hpp"
#include "Vertex.hpp"

namespace FLAT
{
    SpatialObject::SpatialObject()
    {

    }
    SpatialObject::~SpatialObject()
    {

    }
	bool SpatialObjectXAsc::operator()(SpatialObject* const r1, SpatialObject* const r2)
	{
		spaceUnit a = r1->getSortDimension(0);
		spaceUnit b = r2->getSortDimension(0);

		if (a==b)
		{
			spaceUnit c = r1->getSortDimension(1);
			spaceUnit d = r2->getSortDimension(1);
			if (c==d)
			{
				spaceUnit e = r1->getSortDimension(2);
				spaceUnit f = r2->getSortDimension(2);
//				if (e==f && r1!=r2) std::cout << "WTF! objects with same center in the dataset! found while sorting X :Center = "
//						       << r1->getCenter() << " and " << r2->getCenter() << " MBR=" << r1->getMBR() << " and " << r2->getMBR() << " same pointer? " << (r1==r2)<< "\n";
				return (e<f);
			}
			else return (c<d);
		}
		else return (a<b);

//		if (r1->getSortDimension(0) < r2->getSortDimension(0)) return true;
//		else return false;
	}
	bool SpatialObjectYAsc::operator()(SpatialObject* const r1, SpatialObject* const r2)
	{
		spaceUnit a = r1->getSortDimension(1);
		spaceUnit b = r2->getSortDimension(1);

		if (a==b)
		{
			spaceUnit c = r1->getSortDimension(2);
			spaceUnit d = r2->getSortDimension(2);
			if (c==d)
			{
				spaceUnit e = r1->getSortDimension(0);
				spaceUnit f = r2->getSortDimension(0);
//				if (e==f && r1!=r2) std::cout << "WTF! objects with same center in the dataset! found while sorting Y :Center = "
//						       << r1->getCenter() << " and " << r2->getCenter() << " MBR=" << r1->getMBR() << " and " << r2->getMBR() << " same pointer? " << (r1==r2) <<"\n";
				return (e<f);
			}
			else return (c<d);
		}
		else return (a<b);

//		if (r1->getSortDimension(1) < r2->getSortDimension(1)) return true;
//		else return false;
	}
	bool SpatialObjectZAsc::operator()(SpatialObject* const r1, SpatialObject* const r2)
	{
		spaceUnit a = r1->getSortDimension(2);
		spaceUnit b = r2->getSortDimension(2);

		if (a==b)
		{
			spaceUnit c = r1->getSortDimension(0);
			spaceUnit d = r2->getSortDimension(0);
			if (c==d)
			{
				spaceUnit e = r1->getSortDimension(1);
				spaceUnit f = r2->getSortDimension(1);
//				if (e==f && r1!=r2) std::cout << "WTF! objects with same center in the dataset! found while sorting Z :Center = "
//						       << r1->getCenter() << " and " << r2->getCenter() << " MBR=" << r1->getMBR() << " and " << r2->getMBR() << " same pointer? " << (r1==r2)<< "\n";
				return (e<f);
			}
			else return (c<d);
		}
		else return (a<b);

		//if (r1->getSortDimension(2) < r2->getSortDimension(2)) return true;
		//else return false;
	}
}
