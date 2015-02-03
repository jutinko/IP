#include "Synapse.hpp"


namespace FLAT
{
	std::ostream & operator << (std::ostream & lhs, const Synapse& rhs)
	{
	lhs << "PostPos: " << rhs.postSynPosition << " PrePos: " << rhs.preSynPosition
		<< " SynID: " << rhs.globalSynapseId << " SynCount: " << rhs.synapseCounterId << " PostNeuronID: "
		<< rhs.postSynapticId << " PreNeuronID: " << rhs.preSynapticId << " SpineLength:" <<  rhs.spineLength;

	return lhs;
	}

	// Spatial Object virtual functions
	Box Synapse::getMBR()
	{
		Box box;
		std::vector<Vertex> corners;
		corners.push_back(postSynPosition);
		corners.push_back(preSynPosition);
		Box::boundingBox(box,corners);
		return box;
	}

	Vertex Synapse::getCenter()
	{
		return preSynPosition;
	}

	spaceUnit Synapse::getSortDimension(int dimension)
	{
		return preSynPosition.Vector[dimension];
	}

	bool Synapse::IsResult(Box& region)
	{
		return true;
	}

	SpatialObjectType Synapse::getType()
	{
		return SYNAPSE;
	}

	bigSpaceUnit Synapse::pointDistance(Vertex& p)
	{
		std::cout << "point distance not implemented for synapses" <<std::endl;
		return 0;
	}

	void Synapse::serialize(int8* buffer)
	{
		int8* ptr = buffer;

		memcpy(ptr,&(preSynPosition.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);

		memcpy(ptr,&(postSynPosition.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);

		memcpy(ptr,&spineLength,sizeof(spaceUnit));
		ptr += sizeof(spaceUnit);

		memcpy(ptr,&globalSynapseId,sizeof(uint32));
		ptr += sizeof(uint32);

		memcpy(ptr,&synapseCounterId,sizeof(uint32));
		ptr += sizeof(uint32);

		memcpy(ptr,&postSynapticId,sizeof(uint32));
		ptr += sizeof(uint32);

		memcpy(ptr,&preSynapticId,sizeof(uint32));
	}

	void Synapse::unserialize(int8* buffer)
	{
		int8* ptr = buffer;

		memcpy(&(preSynPosition.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);

		memcpy(&(postSynPosition.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);

		memcpy(&spineLength, ptr,sizeof(spaceUnit));
		ptr += sizeof(spaceUnit);

		memcpy(&globalSynapseId, ptr,sizeof(uint32));
		ptr += sizeof(uint32);

		memcpy(&synapseCounterId, ptr,sizeof(uint32));
		ptr += sizeof(uint32);

		memcpy(&postSynapticId, ptr,sizeof(uint32));
		ptr += sizeof(uint32);

		memcpy(&preSynapticId, ptr,sizeof(uint32));
	}

	uint32 Synapse::getSize()
	{
		return (DIMENSION*sizeof(spaceUnit)*2) + (sizeof(uint32)*4) + sizeof(spaceUnit);
	}
}
