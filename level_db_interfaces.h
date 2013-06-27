//          Copyright John R. Bandela 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../cross_compiler_call/cross_compiler_interface/cross_compiler_component.hpp"
#include "slice.h"
#include "status.h"

using cross_compiler_interface::out;
using cppcomponents::use;

namespace leveldb_cc{

	class bad_status : public std::runtime_error{
		Status s_;
	public:
		bad_status(const Status& s) : std::runtime_error(s.ToString()), s_(s){}

		const Status& status(){
			return s_;
		};
	};

	enum CompressionType {
		// NOTE: do not change the values of existing entries, as these are
		// part of the persistent format on disk.
		kNoCompression = 0x0,
		kSnappyCompression = 0x1
	};

	struct ISnapshot
	{
		// {D6F3D379-9E73-407F-846E-CCAFA1A3EA6E}
		typedef cppcomponents::uuid <
			0xD6F3D379, 0x9E73, 0x407F, 0x84, 0x6E, 0xCC, 0xAF, 0xA1, 0xA3, 0xEA, 0x6E
		> uuid;
		CPPCOMPONENTS_CONSTRUCT_NO_METHODS(ISnapshot);

	};

	struct IComparator
	{
		// {C5B46BB4-7CDC-41E9-BCC7-3EBAF62C8873}
		typedef cross_compiler_interface::uuid <
			0xC5B46BB4, 0x7CDC, 0x41E9, 0xBC, 0xC7, 0x3E, 0xBA, 0xF6, 0x2C, 0x88, 0x73
		> uuid;

		std::int32_t Compare(Slice, Slice);
		const char* Name();
		std::string FindShortestSeparator(Slice);
		std::string FindShortSuccessor(std::string);



		CPPCOMPONENTS_CONSTRUCT(IComparator, Compare, Name, FindShortestSeparator, FindShortSuccessor);
	};




	// Only support LRUCache for now
	struct ICache{
		// {1AF4C611-D729-4B15-91C0-D619FDDD4D85}
		typedef cross_compiler_interface::uuid <
			0x1AF4C611, 0xD729, 0x4B15, 0x91, 0xC0, 0xD6, 0x19, 0xFD, 0xDD, 0x4D, 0x85
		> uuid;


		CPPCOMPONENTS_CONSTRUCT_NO_METHODS(ICache);
	};


	struct LRUCacheFactoryInterface{
		// {E4FC8445-D492-4BF6-8EA5-8B0C43CFD414}
		typedef cppcomponents::uuid <
			0xE4FC8445, 0xD492, 0x4BF6, 0x8E, 0xA5, 0x8B, 0x0C, 0x43, 0xCF, 0xD4, 0x14
		> uuid;

		cppcomponents::use<cppcomponents::InterfaceUnknown> Create(std::uint64_t);



		CPPCOMPONENTS_CONSTRUCT(LRUCacheFactoryInterface, Create);


	};
	typedef cppcomponents::DefaultStaticInterface LRUCacheStaticInterface;

	inline std::string LRUCacheComponentName(){ return "leveldb_cc_dll!LRUCache"; }

	typedef cppcomponents::runtime_class<LRUCacheComponentName, ICache, LRUCacheFactoryInterface, LRUCacheStaticInterface>
		LRUCache_t;
	typedef cppcomponents::use_runtime_class<LRUCache_t> LRUCache;


	// Only support bloom filter
	struct IFilterPolicy{
		// {46559B7D-60E6-4D41-8B41-4262DD6F739C}
		typedef cross_compiler_interface::uuid <
			0x46559B7D, 0x60E6, 0x4D41, 0x8B, 0x41, 0x42, 0x62, 0xDD, 0x6F, 0x73, 0x9C
		>
		uuid;


		CPPCOMPONENTS_CONSTRUCT_NO_METHODS(IFilterPolicy);
	};

	struct BloomFilterFactoryInterface{
		// {01655F1E-F594-41FD-B668-24EF046A7DF6}
		typedef cppcomponents::uuid <
			0x01655F1E, 0xF594, 0x41FD, 0xB6, 0x68, 0x24, 0xEF, 0x04, 0x6A, 0x7D, 0xF6
		> uuid;

		cppcomponents::use<cppcomponents::InterfaceUnknown> Create(std::int32_t);



		CPPCOMPONENTS_CONSTRUCT(BloomFilterFactoryInterface, Create);


	};
	typedef cppcomponents::DefaultStaticInterface BloomFilterStaticInterface;

	inline std::string BloomFilterComponentName(){ return "leveldb_cc_dll!BloomFilter"; }

	typedef cppcomponents::runtime_class<BloomFilterComponentName, IFilterPolicy, BloomFilterFactoryInterface, BloomFilterStaticInterface>
		BloomFilter_t;
	typedef cppcomponents::use_runtime_class<BloomFilter_t> BloomFilter;


