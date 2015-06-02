#include "SpatialObjectStream.hpp"


using namespace std;
namespace FLAT
{

    SpatialObjectStream::SpatialObjectStream()
    {
		objectType=NONE;
		objectCount=0;
		objectByteSize=0;
		Box::infiniteBox(universe);
    }

    SpatialObjectStream::~SpatialObjectStream()
    {

    }
}
