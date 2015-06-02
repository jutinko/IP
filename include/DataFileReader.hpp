#ifndef DATA_FILE_READER_HPP_
#define DATA_FILE_READER_HPP_
#include "SpatialObjectStream.hpp"
#include "BufferedFile.hpp"
#include "Box.hpp"

using namespace std;

namespace FLAT
{
	/*
	 * Class Responsible for Handling the Input Data
	 * It Reads the Header info
	 * It Reads items from File into memory and returns
	 */
	class DataFileReader :public SpatialObjectStream
	{
	public:
		SpatialObject* data;
		BufferedFile* inputFile;      //Pointer to file containing input Data
		uint64 counter;

		DataFileReader(string& fileName);

		~DataFileReader();

		void loadHeader();

		bool hasNext();

		SpatialObject* getNext();

		void rewind();
	};
}

#endif