	struct IOptions{
		// {FC12AA30-D42B-48FB-9A93-3716D962BFB7}
		typedef cross_compiler_interface::uuid <
			0xFC12AA30, 0xD42B, 0x48FB, 0x9A, 0x93, 0x37, 0x16, 0xD9, 0x62, 0xBF, 0xB7
		>
		uuid;
		bool get_create_if_missing();
		void set_create_if_missing(bool);

		bool get_error_if_exists();
		void set_error_if_exists(bool);

		bool get_paranoid_checks();
		void set_paranoid_checks(bool);

		std::uint32_t get_write_buffer_size();
		void set_write_buffer_size(std::uint32_t);

		std::uint32_t get_max_open_files();
		void set_max_open_files(std::uint32_t);


		std::uint32_t get_block_size();
		void set_block_size(std::uint32_t);

		std::uint32_t get_block_restart_interval();
		void set_block_restart_interval(std::uint32_t);

		std::uint32_t get_compression();
		void set_compression(std::uint32_t);

		void set_comparator(use<IComparator>);

		void set_block_cache(use<ICache>);

		void set_filter_policy(use<IFilterPolicy>);


		CPPCOMPONENTS_CONSTRUCT(IOptions,
			get_create_if_missing, set_create_if_missing, get_error_if_exists,
			set_error_if_exists, get_paranoid_checks, set_paranoid_checks,

			get_write_buffer_size, set_write_buffer_size, get_max_open_files,
			set_max_open_files,
			get_block_size, set_block_size, get_block_restart_interval,
			set_block_restart_interval, get_compression, set_compression,
			set_comparator, set_block_cache,
			set_filter_policy);
	};

	std::string OptionsComponentName(){ return "leveldb_cc_dll!Options"; }

	typedef cppcomponents::runtime_class<OptionsComponentName, IOptions, cppcomponents::DefaultFactoryInterface, cppcomponents::DefaultStaticInterface> Options_t;
	typedef cppcomponents::use_runtime_class<Options_t> Options;


	struct IReadOptions{
		// {644668DE-09EC-49B1-A715-AD0D16CD8259}
		typedef cross_compiler_interface::uuid <
			0x644668DE, 0x09EC, 0x49B1, 0xA7, 0x15, 0xAD, 0x0D, 0x16, 0xCD, 0x82, 0x59
		>
		uuid;
		bool get_verify_checksums();
		void set_verify_checksums(bool);

		bool get_fill_cache();
		void set_fill_cache(bool);

		void set_snapshot(use<ISnapshot>);


		CPPCOMPONENTS_CONSTRUCT(IReadOptions,
			get_verify_checksums, set_verify_checksums,
			get_fill_cache, set_fill_cache,
			set_snapshot);
	};
	std::string ReadOptionsComponentName(){ return "leveldb_cc_dll!ReadOptions"; }

	typedef cppcomponents::runtime_class<ReadOptionsComponentName, IReadOptions, cppcomponents::DefaultFactoryInterface, cppcomponents::DefaultStaticInterface> ReadOptions_t;
	typedef cppcomponents::use_runtime_class<ReadOptions_t> ReadOptions;


	struct IWriteOptions{
		// {23ACDD39-9502-42A0-B01D-F65E12859663}
		typedef cross_compiler_interface::uuid <
			0x23ACDD39, 0x9502, 0x42A0, 0xB0, 0x1D, 0xF6, 0x5E, 0x12, 0x85, 0x96, 0x63
		>
		uuid;
		bool get_sync();
		void set_sync(bool);


		CPPCOMPONENTS_CONSTRUCT(IWriteOptions,
			get_sync, set_sync);
	};

	std::string WriteOptionsComponentName(){ return "leveldb_cc_dll!WriteOptions"; }

	typedef cppcomponents::runtime_class<WriteOptionsComponentName, IWriteOptions, cppcomponents::DefaultFactoryInterface, cppcomponents::DefaultStaticInterface> WriteOptions_t;
	typedef cppcomponents::use_runtime_class<WriteOptions_t> WriteOptions;

	struct IHandler{
		// {549DA77F-2FC1-449F-9749-2E8F83F94BD1}
		typedef cross_compiler_interface::uuid <
			0x549DA77F, 0x2FC1, 0x449F, 0x97, 0x49, 0x2E, 0x8F, 0x83, 0xF9, 0x4B, 0xD1
		>
		uuid;
		void Put(Slice, Slice);
		void Delete(Slice);




		CPPCOMPONENTS_CONSTRUCT(IHandler
			, Put, Delete);
	};


	struct IWriteBatch{
		// {EF48CDBF-66E3-487C-921B-DD794BC56A03}
		typedef cross_compiler_interface::uuid <
			0xEF48CDBF, 0x66E3, 0x487C, 0x92, 0x1B, 0xDD, 0x79, 0x4B, 0xC5, 0x6A, 0x03
		>
		uuid;

		void Put(Slice, Slice);
		void Delete(Slice);
		void Clear();
		Status Iterate(use<IHandler>);


