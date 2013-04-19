#include <iostream>
#include  "leveldb_cc/level_db_interfaces.h"

using cross_compiler_interface::use_unknown;


// Need this for MSVC bug  http://connect.microsoft.com/VisualStudio/feedback/details/772001/codename-milan-c-11-compilation-issue#details
using namespace leveldb_cc;

int main(){


	cross_compiler_interface::module m("leveldb_cc_dll");

	auto creator = cross_compiler_interface::create_unknown(m,"CreateLevelDBStaticFunctions")
		.QueryInterface<leveldb_cc::ILevelDBStaticFunctions>();

	// Open a scope so db goes out of scope so we can delete the database
	{
		auto options = creator.CreateOptions();
		options.set_create_if_missing(true);
		options.set_write_buffer_size(8*1024*1024);

		// Set cache of 1MB
		options.set_block_cache(creator.NewLRUCache(1024*1024));

		// Set bloom filter with 10 bits per key
		options.set_filter_policy(creator.NewBloomFilterPolicy(10));

		// Open the db		
		auto db = creator.OpenDB(options,"/tmp/testdb");

		auto wo = creator.CreateWriteOptions();
		wo.set_sync(false);

		// Add a few key/value pairs in a batch
		auto wb = creator.CreateWriteBatch();

		wb.Put("Key1","Value1");
		wb.Put("Key2","Value2");
		wb.Put("Key3","Value3");
		wb.Put("Key4","Value4");
		
		wo.set_sync(true);
		db.WriteBatch(wo,wb);



		auto ro = creator.CreateReadOptions();
		
		// Read a value that is not there
		try{
			std::string name = db.GetValue(ro,"Name");
			std::cout << name << "\n";
		}
		catch(std::exception& e){
			bad_status* ps = nullptr;
			if((ps = dynamic_cast<bad_status*>(&e)) && ps->status().IsNotFound()){
				std::cout << "Not Found ";
			}
			std::cout << "Error: " << e.what() << std::endl;

		}

		// Save a snapshot
		auto snapshot = db.GetSnapshot();

		// Add more stuff to db
		db.PutValue(wo,"AfterSnapshot1","More Value1");

		
		// Use the snapshot
		ro.set_snapshot(snapshot);
		auto iter = db.NewIterator(ro);
		std::cout << "Iterator with snapshot\n";
		for(iter.SeekToFirst();iter.Valid();iter.Next()){
			std::cout << iter.key().ToString() << "=" << iter.value().ToString() << "\n";

		};

		std::cout << "\n\n";

		// Clear the snapshot
		ro.set_snapshot(nullptr);
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
		std::vector<Range> vr;
		vr.push_back(Range(Slice("Name3"),Slice("Name4")));
		vr.push_back(Range(Slice("Name3"),Slice("Name6")));
		auto v = db.GetApproximateSizes(vr);
		for(auto i:v){
			std::cout << i << std::endl;
		};
	}

	// Delete the db 
	auto s = creator.DestroyDB("/tmp/testdb",creator.CreateOptions());

}
