#include <iostream>
#include "level_db_interfaces.h"

using cross_compiler_interface::use_unknown;


// Need this for MSVC bug  http://connect.microsoft.com/VisualStudio/feedback/details/772001/codename-milan-c-11-compilation-issue#details
using namespace leveldb_cc;

int main(){


	cross_compiler_interface::module m("leveldb_cc_dll");

	auto creator = cross_compiler_interface::create_unknown(m,"CreateLevelDBStaticFunctions")
		.QueryInterface<leveldb_cc::ILevelDBStaticFunctions>();

	// Open a scope so db goes out of scope so we can delete the database
	{
		use_unknown<leveldb_cc::IDB> db;
		auto options = creator.CreateOptions();
		options.set_create_if_missing(true);
		options.set_write_buffer_size(8*1024*1024);

		auto status = creator.Open(options,"c:/tmp/testdb",&db);

		auto wo = creator.CreateWriteOptions();
		wo.set_sync(false);
		auto wb = creator.CreateWriteBatch();
		wb.Put("Name3","John3");
		wb.Put("Name4","Paul4");
		wb.Put("Name5","Paul5");
		wb.Put("Name6","Paul6");

		db.Write(wo,wb);




		auto ro = creator.CreateReadOptions();
		std::string name;
		db.Get(ro,"Name",&name);

		std::cout << name << "\n";

		auto iter = db.NewIterator(ro);
		for(iter.SeekToFirst();iter.Valid();iter.Next()){
			std::cout << iter.key().ToString() << "=" << iter.value().ToString() << "\n";

		};

		std::cout << "\n\n";

		db.Delete(wo,"Name3");
		auto iter2 = db.NewIterator(ro);
		for(iter2.SeekToFirst();iter2.Valid();iter2.Next()){
			std::cout << iter2.key().ToString() << "=" << iter2.value().ToString() << "\n";

		};
	}
	auto s = creator.DestroyDB("c:/tmp/testdb",creator.CreateOptions());
	std::cout << s.ToString() << "\n\n";

}