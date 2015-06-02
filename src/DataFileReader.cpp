#include "DataFileReader.hpp"

using namespace std;

namespace FLAT
{
	DataFileReader::DataFileReader(string& fileName)
	{
		try
		{
			inputFile = new BufferedFile();
			inputFile->open(fileName);
			counter=0;
			loadHeader();
		}
		catch(...)
		{
#ifdef FATAL
			cout << "Cannot load InputFile" << endl;
#endif
			exit(0);
		}
	}

	DataFileReader::~DataFileReader()
	{
		inputFile->close();
		delete inputFile;
	}

	void DataFileReader::loadHeader()
	{
		inputFile->seekEnd(RAW_DATA_HEADER_SIZE);
		objectType      = (SpatialObjectType)inputFile->readUInt32();
		objectCount     = inputFile->readUInt64();         // Number of Objects in Dataset
		objectByteSize  = inputFile->readUInt32();         // Size in Bytes of each Object
		inputFile->read(&universe);
		rewind();

#ifdef INFORMATION
		cout << "\n == INPUT FILE HEADER == \n\n"
		     << "OBJECT TYPE: " << SpatialObjectFactory::getTitle(objectType) << endl
			 << "TOTAL OBJECTS: " << objectCount << endl
			 << "OBJECT BYTE SIZE: " << objectByteSize <<endl
			 << "UNIVERSE BOUNDS: " << universe << endl;
#endif
	}

	bool DataFileReader::hasNext()
	{
		data = SpatialObjectFactory::create(objectType);

		inputFile->read(data);
		if (inputFile->eof || counter>=objectCount)
		{
			delete data;
			return false;
		}
		else
		{
			counter++;
			return true;
		}
	}

	SpatialObject* DataFileReader::getNext()
	{
		return data;
	}

	void DataFileReader::rewind()
	{
		inputFile->rewindForReading();
		counter=0;
	}
}
