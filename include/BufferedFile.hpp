#ifndef DIAS_FILING_HPP_
#define DIAS_FILING_HPP_

#include "SpatialObject.hpp"
#include <fstream>
#include <iostream>
using namespace std;

namespace FLAT
{
	/*
	 * Class for reading and writing on file
	 * supports large disk files
	 * uses internal buffer for fast access
	 * Does not allocates or deallocates memory.
	 * Allocating and Deallocating is responsibility of the caller
	 *
	 * Author: Farhan Tauheed
	 */
	class BufferedFile
	{
	public:
		BufferedFile();
		~BufferedFile();

		void open(string filename);
		void create(string filename);
		void maketemporary();

		void close();
		void rewindForReading();
		void seek(uint64 offset);
		void seekEnd(uint64 offset);

		void read(uint32 size, int8* buffer);
		void write(uint32 size, int8* buffer);

		int32 readInt32();
		int64 readInt64();

		float readFloat();
		double readDouble();

		uint8  readUInt8();
		uint32 readUInt32();
		void   writeUInt32(uint32 i);

		uint64 readUInt64();
		void   writeUInt64(uint64 i);

		void read(SpatialObject* sobj);
		void write(SpatialObject* sobj);

		fstream file;
		string filename;
		char* buffer;
		bool eof;
		bool temporary;
	};


	inline void BufferedFile::read(SpatialObject* sobj)
	{
		try
		{
			uint32 size = sobj->getSize();
			int8 buffer[size];
			read(size,buffer);
			sobj->unserialize(buffer);
		}
		catch(...)
		{
			eof = true;
		}
	}

	inline void BufferedFile::write(SpatialObject* sobj)
	{
		try
		{
			uint32 size = sobj->getSize();
			int8 buffer[size];
			sobj->serialize(buffer);
			write(size,buffer);
		}
		catch(...)
		{
			eof = true;
#ifdef FATAL
		cout << "Problem writing spatial object to disk file:" << filename << "\n";
#endif
		}
	}

	inline void BufferedFile::seek(uint64 offset)
	{
		try
		{
			eof = false;
			file.clear();
	#ifdef WIN32
			file.seekg(0, std::ios_base::beg);
			uint32 n4GBSeeks = offset / 0xFFFFFFFF;
			for(uint32 i = 0; i < n4GBSeeks; i++)
				file.seekg(0xFFFFFFFF, ios_base::cur);
			file.seekg(offset % 0xFFFFFFFF, ios_base::cur);
	#else
			file.seekg(offset, ios_base::beg);
	#endif
			if (!file.good()) throw 1;
		}
		catch(...)
		{
			eof = true;
	#ifdef FATAL
			cout << "Cannot Seek file" << filename << " to:" << offset << "\n";
	#endif
		}
	}

	inline void BufferedFile::read(uint32 size, int8* buffer)
	{
	#ifdef FATAL
		if (eof)
			cout << "Reading file after eof : " << filename << "\n";
	#endif
		try
		{
			file.read(buffer,size);
			if (!file.good()) throw 1;
			eof = file.eof();
		}
		catch(...)
		{
			eof= true;
		}
	}

	inline void BufferedFile::write(uint32 size, int8* buffer)
	{
		try
		{
			file.write(buffer, size);
			if (!file.good()) throw 1;
			eof = file.eof();
		}
		catch(...)
		{
			eof= true;
#ifdef FATAL
		cout << "Problem writing buffer to disk file: " << filename << "\n";
#endif
		}
	}

}


#endif
