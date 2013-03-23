#include <iostream>
#include "level_db_interfaces.h"

using cross_compiler_interface::use_unknown;
int main(){
	auto creator = cross_compiler_interface::create_unknown("leveldb_cc_dll","CreateLevelDBStaticFunctions")
		.QueryInterface<ILevelDBStaticFunctions>();

	use_unknown<IDB> db;
	auto options = creator.CreateOptions();
	options.set_create_if_missing(true);
	options.set_write_buffer_size(8*1024*1024);

	auto status = creator.Open(options,"c:/tmp/testdb",&db);
	
	auto wo = creator.CreateWriteOptions();
	wo.set_sync(false);

	db.Put(wo,"Name","John");

	auto ro = creator.CreateReadOptions();
	std::string name;
	db.Get(ro,"Name",&name);

	std::cout << name;




}