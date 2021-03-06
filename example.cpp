//          Copyright John R. Bandela 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include <iostream>
#include "leveldb_interfaces.h"


int main(){
#ifdef _WIN32
		std::string path{ "c:/tmp/testdb" };
#else
		std::string path{ "/tmp/testdb" };
#endif
	// Open a scope so db goes out of scope so we can delete the database
	{
		leveldb_cc::ReadOptions op1;
		int major, minor;
		std::tie(major, minor) = leveldb_cc::DB::GetVersion();
		std::cout << "Trying leveldb version " << major << "." << minor  << "\n";
		leveldb_cc::Options options;
		options.CreateIfMissing = true;
		options.WriteBufferSize = 8 * 1024 * 1024;

		// Set cache of 1MB
		options.BlockCache = leveldb_cc::LRUCache{ 1024 * 1024 };

		// Set bloom filter with 10 bits per key
		options.FilterPolicy = leveldb_cc::BloomFilter{ 10 };

		// Open the db		

		leveldb_cc::DB db(options,path);

		leveldb_cc::WriteOptions wo;
		wo.Sync = false;

		// Add a few key/value pairs in a batch
		leveldb_cc::WriteBatch wb;

		wb.Put("Key1","Value1");
		wb.Put("Key2","Value2");
		wb.Put("Key3","Value3");
		wb.Put("Key4","Value4");
		
		wo.Sync = true;
		db.WriteBatch(wo,wb);



		leveldb_cc::ReadOptions ro;
		
		// Read a value that is not there
		try{
			std::string name = db.GetValue(ro,"Name");
			std::cout << name << "\n";
		}
		catch(std::exception& e){
			leveldb_cc::bad_status* ps = nullptr;
			if ((ps = dynamic_cast<leveldb_cc::bad_status*>(&e)) && ps->status().IsNotFound()){
				std::cout << "Not Found ";
			}
			std::cout << "Error: " << e.what() << std::endl;

		}

		// Save a snapshot
		auto snapshot = db.GetSnapshot();

		// Add more stuff to db
		db.PutValue(wo,"AfterSnapshot1","More Value1");

		
		// Use the snapshot
		ro.Snapshot = snapshot;
		auto iter = db.NewIterator(ro);
		std::cout << "Iterator with snapshot\n";
		for(iter.SeekToFirst();iter.Valid();iter.Next()){
			std::cout << iter.key().ToString() << "=" << iter.value().ToString() << "\n";

		};

		std::cout << "\n\n";

		// Clear the snapshot
		ro.Snapshot = nullptr;
		db.ReleaseSnapshot(snapshot);

		auto iter2 = db.NewIterator(ro);
		std::cout << "Iterator without snapshot\n";
		for(iter2.SeekToFirst();iter2.Valid();iter2.Next()){
			std::cout << iter2.key().ToString() << "=" << iter2.value().ToString() << "\n";

		};
		std::cout << "\n\n";

		db.DeleteValue(wo,"Key1");
		auto iter3 = db.NewIterator(ro);
		std::cout << "Iterator after delete Key1 snapshot\n";
		for(iter3.SeekToFirst();iter3.Valid();iter3.Next()){
			std::cout << iter3.key().ToString() << "=" << iter3.value().ToString() << "\n";

		};

		std::cout << "\n\n";

		// Get approximate sizes - seems to return 0
		// maybe because keys/values are small
		std::vector<leveldb_cc::Range> vr;
		vr.push_back(leveldb_cc::Range(leveldb_cc::Slice("Name3"), leveldb_cc::Slice("Name4")));
		vr.push_back(leveldb_cc::Range(leveldb_cc::Slice("Name3"), leveldb_cc::Slice("Name6")));
		auto v = db.GetApproximateSizes(vr);
		for(auto i:v){
			std::cout << i << std::endl;
		};
	}

	// Delete the db 
	auto s = leveldb_cc::DB::DestroyDB(path, leveldb_cc::Options{});

}