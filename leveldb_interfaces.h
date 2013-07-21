//          Copyright John R. Bandela 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cppcomponents/cppcomponents.hpp>
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

	struct ISnapshot : cppcomponents::define_interface<cppcomponents::uuid<0xD6F3D379, 0x9E73, 0x407F, 0x846E, 0xCCAFA1A3EA6E>>
	{

		CPPCOMPONENTS_CONSTRUCT_NO_METHODS(ISnapshot);

	};

	struct IComparator : cppcomponents::define_interface < cppcomponents::uuid< 0xC5B46BB4, 0x7CDC, 0x41E9, 0xBCC7, 0x3EBAF62C8873>>
	{
			

		std::int32_t Compare(Slice, Slice);
		const char* Name();
		std::string FindShortestSeparator(Slice);
		std::string FindShortSuccessor(std::string);



		CPPCOMPONENTS_CONSTRUCT(IComparator, Compare, Name, FindShortestSeparator, FindShortSuccessor);
	};




	// Only support LRUCache for now
	struct ICache : cppcomponents::define_interface <cppcomponents::uuid< 0x1AF4C611, 0xD729, 0x4B15, 0x91C0, 0xD619FDDD4D85>>{



		CPPCOMPONENTS_CONSTRUCT_NO_METHODS(ICache);
	};


	struct LRUCacheFactoryInterface : cppcomponents::define_interface <cppcomponents::uuid< 0xE4FC8445, 0xD492, 0x4BF6, 0x8EA5, 0x8B0C43CFD414>>{


		cppcomponents::use<cppcomponents::InterfaceUnknown> Create(std::uint64_t);



		CPPCOMPONENTS_CONSTRUCT(LRUCacheFactoryInterface, Create);


	};


	inline std::string LRUCacheComponentName(){ return "leveldb_cc_dll!LRUCache"; }

	typedef cppcomponents::runtime_class<LRUCacheComponentName, cppcomponents::object_interfaces<ICache>,
		cppcomponents::factory_interface<LRUCacheFactoryInterface>>
		LRUCache_t;
	typedef cppcomponents::use_runtime_class<LRUCache_t> LRUCache;


	// Only support bloom filter
	struct IFilterPolicy : cppcomponents::define_interface<cppcomponents::uuid<0x46559B7D, 0x60E6, 0x4D41, 0x8B41, 0x4262DD6F739C>>{


		CPPCOMPONENTS_CONSTRUCT_NO_METHODS(IFilterPolicy);
	};

	struct BloomFilterFactoryInterface:cppcomponents::define_interface<cppcomponents::uuid<	0x01655F1E, 0xF594, 0x41FD, 0xB668, 0x24EF046A7DF6>>{	

		cppcomponents::use<cppcomponents::InterfaceUnknown> Create(std::int32_t);

		CPPCOMPONENTS_CONSTRUCT(BloomFilterFactoryInterface, Create);


	};

	inline std::string BloomFilterComponentName(){ return "leveldb_cc_dll!BloomFilter"; }

	typedef cppcomponents::runtime_class<BloomFilterComponentName, cppcomponents::object_interfaces<IFilterPolicy>,
		cppcomponents::factory_interface<BloomFilterFactoryInterface>>
		BloomFilter_t;
	typedef cppcomponents::use_runtime_class<BloomFilter_t> BloomFilter;


	struct IOptions:cppcomponents::define_interface<cppcomponents::uuid<0xFC12AA30, 0xD42B, 0x48FB, 0x9A93, 0x3716D962BFB7>>{	
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


		template<class T>
		struct InterfaceExtras : public InterfaceExtrasBase<T>{
			cppcomponents::property < T, decltype(Interface<T>::get_create_if_missing), decltype(Interface<T>::set_create_if_missing)> CreateIfMissing;
			cppcomponents::property < T, decltype(Interface<T>::get_error_if_exists), decltype(Interface<T>::set_error_if_exists)> ErrorIfExists;
			cppcomponents::property < T, decltype(Interface<T>::get_paranoid_checks), decltype(Interface<T>::set_paranoid_checks)> ParanoidChecks;
			cppcomponents::property < T, decltype(Interface<T>::get_write_buffer_size), decltype(Interface<T>::set_write_buffer_size)> WriteBufferSize;
			cppcomponents::property < T, decltype(Interface<T>::get_max_open_files), decltype(Interface<T>::set_max_open_files)> MaxOpenFiles;
			cppcomponents::property < T, decltype(Interface<T>::get_block_size), decltype(Interface<T>::set_block_size)> BlockSize;
			cppcomponents::property < T, decltype(Interface<T>::get_block_restart_interval), decltype(Interface<T>::set_block_restart_interval)> BlockRestartInterval;
			cppcomponents::property < T, decltype(Interface<T>::get_compression), decltype(Interface<T>::set_compression)> Compression;
			cppcomponents::write_only_property < T, decltype(Interface<T>::set_comparator)> Comparator;
			cppcomponents::write_only_property < T, decltype(Interface<T>::set_block_cache)> BlockCache;
			cppcomponents::write_only_property < T, decltype(Interface<T>::set_filter_policy)> FilterPolicy;

			InterfaceExtras() : CreateIfMissing(this),ErrorIfExists(this),ParanoidChecks(this),WriteBufferSize(this),
			MaxOpenFiles(this),BlockSize(this),BlockRestartInterval(this),Compression(this),Comparator(this),BlockCache(this),FilterPolicy(this){}
		};
	};

	std::string OptionsComponentName(){ return "leveldb_cc_dll!Options"; }

	typedef cppcomponents::runtime_class<OptionsComponentName,cppcomponents::object_interfaces<IOptions>> Options_t;
	typedef cppcomponents::use_runtime_class<Options_t> Options;


	struct IReadOptions:cppcomponents::define_interface<cppcomponents::uuid<0x644668DE, 0x09EC, 0x49B1, 0xA715, 0xAD0D16CD8259>>
	{	
		bool get_verify_checksums();
		void set_verify_checksums(bool);

		bool get_fill_cache();
		void set_fill_cache(bool);

		void set_snapshot(use<ISnapshot>);


		CPPCOMPONENTS_CONSTRUCT(IReadOptions,
			get_verify_checksums, set_verify_checksums,
			get_fill_cache, set_fill_cache,
			set_snapshot);

		template<class T>
		struct InterfaceExtras : public InterfaceExtrasBase<T>{

			cppcomponents::property < T, decltype(Interface<T>::get_verify_checksums), decltype(Interface<T>::set_verify_checksums)> VerifyChecksums;
			cppcomponents::property < T, decltype(Interface<T>::get_fill_cache), decltype(Interface<T>::set_fill_cache)> FillCache;
			cppcomponents::write_only_property < T, decltype(Interface<T>::set_snapshot)> Snapshot;

			InterfaceExtras() : VerifyChecksums(this), FillCache(this), Snapshot(this){}

		};
	};
	std::string ReadOptionsComponentName(){ return "leveldb_cc_dll!ReadOptions"; }

	typedef cppcomponents::runtime_class<ReadOptionsComponentName, cppcomponents::object_interfaces<IReadOptions>> ReadOptions_t;
	typedef cppcomponents::use_runtime_class<ReadOptions_t> ReadOptions;


	struct IWriteOptions:cppcomponents::define_interface<cppcomponents::uuid<0x23ACDD39, 0x9502, 0x42A0, 0xB01D, 0xF65E12859663>>{
		bool get_sync();
		void set_sync(bool);


		CPPCOMPONENTS_CONSTRUCT(IWriteOptions,
			get_sync, set_sync);

		template<class T>
		struct InterfaceExtras : public InterfaceExtrasBase<T>{
			cppcomponents::property < T, decltype(Interface<T>::get_sync), decltype(Interface<T>::set_sync)> Sync;

			InterfaceExtras() : Sync(this){}
		};
	};

	std::string WriteOptionsComponentName(){ return "leveldb_cc_dll!WriteOptions"; }

	typedef cppcomponents::runtime_class<WriteOptionsComponentName, cppcomponents::object_interfaces<IWriteOptions>> WriteOptions_t;
	typedef cppcomponents::use_runtime_class<WriteOptions_t> WriteOptions;

	struct IHandler:cppcomponents::define_interface<
			cppcomponents::uuid<0x549DA77F, 0x2FC1, 0x449F, 0x9749, 0x2E8F83F94BD1>>{	
		void Delete(Slice);




		CPPCOMPONENTS_CONSTRUCT(IHandler
			, Put, Delete);
	};


	struct IWriteBatch:cppcomponents::define_interface<cppcomponents::uuid<0xEF48CDBF, 0x66E3, 0x487C, 0x921B, 0xDD794BC56A03>>{	
		void Put(Slice, Slice);
		void Delete(Slice);
		void Clear();
		Status Iterate(use<IHandler>);


		CPPCOMPONENTS_CONSTRUCT(IWriteBatch
			, Put, Delete, Clear, Iterate);
	};
	std::string WriteBatchComponentName(){ return "leveldb_cc_dll!WriteBatch"; }

	typedef cppcomponents::runtime_class<WriteBatchComponentName, cppcomponents::object_interfaces<IWriteBatch>> WriteBatch_t;
	typedef cppcomponents::use_runtime_class<WriteBatch_t> WriteBatch;

	struct IIterator:cppcomponents::define_interface<cppcomponents::uuid<0x6F230C64, 0xA115, 0x4AF7, 0x81E3, 0x05D55B63980B>>{
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

	struct IDB:cppcomponents::define_interface<
			cppcomponents::uuid<0xE94D09D0, 0xCDB1, 0x4AB3, 0xBCAC, 0xBDB2FC0B2A94>>{	
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



		CPPCOMPONENTS_CONSTRUCT(IDB
			, Put, Delete, Write, Get, NewIterator,
			GetSnapshot, ReleaseSnapshot, GetProperty,
			GetApproximateSizes, CompactRange, CompactAll);

		template<class T>
		struct InterfaceExtras : public InterfaceExtrasBase<T>{

			void PutValue(use<IWriteOptions> wo, Slice name, Slice value){
				Status s = this->get_interface().Put(wo, name, value);
				if (!s.ok()){
					throw bad_status(s);
				}
			}

			void DeleteValue(use<IWriteOptions> wo, Slice name){
				Status s = this->get_interface().Delete(wo, name);
				if (!s.ok()){
					throw bad_status(s);
				}
			}

			void WriteBatch(use<IWriteOptions> wo, use<IWriteBatch> wb){
				Status s = this->get_interface().Write(wo, wb);
				if (!s.ok()){
					throw bad_status(s);
				}

			}
			std::string GetValue(use<IReadOptions> ro, Slice name){
				std::string ret;
				Status s = this->get_interface().Get(ro, name, &ret);
				if (!s.ok()){
					throw bad_status(s);
				}
				return ret;
			}

		};
	};



	struct DBFactoryInterface:cppcomponents::define_interface<cppcomponents::uuid<0x06BD1200, 0x1F74, 0x4232, 0xB2AF, 0x957B032679EF>>{


		use<cppcomponents::InterfaceUnknown> Create(use<IOptions>, std::string name);





		CPPCOMPONENTS_CONSTRUCT(DBFactoryInterface, Create);


	};
	struct DBStaticInterface : cppcomponents::define_interface<cppcomponents::uuid<0x56CFF5CF, 0x672A, 0x4C00, 0x8BDC, 0xDE55C60C5DE1>>{


		Status Open(use<IOptions>, std::string name, out < use < IDB >> );

		Status DestroyDB(std::string name, use<IOptions>);
		Status RepairDB(std::string name, use<IOptions>);
		std::pair<int,int> GetVersion();



		CPPCOMPONENTS_CONSTRUCT(DBStaticInterface, Open, DestroyDB, RepairDB,GetVersion);


	};
	inline std::string DBComponentName(){ return "leveldb_cc_dll!DB"; }

	typedef cppcomponents::runtime_class<DBComponentName, cppcomponents::object_interfaces<IDB>,
		cppcomponents::factory_interface<DBFactoryInterface>,
		cppcomponents::static_interfaces<DBStaticInterface>>
		DB_t;
	typedef cppcomponents::use_runtime_class<DB_t> DB;



}