		CPPCOMPONENTS_CONSTRUCT(IWriteBatch
			, Put, Delete, Clear, Iterate);
	};
	std::string WriteBatchComponentName(){ return "leveldb_cc_dll!WriteBatch"; }

	typedef cppcomponents::runtime_class<WriteBatchComponentName, IWriteBatch, cppcomponents::DefaultFactoryInterface, cppcomponents::DefaultStaticInterface> WriteBatch_t;
	typedef cppcomponents::use_runtime_class<WriteBatch_t> WriteBatch;

	struct IIterator{
		// {6F230C64-A115-4AF7-81E3-05D55B63980B}
		typedef cross_compiler_interface::uuid <
			0x6F230C64, 0xA115, 0x4AF7, 0x81, 0xE3, 0x05, 0xD5, 0x5B, 0x63, 0x98, 0x0B
		>
		uuid;
		bool Valid();
		void SeekToFirst();
		void SeekToLast();
		void Seek(Slice);
		void Next();
		void Prev();
		Slice key();
		Slice value();
		Status status();

		// No support for cleanup function


		CPPCOMPONENTS_CONSTRUCT(IIterator
			, Valid, SeekToFirst, SeekToLast, Seek,
			Next, Prev, key, value, status);
	};

	struct IDB_Base{
		// {E94D09D0-CDB1-4AB3-BCAC-BDB2FC0B2A94}
		typedef cross_compiler_interface::uuid <
			0xE94D09D0, 0xCDB1, 0x4AB3, 0xBC, 0xAC, 0xBD, 0xB2, 0xFC, 0x0B, 0x2A, 0x94
		>
		uuid;

		Status Put(use<IWriteOptions>, Slice, Slice);
		Status Delete(use<IWriteOptions>, Slice);
		Status Write(use<IWriteOptions>, use<IWriteBatch>);

		Status Get(use<IReadOptions>, Slice, out<std::string>);


		use<IIterator> NewIterator(use<IReadOptions>);

		use<ISnapshot> GetSnapshot();
		void ReleaseSnapshot(use<ISnapshot>);

		bool GetProperty(Slice, out<std::string>);

		// Note change in function signature
		std::vector<std::uint64_t> GetApproximateSizes(std::vector<Range>);

		// Note change in function signatures
		void CompactRange(Slice begin, Slice end);

		void CompactAll();



		CPPCOMPONENTS_CONSTRUCT(IDB_Base
			, Put, Delete, Write, Get, NewIterator,
			GetSnapshot, ReleaseSnapshot, GetProperty,
			GetApproximateSizes, CompactRange, CompactAll);
	};

	struct IDB{

		template<class T>
		struct Interface : public IDB_Base::Interface<T>{
			void PutValue(use<IWriteOptions> wo, Slice name, Slice value){
				Status s = this->Put(wo, name, value);
				if (!s.ok()){
					throw bad_status(s);
				}
			}

			void DeleteValue(use<IWriteOptions> wo, Slice name){
				Status s = this->Delete(wo, name);
				if (!s.ok()){
					throw bad_status(s);
				}
			}

			void WriteBatch(use<IWriteOptions> wo, use<IWriteBatch> wb){
				Status s = this->Write(wo, wb);
				if (!s.ok()){
					throw bad_status(s);
				}

			}
			std::string GetValue(use<IReadOptions> ro, Slice name){
				std::string ret;
				Status s = this->Get(ro, name, &ret);
				if (!s.ok()){
					throw bad_status(s);
				}
				return ret;
			}

		};

	};

	struct DBFactoryInterface{
		// {06BD1200-1F74-4232-B2AF-957B032679EF}
		typedef cppcomponents::uuid <
			0x06BD1200, 0x1F74, 0x4232, 0xB2, 0xAF, 0x95, 0x7B, 0x03, 0x26, 0x79, 0xEF
		> uuid;

		use<cppcomponents::InterfaceUnknown> Create(use<IOptions>, std::string name);





		CPPCOMPONENTS_CONSTRUCT(DBFactoryInterface, Create);


	};
	struct DBStaticInterface{
		// {56CFF5CF-672A-4C00-8BDC-DE55C60C5DE1}
		typedef cppcomponents::uuid <
			0x56CFF5CF, 0x672A, 0x4C00, 0x8B, 0xDC, 0xDE, 0x55, 0xC6, 0x0C, 0x5D, 0xE1
		> uuid;

		Status Open(use<IOptions>, std::string name, out < use < IDB >> );

		Status DestroyDB(std::string name, use<IOptions>);
		Status RepairDB(std::string name, use<IOptions>);
		std::pair<int,int> GetVersion();



		CPPCOMPONENTS_CONSTRUCT(DBStaticInterface, Open, DestroyDB, RepairDB,GetVersion);


	};
	inline std::string DBComponentName(){ return "leveldb_cc_dll!DB"; }

	typedef cppcomponents::runtime_class<DBComponentName, IDB, DBFactoryInterface, DBStaticInterface>
		DB_t;
	typedef cppcomponents::use_runtime_class<DB_t> DB;



}