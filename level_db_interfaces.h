#include "../cross_compiler_call/cross_compiler_interface/interface_unknown.hpp"
#include "slice.h"
#include "status.h"

using cross_compiler_interface::cross_function;
using cross_compiler_interface::use_unknown;
using cross_compiler_interface::implement_unknown_interfaces;
using cross_compiler_interface::out;

namespace leveldb_cc{

	class bad_status:public std::runtime_error{
		Status s_;
	public:
		bad_status(const Status& s):std::runtime_error(s.ToString()),s_(s){}

		const Status& status(){
			return s_;
		};
	};

enum CompressionType {
  // NOTE: do not change the values of existing entries, as these are
  // part of the persistent format on disk.
  kNoCompression     = 0x0,
  kSnappyCompression = 0x1
};

template<class T>
struct ISnapshot
	:public cross_compiler_interface::define_interface_unknown<T,
	// {D6F3D379-9E73-407F-846E-CCAFA1A3EA6E}
	cross_compiler_interface::uuid<
	0xD6F3D379,0x9E73,0x407F,0x84,0x6E,0xCC,0xAF,0xA1,0xA3,0xEA,0x6E
	>
	>
{
	ISnapshot()
	{}
};

template<class T>
struct IComparator
	:public cross_compiler_interface::define_interface_unknown<T,
	// {C5B46BB4-7CDC-41E9-BCC7-3EBAF62C8873}
	cross_compiler_interface::uuid<
	0xC5B46BB4,0x7CDC,0x41E9,0xBC,0xC7,0x3E,0xBA,0xF6,0x2C,0x88,0x73
	>
	>
{

	cross_function<IComparator,0,std::int32_t(Slice,Slice)> Compare;
	cross_function<IComparator,1,const char*()> Name;
	cross_function<IComparator,2,std::string(Slice)> FindShortestSeparator;
	cross_function<IComparator,3,std::string(std::string)> FindShortSuccessor;



	IComparator()
		:Compare(this),Name(this),FindShortestSeparator(this),FindShortSuccessor(this)
	{}
};

// Leave out support for logger,env, and cache

template<class T>
struct IOptions
	:public cross_compiler_interface::define_interface_unknown<T,
	// {FC12AA30-D42B-48FB-9A93-3716D962BFB7}
	cross_compiler_interface::uuid<
	0xFC12AA30,0xD42B,0x48FB,0x9A,0x93,0x37,0x16,0xD9,0x62,0xBF,0xB7
	>
	>
{
	cross_function<IOptions,0,bool()> get_create_if_missing;
	cross_function<IOptions,1,void(bool)> set_create_if_missing;

	cross_function<IOptions,2,bool()> get_error_if_exists;
	cross_function<IOptions,3,void(bool)> set_error_if_exists;

	cross_function<IOptions,4,bool()> get_paranoid_checks;
	cross_function<IOptions,5,void(bool)> set_paranoid_checks;

	cross_function<IOptions,6,std::uint32_t()> get_write_buffer_size;
	cross_function<IOptions,7,void(std::uint32_t)> set_write_buffer_size;

	cross_function<IOptions,8,std::uint32_t()> get_max_open_files;
	cross_function<IOptions,9,void(std::uint32_t)> set_max_open_files;


	cross_function<IOptions,10,std::uint32_t()> get_block_size;
	cross_function<IOptions,11,void(std::uint32_t)> set_block_size;

	cross_function<IOptions,12,std::uint32_t()> get_block_restart_interval;
	cross_function<IOptions,13,void(std::uint32_t)> set_block_restart_interval;

	cross_function<IOptions,14,std::uint32_t()> get_compression;
	cross_function<IOptions,15,void(std::uint32_t)> set_compression;

	cross_function<IOptions,16,void(use_unknown<IComparator>)> set_comparator;
	
	
	IOptions()
		:get_create_if_missing(this),set_create_if_missing(this),get_error_if_exists(this),
		set_error_if_exists(this),get_paranoid_checks(this),set_paranoid_checks(this),
		
		get_write_buffer_size(this),set_write_buffer_size(this),get_max_open_files(this),
		set_max_open_files(this),
		get_block_size(this),set_block_size(this),get_block_restart_interval(this),
		set_block_restart_interval(this),get_compression(this),set_compression(this),
		set_comparator(this)
	{}
};

template<class T>
struct IReadOptions
	:public cross_compiler_interface::define_interface_unknown<T,
	// {644668DE-09EC-49B1-A715-AD0D16CD8259}
	cross_compiler_interface::uuid<
	0x644668DE,0x09EC,0x49B1,0xA7,0x15,0xAD,0x0D,0x16,0xCD,0x82,0x59
	>
	>
{
	cross_function<IReadOptions,0,bool()> get_verify_checksums;
	cross_function<IReadOptions,1,void(bool)> set_verify_checksums;

	cross_function<IReadOptions,2,bool()> get_fill_cache;
	cross_function<IReadOptions,3,void(bool)> set_fill_cache;

	cross_function<IReadOptions,4,void(use_unknown<ISnapshot>)> set_snapshot;


	IReadOptions()
		:get_verify_checksums(this),set_verify_checksums(this),
		get_fill_cache(this),set_fill_cache(this),
		set_snapshot(this)

	{}
};


template<class T>
struct IWriteOptions
	:public cross_compiler_interface::define_interface_unknown<T,
	// {23ACDD39-9502-42A0-B01D-F65E12859663}
	cross_compiler_interface::uuid<
	0x23ACDD39,0x9502,0x42A0,0xB0,0x1D,0xF6,0x5E,0x12,0x85,0x96,0x63
	>
	>
{
	cross_function<IWriteOptions,0,bool()> get_sync;
	cross_function<IWriteOptions,1,void(bool)> set_sync;


	IWriteOptions()
		:get_sync(this),set_sync(this)
	{}
};

template<class T>
struct IHandler
	:public cross_compiler_interface::define_interface_unknown<T,
	// {549DA77F-2FC1-449F-9749-2E8F83F94BD1}
	cross_compiler_interface::uuid<
	0x549DA77F,0x2FC1,0x449F,0x97,0x49,0x2E,0x8F,0x83,0xF9,0x4B,0xD1
	>
	>
{
	cross_function<IHandler,0,void(Slice,Slice)> Put;
	cross_function<IHandler,1,void(Slice)> Delete;




	IHandler()
		:Put(this),Delete(this)
	{}
};


template<class T>
struct IWriteBatch
	:public cross_compiler_interface::define_interface_unknown<T,
	// {EF48CDBF-66E3-487C-921B-DD794BC56A03}
	cross_compiler_interface::uuid<
	0xEF48CDBF,0x66E3,0x487C,0x92,0x1B,0xDD,0x79,0x4B,0xC5,0x6A,0x03
	>
	>
{

		cross_function<IWriteBatch,0,void(Slice,Slice)> Put;
		cross_function<IWriteBatch,1,void(Slice)> Delete;
		cross_function<IWriteBatch,2,void()> Clear;
		cross_function<IWriteBatch,3,Status(use_unknown<IHandler>)> Iterate;


	IWriteBatch()
		:Put(this),Delete(this),Clear(this),Iterate(this)
	{}
};

template<class T>
struct IIterator
	:public cross_compiler_interface::define_interface_unknown<T,
	// {6F230C64-A115-4AF7-81E3-05D55B63980B}
	cross_compiler_interface::uuid<
	0x6F230C64,0xA115,0x4AF7,0x81,0xE3,0x05,0xD5,0x5B,0x63,0x98,0x0B
	>
	>
{
	cross_function<IIterator,0,bool()> Valid;
	cross_function<IIterator,1,void()> SeekToFirst;
	cross_function<IIterator,2,void()> SeekToLast;
	cross_function<IIterator,3,void(Slice)> Seek;
	cross_function<IIterator,4,void()> Next;
	cross_function<IIterator,5,void()> Prev;
	cross_function<IIterator,6,Slice()> key;
	cross_function<IIterator,7,Slice()> value;
	cross_function<IIterator,8,Status()> status;

	// No support for cleanup function


	IIterator()
		:Valid(this),SeekToFirst(this),SeekToLast(this),Seek(this),
		Next(this),Prev(this),key(this),value(this),status(this)
	{}
};

template<class T>
struct IDB
	:public cross_compiler_interface::define_interface_unknown<T,
	// {E94D09D0-CDB1-4AB3-BCAC-BDB2FC0B2A94}
	cross_compiler_interface::uuid<
	0xE94D09D0,0xCDB1,0x4AB3,0xBC,0xAC,0xBD,0xB2,0xFC,0x0B,0x2A,0x94
	>
	>
{

	cross_function<IDB,0,Status(use_unknown<IWriteOptions>,Slice,Slice)> Put;
	void PutValue(use_unknown<IWriteOptions> wo,Slice name,Slice value){
		Status s = Put(wo,name,value);
		if(!s.ok()){
			throw bad_status(s);
		}
	}
	cross_function<IDB,1,Status(use_unknown<IWriteOptions>,Slice)> Delete;
	void DeleteValue(use_unknown<IWriteOptions> wo,Slice name){
		Status s = Delete(wo,name);
		if(!s.ok()){
			throw bad_status(s);
		}
	}
	cross_function<IDB,2,Status(use_unknown<IWriteOptions>,
		use_unknown<IWriteBatch>)> Write;
	void WriteBatch(use_unknown<IWriteOptions> wo, use_unknown<IWriteBatch> wb){
		Status s = Write(wo,wb);
		if(!s.ok()){
			throw bad_status(s);
		}

	}
	cross_function<IDB,3,Status(use_unknown<IReadOptions>,
		Slice,out<std::string>)> Get;
	std::string GetValue(use_unknown<IReadOptions> ro,Slice name){
		std::string ret;
		Status s = Get(ro,name,&ret);
		if(!s.ok()){
			throw bad_status(s);
		}
		return ret;
	}

	cross_function<IDB,4,use_unknown<IIterator>(use_unknown<IReadOptions>)> NewIterator;

	cross_function<IDB,5,use_unknown<ISnapshot>()> GetSnapshot;
	cross_function<IDB,6,void(use_unknown<ISnapshot>)> ReleaseSnapshot;

	cross_function<IDB,7,bool(Slice,out<std::string>)> GetProperty;

	// Note change in function signature
	cross_function<IDB,8,std::vector<std::uint64_t>(std::vector<Range>)> GetApproximateSizes;

	// Note change in function signatures
	cross_function<IDB,9,void(Slice begin,Slice end)> CompactRange;

	cross_function<IDB,10,void()> CompactAll;



	IDB()
		:Put(this),Delete(this),Write(this),Get(this),NewIterator(this),
		GetSnapshot(this),ReleaseSnapshot(this),GetProperty(this),
		GetApproximateSizes(this),CompactRange(this),CompactAll(this)
	{}
};


template<class T>
struct ILevelDBStaticFunctions
	:public cross_compiler_interface::define_interface_unknown<T,
	// {989DCE03-99AE-4DB2-9D74-A6FAEFEC3628}
	cross_compiler_interface::uuid<
	0x989DCE03,0x99AE,0x4DB2,0x9D,0x74,0xA6,0xFA,0xEF,0xEC,0x36,0x28
	>
	>
{
	cross_function<ILevelDBStaticFunctions,0,Status
		(use_unknown<IOptions>,std::string name,out<use_unknown<IDB>>)> Open;
	use_unknown<IDB> OpenDB(use_unknown<IOptions> options,std::string name){
		use_unknown<IDB> ret;
			Status s = Open(options,name,&ret);
			if(!s.ok()){
				throw bad_status(s);
			}
			return ret;

	}
	cross_function<ILevelDBStaticFunctions,1,use_unknown<IOptions>()> CreateOptions;
	cross_function<ILevelDBStaticFunctions,2,use_unknown<IReadOptions>()> CreateReadOptions;
	cross_function<ILevelDBStaticFunctions,3,use_unknown<IWriteOptions>()> CreateWriteOptions;
	cross_function<ILevelDBStaticFunctions,4,use_unknown<IWriteBatch>()> CreateWriteBatch;
	cross_function<ILevelDBStaticFunctions,5,Status(std::string name,use_unknown<IOptions>)>
		DestroyDB;
	cross_function<ILevelDBStaticFunctions,6,Status(std::string name,use_unknown<IOptions>)>
		RepairDB;



	ILevelDBStaticFunctions()
		:Open(this),CreateOptions(this),CreateReadOptions(this),
		CreateWriteOptions(this),CreateWriteBatch(this), DestroyDB(this),RepairDB(this)
	{}
};

}