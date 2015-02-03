#ifndef SYNAPSE_HPP
#define SYNAPSE_HPP

#include <math.h>
#include "SpatialObject.hpp"
#include "Box.hpp"

namespace FLAT
{
	/*
	 * Synapse Model
	 * and Basic Geometric Functions
	 *
	 * Author: Farhan Tauheed
	 */
	class Synapse:public SpatialObject
	{
	public:
		Vertex postSynPosition;
		Vertex preSynPosition;
		uint32 globalSynapseId;
		uint32 synapseCounterId;
		uint32 postSynapticId;
		uint32 preSynapticId;
		spaceUnit spineLength;

		Synapse()
		{
		}

		Synapse(const Vertex& pre,const Vertex& post, const spaceUnit& l,uint32 gSynId,uint32 cSynId,uint32 preId,uint32 postId)
		{
			postSynPosition = post;
			preSynPosition = pre;
			globalSynapseId = gSynId;
			synapseCounterId = cSynId;
			postSynapticId = postId;
			preSynapticId = preId;
			spineLength = l;
		}

		Synapse 	 & operator  =(const Synapse& rhs);
		bool 		   operator ==(const Synapse& rhs) const;
		friend std::ostream & operator << (std::ostream & lhs, const Synapse& rhs);

		// Spatial Object virtual functions
		Box getMBR();
		Vertex getCenter();
		spaceUnit getSortDimension(int dimension);
		bool IsResult(Box& region);
		void serialize(int8* buffer);
		void unserialize(int8* buffer);
		uint32 getSize();
		SpatialObjectType getType();
		bigSpaceUnit pointDistance(Vertex& p);
	};

	inline Synapse & Synapse::operator=(const Synapse &rhs)
	{
		postSynPosition = rhs.postSynPosition;
		preSynPosition = rhs.preSynPosition;

		globalSynapseId = rhs.globalSynapseId;
		synapseCounterId = rhs.synapseCounterId;

		postSynapticId = rhs.postSynapticId;
		preSynapticId = rhs.preSynapticId;

		spineLength = rhs.spineLength;
		return *this;
	}

	inline bool Synapse::operator==(const Synapse &rhs) const
	{
		return   (
				postSynPosition == rhs.postSynPosition &&
				preSynPosition == rhs.preSynPosition &&
				globalSynapseId == rhs.globalSynapseId &&
				synapseCounterId == rhs.synapseCounterId &&
				postSynapticId == rhs.postSynapticId &&
				preSynapticId == rhs.preSynapticId &&
				spineLength == rhs.spineLength
				);
	}
}
#endif
