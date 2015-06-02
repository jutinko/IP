#include "SpatialQuery.hpp"

#include <fstream>

namespace FLAT
{

  void tokenize(const std::string& str,std::vector<std::string>& tokens,const std::string& delimiters = " ")
  {
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos) 
    {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
    }
  }

  SpatialQuery::SpatialQuery()
  {

  }

  void SpatialQuery::ReadQueries(vector<SpatialQuery>& queries, string filename)
  {
    try
    {
      std::ifstream readFile;
      readFile.open(filename.c_str(),std::ios::in);
      std::string line;
      getline (readFile,line);
      SpatialQueryType type = (SpatialQueryType)atoi(line.c_str());

      if (type==RANGE_QUERY)
      {
        if (readFile.is_open())
        {
          while (!readFile.eof())
          {
            std::vector<std::string> tokens;
            getline (readFile,line);
            if (readFile.eof()) break;
            tokenize(line, tokens);
            SpatialQuery temp;
            temp.type = (SpatialQueryType)type;
            temp.Region.low[0]   = atof(tokens.at(0).c_str());
            temp.Region.low[1]   = atof(tokens.at(1).c_str());
            temp.Region.low[2]   = atof(tokens.at(2).c_str());
            temp.Region.high[0]  = atof(tokens.at(3).c_str());
            temp.Region.high[1]  = atof(tokens.at(4).c_str());
            temp.Region.high[2]  = atof(tokens.at(5).c_str());
            queries.push_back(temp);
          }
          readFile.close();
        }
      }
      else if (type==MOVING_QUERY)
      {
        getline (readFile,line);
        int Queries = atoi(line.c_str());

        if (readFile.is_open())
        {
          for (int i=0;i<Queries;i++)
          {
            SpatialQuery tempQuery;
            tempQuery.type = (SpatialQueryType)type;
            getline (readFile,line);
            int Steps = atoi(line.c_str());
            for (int j=0;j<Steps;j++)
            {
              std::vector<std::string> tokens;
              getline (readFile,line);
              if (readFile.eof())
              {
#ifdef FATAL
                cout << "UnExpected end of file : " << filename;
#endif
              }
              tokenize(line, tokens);
              Box temp;
              temp.low[0]   = atof(tokens.at(0).c_str());
              temp.low[1]   = atof(tokens.at(1).c_str());
              temp.low[2]   = atof(tokens.at(2).c_str());
              temp.high[0]  = atof(tokens.at(3).c_str());
              temp.high[1]  = atof(tokens.at(4).c_str());
              temp.high[2]  = atof(tokens.at(5).c_str());
              tempQuery.Moving.push_back(temp);
            }
            queries.push_back(tempQuery);
          }
          readFile.close();
        }
      }
      else
      {
        int k;
        bool isKNN = false;
        if(type==KNN_QUERY)
        {
          isKNN = true;
          getline (readFile,line);
          k = atoi(line.c_str());
        }

        if (readFile.is_open())
        {
          while (!readFile.eof())
          {
            std::vector<std::string> tokens;
            getline (readFile,line);
            if (readFile.eof()) break;
            tokenize(line, tokens);
            SpatialQuery temp;
            if(isKNN) 
            {
              temp.k = k;
            }
            temp.type = (SpatialQueryType)type;
            temp.Point[0]   = atof(tokens.at(0).c_str());
            temp.Point[1]   = atof(tokens.at(1).c_str());
            temp.Point[2]   = atof(tokens.at(2).c_str());
            queries.push_back(temp);
          }
          readFile.close();
        }
      }
    }
    catch(...)
    {
#ifdef FATAL
      cout << "Could not read queries from file : " << filename;
#endif
    }
  }

  void SpatialQuery::WriteQueries(vector<SpatialQuery>& queries, string filename,SpatialQueryType type)
  {
    if (queries.size()<=0)
    {
#ifdef FATAL
      cout << "Zero queries to write: " << filename;
#endif
      return;
    }

    try
    {
      std::ofstream writeFile;
      writeFile.open (filename.c_str(),std::ios::out |std::ios::trunc);
      writeFile << type << endl;

      if (type==RANGE_QUERY)
      {
        for (std::vector<SpatialQuery>::iterator i = queries.begin();i!=queries.end();++i)
          writeFile << i->Region.low[0] << " " << i->Region.low[1] << " " << i->Region.low[2]
            << " " << i->Region.high[0] << " " << i->Region.high[1] << " " << i->Region.high[2] << " " << std::endl;
      }
      else if (type==MOVING_QUERY)
      {
        writeFile << queries.size() << endl;
        for (std::vector<SpatialQuery>::iterator i = queries.begin();i!=queries.end();++i)
        {
          writeFile << i->Moving.size() << endl;
          for (std::vector<Box>::iterator j = i->Moving.begin();j!=i->Moving.end();++j)
            writeFile << j->low[0] << " " << j->low[1] << " " << j->low[2]
              << " " << j->high[0] << " " << j->high[1] << " " << j->high[2] << " " << std::endl;
        }
      }
      else
      {
        for (std::vector<SpatialQuery>::iterator i = queries.begin();i!=queries.end();++i)
          writeFile << i->Point[0] << " " << i->Point[1] << " " << i->Point[2] << " "<< std::endl;
      }
      writeFile.close();
    }
    catch(...)
    {
#ifdef FATAL
      cout << "Could not write queries to file : " << filename;
#endif
    }
  }

  QueryStatistics::QueryStatistics()
  {
    FLAT_seedIOs=0;
    FLAT_metaDataIOs=0;
    FLAT_payLoadIOs=0;
    FLAT_metaDataEntryLookup=0;
    FLAT_seedId=-1;

    FLAT_prefetchMetaHits=0;
    FLAT_prefetchPayLoadHit=0;
    FLAT_prefetchBuildingComparison=0;
    FLAT_prefetchVertices=0;
    FLAT_prefetchEdges=0;
    FLAT_prefetchPredictionComparison=0;
    FLAT_prefetchEntryCandidates=0;

    UselessPoints=0;
    ResultPoints=0;
    ObjectsPerPage=0;
    ObjectSize=0;

    RTREE_nodeIOs=0;
    RTREE_leafIOs=0;
  }

  void QueryStatistics::add(QueryStatistics& qs)
  {
    FLAT_seedIOs     += qs.FLAT_seedIOs;
    FLAT_metaDataIOs += qs.FLAT_metaDataIOs;
    FLAT_payLoadIOs  +=qs.FLAT_payLoadIOs;
    FLAT_metaDataEntryLookup +=FLAT_metaDataEntryLookup;

    UselessPoints +=qs.UselessPoints;
    ResultPoints +=qs.ResultPoints;

    RTREE_nodeIOs +=qs.RTREE_nodeIOs;
    RTREE_leafIOs +=qs.RTREE_leafIOs;

    FLAT_prefetchMetaHits+= qs.FLAT_prefetchMetaHits;
    FLAT_prefetchPayLoadHit+= qs.FLAT_prefetchPayLoadHit;
    FLAT_prefetchBuildingComparison+=qs.FLAT_prefetchBuildingComparison;

    FLAT_prefetchVertices+=qs.FLAT_prefetchVertices;
    FLAT_prefetchEdges+=qs.FLAT_prefetchEdges;

    FLAT_prefetchPredictionComparison+=qs.FLAT_prefetchPredictionComparison;
    FLAT_prefetchEntryCandidates+=qs.FLAT_prefetchEntryCandidates;

    executionTime.add(qs.executionTime);
    ioTime.add(qs.ioTime);
    modelingTime.add(qs.modelingTime);
    predictionTime.add(qs.predictionTime);
    prefetchingTime.add(qs.prefetchingTime);
    FLAT_seeding.add(qs.FLAT_seeding);
    FLAT_crawling.add(qs.FLAT_crawling);
    hashValueTime.add(qs.hashValueTime);
    walkTime.add(qs.walkTime);
  }

  void QueryStatistics::printFLATheader()
  {
    std::cout  << "TotalIO" << "\t"<< "SeedIO" << "\t" << "MetaDataIO" << "\t" << "PayLoadIO" << "\t"
      << "TotalTime" << "\t" << "SeedTime" << "\t" << "CrawlTime" << "\t" << "Results" << "\t" << "Useless" << "\t"
      << "Total MB" << "\t" << "Seed MB" << "\t" << "Metadata MB" << "\t" << "Result MB" << "\t" << "Useless MB" << "\t" << "Empty MB" << "\t"
      << "IO per Result" << "\t" << "Time per Result";
  }

  void QueryStatistics::printFLATstats()
  {
    uint64 FLAT_TotalIO  = FLAT_seedIOs + FLAT_metaDataIOs + FLAT_payLoadIOs;
    double FLAT_TotalMB  = ((FLAT_TotalIO+0.0)*(PAGE_SIZE+0.0))/1024.0/1024.0;

    double FLAT_SeedMB    = ((FLAT_seedIOs+0.0)*(PAGE_SIZE+0.0))/1024.0/1024.0;
    double FLAT_MetadataMB= ((FLAT_metaDataIOs+0.0)*(PAGE_SIZE+0.0))/1024.0/1024.0;
    double FLAT_ResultMB = ((ResultPoints+0.0)*(ObjectSize+0.0))/1024.0/1024.0;
    double FLAT_UselessMB= ((UselessPoints+0.0)*(ObjectSize+0.0))/1024.0/1024.0;
    double FLAT_EmptyMB  = FLAT_TotalMB - (FLAT_SeedMB+FLAT_MetadataMB+FLAT_ResultMB+FLAT_UselessMB);

    double FLAT_IOperResult = (FLAT_TotalIO+0.0)/(ResultPoints+0.0);
    double FLAT_TimeperResult = (executionTime._elapsed_milliseconds+0.0)/(ResultPoints+0.0)/1000.0;

    std::cout << FLAT_TotalIO << "\t" << FLAT_seedIOs << "\t" << FLAT_metaDataIOs << "\t" << FLAT_payLoadIOs << "\t"
      << executionTime << "\t" << FLAT_seeding << "\t" << FLAT_crawling  << "\t"<< ResultPoints << "\t" << UselessPoints << "\t"
      << FLAT_TotalMB << "\t" << FLAT_SeedMB << "\t" << FLAT_MetadataMB << "\t" << FLAT_ResultMB << "\t" << FLAT_UselessMB << "\t" << FLAT_EmptyMB << "\t"
      << FLAT_IOperResult << "\t" << FLAT_TimeperResult <<endl;
  }

  void QueryStatistics::printFLATprefetchStat()
  {
    uint64 FLAT_TotalIO  = FLAT_payLoadIOs+FLAT_metaDataIOs;
    uint64 TotalHit = FLAT_prefetchPayLoadHit+FLAT_prefetchMetaHits;

    float accuracy = ((TotalHit+0.0) / (FLAT_TotalIO+0.0)) *100;

    std::cout << accuracy << "\t" << TotalHit << "\t" << FLAT_TotalIO << "\t"<< ResultPoints << "\t" << executionTime << "\t"
      << ioTime << "\t" << modelingTime << "\t" << predictionTime << "\t" << walkTime << "\t" << prefetchingTime
      << "\t"<<  FLAT_prefetchBuildingComparison << "\t" << FLAT_prefetchVertices << "\t" << FLAT_prefetchEdges << "\t"
      << FLAT_prefetchPredictionComparison << "\t" << FLAT_prefetchEntryCandidates << endl;

    //    std::cout << accuracy << "\t" << error << "\t" << FLAT_TotalIO << "\t" << TotalHit << "\t" << TotalMiss
    //        << "\t" << FLAT_metaDataIOs << "\t" << FLAT_prefetchMetaHits
    //        << "\t" << FLAT_payLoadIOs << "\t" << FLAT_prefetchPayLoadHit   << "\t"
    //        << ResultPoints << endl;
  }

  void QueryStatistics::printRTREEheader()
  {
    std::cout  << "TotalIO" << "\t"<< "NodeIO" << "\t" << "LeafIO" << "\t"
      << "TotalTime" << "\t" << "Results" << "\t" << "Useless" << "\t"
      << "Total MB" << "\t" << "Node MB" << "\t" << "Result MB" << "\t" << "Useless MB" << "\t" << "Empty MB" << "\t"
      << "IO per Result" << "\t" << "Time per Result";
  }

  void QueryStatistics::printRTREEstats()
  {
    uint64 RTREE_TotalIO  = RTREE_nodeIOs+RTREE_leafIOs;
    double RTREE_TotalMB  = ((RTREE_TotalIO+0.0)*(PAGE_SIZE+0.0))/1024.0/1024.0;
    double RTREE_NodeMB   = ((RTREE_nodeIOs+0.0)*(PAGE_SIZE+0.0))/1024.0/1024.0;
    double RTREE_ResultMB = ((ResultPoints+0.0)*(ObjectSize+0.0))/1024.0/1024.0;
    double RTREE_UselessMB= ((UselessPoints+0.0)*(ObjectSize+0.0))/1024.0/1024.0;
    double RTREE_EmptyMB  = RTREE_TotalMB - (RTREE_NodeMB+RTREE_ResultMB+RTREE_UselessMB);
    double RTREE_IOperResult = (RTREE_TotalIO+0.0)/(ResultPoints+0.0);
    double RTREE_TimeperResult = (executionTime._elapsed_milliseconds+0.0)/(ResultPoints+0.0)/1000.0;

    std::cout << RTREE_TotalIO << "\t" << RTREE_nodeIOs << "\t" << RTREE_leafIOs << "\t"
      << executionTime << "\t" << ResultPoints << "\t" << UselessPoints << "\t"
      << RTREE_TotalMB << "\t" << RTREE_NodeMB << "\t" << RTREE_ResultMB << "\t" << RTREE_UselessMB << "\t" << RTREE_EmptyMB << "\t"
      << RTREE_IOperResult << "\t" << RTREE_TimeperResult <<endl;
  }

  void QueryStatistics::printBRUTEstats()
  {
    std::cout << ResultPoints << "\t" << executionTime << endl;
  }

}
