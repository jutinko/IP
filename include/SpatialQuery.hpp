#ifndef SPATIAL_QUERY_
#define SPATIAL_QUERY_

#include "Timer.hpp"
#include "Box.hpp"
#include <vector>

using namespace std;

namespace FLAT
{
	enum SpatialQueryType
	{
		RANGE_QUERY,
		SEED_QUERY,
		POINT_QUERY,
		KNN_QUERY,
		MOVING_QUERY
	};


	class QueryStatistics
	{
	public:
		Timer executionTime;
		Timer FLAT_seeding;
		Timer FLAT_crawling;

		Timer ioTime;
		Timer modelingTime;
		Timer predictionTime;
		Timer prefetchingTime;
		Timer hashValueTime;
		Timer walkTime;

		uint64 FLAT_seedIOs;
		uint64 FLAT_metaDataIOs;
		uint64 FLAT_payLoadIOs;
		uint64 FLAT_metaDataEntryLookup;
		int32 FLAT_seedId;
		uint64 FLAT_prefetchMetaHits;
		uint64 FLAT_prefetchPayLoadHit;
		uint64 FLAT_prefetchBuildingComparison;
		uint64 FLAT_prefetchVertices;
		uint64 FLAT_prefetchEdges;
		uint64 FLAT_prefetchPredictionComparison;
		uint64 FLAT_prefetchEntryCandidates;

		uint64 UselessPoints;
		uint64 ResultPoints;
		uint64 ObjectsPerPage;
		uint32 ObjectSize;

		uint64 RTREE_nodeIOs;
		uint64 RTREE_leafIOs;

		QueryStatistics();
		void add(QueryStatistics& qs);

		void printFLATheader();
		void printFLATstats();
		void printFLATprefetchStat();
		void printRTREEheader();
		void printRTREEstats();
		void printBRUTEstats();
	};


	class SpatialQuery
	{
	public:
		QueryStatistics stats;
		SpatialQueryType type;
		Box Region;     // for seed and range query
		Vertex Point;   // for point query or kNN
    uint32_t k;     // k for kNN
		vector<Box> Moving;

		SpatialQuery();

		static void ReadQueries(vector<SpatialQuery>& queries, string filename);
		static void WriteQueries(vector<SpatialQuery>& queries,string filename,SpatialQueryType type);
	};


}
#endif
