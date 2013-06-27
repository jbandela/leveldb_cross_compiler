//          Copyright John R. Bandela 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "leveldb/db.h"
#include "leveldb/comparator.h"
#include "leveldb/write_batch.h"
#include "leveldb/filter_policy.h"
#include "leveldb/cache.h"

#include "level_db_interfaces.h"

#include<memory>

struct IGetNative
{
    typedef
	cppcomponents::uuid<
	0xAAF3BE95,0xA1A8,0x4DCE,0x86,0x72,0xCE,0xF2,0x7C,0xD7,0xEE,0xC9
	>
	uuid;

    void* get_native();


	CPPCOMPONENTS_CONSTRUCT(IGetNative,get_native);
};

using namespace leveldb_cc;
using namespace cppcomponents;

inline std::string NotCreatable(){ return ""; }

typedef runtime_class<NotCreatable, ISnapshot, NoConstructorFactoryInterface, DefaultStaticInterface, IGetNative> SnapShot_t;

struct SnapShotImplementation:public implement_runtime_class<SnapShotImplementation, SnapShot_t>
{
	const leveldb::Snapshot* snapshot_;

	SnapShotImplementation(const leveldb::Snapshot* s):snapshot_(s){
		get_implementation<IGetNative>()->get_native = [this]()->void*{
			return const_cast<leveldb::Snapshot*>(snapshot_);
		};
	}


};

struct ComparatorFromIComparator:public leveldb::Comparator{
	use<IComparator> ic_;

	ComparatorFromIComparator(use<IComparator> ic):ic_(ic){}

	int Compare(const leveldb::Slice& a, const leveldb::Slice& b)const override{
		Slice sa(a.data(),a.size());
		Slice sb(b.data(),b.size());
		return ic_.Compare(sa,sb);

	}

	const char* Name() const override{
		return ic_.Name();

	}

	// Advanced functions: these are used to reduce the space requirements
	// for internal data structures like index blocks.

	// If *start < limit, changes *start to a short string in [start,limit).
	// Simple comparator implementations may return with *start unchanged,
	// i.e., an implementation of this method that does nothing is correct.
	virtual void FindShortestSeparator(
		std::string* start,
		const leveldb::Slice& limit) const override{
			Slice s(limit.data(),limit.size());
			*start = ic_.FindShortestSeparator(s);


	};

	// Changes *key to a short string >= *key.
	// Simple comparator implementations may return with *key unchanged,
	// i.e., an implementation of this method that does nothing is correct.
	virtual void FindShortSuccessor(std::string* key) const override{
		*key = ic_.FindShortSuccessor(*key);

	}


};
typedef cppcomponents::runtime_class<LRUCacheComponentName, ICache, LRUCacheFactoryInterface, LRUCacheStaticInterface,IGetNative>
	LRUCachePrivate_t;

struct LRUCacheImplementation
	:public implement_runtime_class<LRUCacheImplementation,LRUCachePrivate_t>
{
	std::unique_ptr<leveldb::Cache> cache_;

	LRUCacheImplementation(std::uint64_t capacity) : cache_(leveldb::NewLRUCache(static_cast<std::size_t>(capacity))){
		get_implementation<IGetNative>()->get_native = 
			[this]()->void*{return cache_.get();};

	}


};
typedef cppcomponents::runtime_class<BloomFilterComponentName, IFilterPolicy, BloomFilterFactoryInterface, BloomFilterStaticInterface,IGetNative>
	BloomFilterPrivate_t;

struct BloomFilterPolicyImplementation
	:public implement_runtime_class<BloomFilterPolicyImplementation,BloomFilterPrivate_t>
{
	std::unique_ptr<const leveldb::FilterPolicy> filter_;

	BloomFilterPolicyImplementation(std::int32_t bits) : filter_(leveldb::NewBloomFilterPolicy(bits)){
		get_implementation<IGetNative>()->get_native = 
			[this]()->void*{return const_cast<leveldb::FilterPolicy*>(filter_.get());};

	}

};
typedef cppcomponents::runtime_class<OptionsComponentName, IOptions, cppcomponents::DefaultFactoryInterface, cppcomponents::DefaultStaticInterface, IGetNative> OptionsPrivate_t;

struct OptionsImplementation
	: public implement_runtime_class<OptionsImplementation, OptionsPrivate_t>
{
	leveldb::Options options_;

		bool get_create_if_missing (){
			return options_.create_if_missing;
		}

		void set_create_if_missing(bool b){
			options_.create_if_missing = b;

		}

		bool get_error_if_exists(){
			return options_.error_if_exists;
		}		

		void set_error_if_exists(bool b){
			options_.error_if_exists = b;

		};
		bool get_paranoid_checks(){
			return options_.paranoid_checks;
		};

		void set_paranoid_checks(bool b){
			options_.paranoid_checks = b;

		};
		auto get_write_buffer_size()->std::uint32_t{
			return options_.write_buffer_size;
		};

		void set_write_buffer_size(std::uint32_t s){
			options_.write_buffer_size = s;

		};
		auto get_max_open_files()->std::uint32_t{
			return options_.max_open_files;
		};

		void set_max_open_files(std::uint32_t n){
			options_.max_open_files = n;

		};

		auto get_block_size()->std::uint32_t{
			return options_.block_size;
		};

		void set_block_size(std::uint32_t n){
			options_.block_size = n;

		};
		auto get_block_restart_interval()->std::uint32_t{
			return options_.block_restart_interval;
		};

		void set_block_restart_interval(std::uint32_t n){
			options_.block_restart_interval = n;

		};
		auto get_compression()->std::uint32_t{
			return options_.compression;
		};

		void set_compression(std::uint32_t n){
			options_.block_restart_interval = n;

		};

		void set_comparator(use<IComparator> ic){
			if(!ic){
				throw cross_compiler_interface::error_pointer();
				//options_.comparator = nullptr;
			}else{
				options_.comparator = new ComparatorFromIComparator(ic);
			}
		};

		void set_block_cache(use<ICache> ic){
			if(!ic){
				options_.block_cache = nullptr;
			}else{
			options_.block_cache = static_cast<leveldb::Cache*>(
				ic.QueryInterface<IGetNative>().get_native());
			}
		};
		void set_filter_policy(use<IFilterPolicy> ic){
			if(!ic){
				options_.filter_policy = nullptr;
			}else{
			options_.filter_policy = static_cast<leveldb::FilterPolicy*>(
				ic.QueryInterface<IGetNative>().get_native());
			}
		};


		
		OptionsImplementation(){

			get_implementation<IGetNative>()->get_native = [this]()->void*{
				return &options_;
			};




	}




};


typedef cppcomponents::runtime_class<ReadOptionsComponentName, IReadOptions, cppcomponents::DefaultFactoryInterface,
	cppcomponents::DefaultStaticInterface,IGetNative> ReadOptionsPrivate_t;

struct ReadOptionsImplementation
	: public implement_runtime_class<ReadOptionsImplementation, ReadOptionsPrivate_t>
{
	leveldb::ReadOptions options_;

		bool get_verify_checksums(){
			return options_.verify_checksums;
		};
		void set_verify_checksums(bool b){
			options_.verify_checksums = b;
		};

		bool get_fill_cache(){ return options_.fill_cache; }
		void set_fill_cache(bool b){ options_.fill_cache = b; }

		void set_snapshot(use<ISnapshot> s){
			if(!s){
				options_.snapshot = nullptr;
			}else{
				options_.snapshot = static_cast<leveldb::Snapshot*>(s.QueryInterface<IGetNative>()
					.get_native());
			}

		};

	ReadOptionsImplementation(){

		get_implementation<IGetNative>()->get_native = 
			[this](){return &options_;};

	};


};
typedef cppcomponents::runtime_class<WriteOptionsComponentName, IWriteOptions, cppcomponents::DefaultFactoryInterface,
	cppcomponents::DefaultStaticInterface, IGetNative> WriteOptionsPrivate_t;

struct WriteOptionsImplementation
	: public implement_runtime_class<WriteOptionsImplementation,
	WriteOptionsPrivate_t>
{
	leveldb::WriteOptions options_;

	bool get_sync(){

		return options_.sync;
	};
	void set_sync(bool b){
		options_.sync = b;
	};

	WriteOptionsImplementation(){
		get_implementation<IGetNative>()->get_native = [this]()->void*{
			return &options_;

		};
	}


};

struct HandlerFromIHandler:public leveldb::WriteBatch::Handler{
	use<IHandler> handler_;

	virtual void Put(const leveldb::Slice& key, const leveldb::Slice& value)override{
		Slice skey(key.data(),key.size());
		Slice svalue(value.data(),value.size());
		handler_.Put(skey,svalue);

	}
	virtual void Delete(const leveldb::Slice& key)override{
		Slice skey(key.data(),key.size());
		handler_.Delete(skey);

	}

	HandlerFromIHandler(use<IHandler> h)
		:handler_(h){}


};


Status StatusFromLevelDBStatus(leveldb::Status& s){
	// On Purpose we made our Status the same as leveldb::Status
	// So we cast
	return *reinterpret_cast<Status*>(&s);

}

typedef cppcomponents::runtime_class<WriteBatchComponentName, IWriteBatch, cppcomponents::DefaultFactoryInterface, 
	cppcomponents::DefaultStaticInterface, IGetNative> WriteBatchPrivate_t;

struct WriteBatchImplementation
	:public implement_runtime_class<WriteBatchImplementation,
	WriteBatchPrivate_t>
{
	leveldb::WriteBatch wb_;

	void Put(Slice key, Slice value){
		wb_.Put(leveldb::Slice(key.data(), key.size()),
			leveldb::Slice(value.data(), value.size()));

	};

	void Delete(Slice key){
		wb_.Delete(leveldb::Slice(key.data(), key.size()));

	};

	void Clear(){
		wb_.Clear();

	};
	auto Iterate(use<IHandler> h)->Status{
		leveldb::Status s = wb_.Iterate(new HandlerFromIHandler(h));
		return StatusFromLevelDBStatus(s);

	};

	WriteBatchImplementation(){


		get_implementation<IGetNative>()->get_native = 
			[this]()->void*{ return &wb_;};
	}



};

typedef runtime_class<NotCreatable, IIterator, NoConstructorFactoryInterface, DefaultStaticInterface> IteratorPrivate_t;

struct IteratorImplementation
	:public implement_runtime_class<IteratorImplementation,IteratorPrivate_t>{
		std::unique_ptr<leveldb::Iterator> iter_;

		auto Valid()->bool{return iter_->Valid(); };

		void SeekToFirst(){iter_->SeekToFirst(); };

		void SeekToLast(){iter_->SeekToLast(); };

		void Seek(Slice key){ iter_->Seek(leveldb::Slice(
			key.data(), key.size()));
		};

		void Next(){ iter_->Next(); };

		void Prev(){ iter_->Prev(); };

		auto key()->Slice{
			auto s = iter_->key();
			return Slice(s.data(), s.size());
		};

		auto value()->Slice{
			auto s = iter_->value();
			return Slice(s.data(), s.size());
		};

		auto status()->Status{
			auto s = iter_->status();
			return StatusFromLevelDBStatus(s);
		};

		IteratorImplementation(leveldb::Iterator* iter):iter_(iter){};


};


struct DBImplementation:public implement_runtime_class<DBImplementation,
	DB_t>
{
	std::unique_ptr<leveldb::DB> db_;

	auto Put(use<IWriteOptions> wo, Slice key, Slice value)
		->Status{
			auto s = db_->Put(*static_cast<leveldb::WriteOptions*>(wo
				.QueryInterface<IGetNative>().get_native()),
				leveldb::Slice(key.data(), key.size()),
				leveldb::Slice(value.data(), value.size()));
			return StatusFromLevelDBStatus(s);

	};

	auto Delete(use<IWriteOptions> wo, Slice key)
		->Status{
			auto s = db_->Delete(*static_cast<leveldb::WriteOptions*>(wo
				.QueryInterface<IGetNative>().get_native()),
				leveldb::Slice(key.data(), key.size()));
			return StatusFromLevelDBStatus(s);

	};

	auto Write(use<IWriteOptions> wo,
		use<IWriteBatch> wb)
		->Status{
			auto s = db_->Write(*static_cast<leveldb::WriteOptions*>(wo
				.QueryInterface<IGetNative>().get_native()),
				static_cast<leveldb::WriteBatch*>(wb
				.QueryInterface<IGetNative>().get_native()));
			return StatusFromLevelDBStatus(s);

	};


	auto Get(use<IReadOptions> ro,
		Slice key, out<std::string> oret)->Status
	{
		std::string value;

		auto s = db_->Get(*static_cast<leveldb::ReadOptions*>(ro
			.QueryInterface<IGetNative>().get_native()),
			leveldb::Slice(key.data(), key.size()),
			&value);
		oret.set(value);
		return StatusFromLevelDBStatus(s);

	};

	auto NewIterator(use<IReadOptions> ro)->use<IIterator>{
		return IteratorImplementation::create(db_->NewIterator(
			*static_cast<leveldb::ReadOptions*>(ro
			.QueryInterface<IGetNative>().get_native())))
			.QueryInterface<IIterator>();

	};

	auto GetSnapshot()->use<ISnapshot>{
		return SnapShotImplementation::create(db_->GetSnapshot())
			.QueryInterface<ISnapshot>();

	};

	void ReleaseSnapshot(use<ISnapshot> s){
		db_->ReleaseSnapshot(static_cast<leveldb::Snapshot*>(s
			.QueryInterface<IGetNative>().get_native()));

	};

	bool GetProperty(Slice prop, out<std::string> oret){
		std::string value;
		auto b = db_->GetProperty(leveldb::Slice(prop.data(), prop.size()),
			&value);
		oret.set(value);
		return b;
	};

	auto GetApproximateSizes(std::vector<Range> v)->std::vector<std::uint64_t>{
		std::vector<std::uint64_t> ret(v.size());
		std::vector<leveldb::Range> ranges;
		for (auto& r : v){
			ranges.push_back(leveldb::Range(

				leveldb::Slice(r.first.data(), r.first.size()),
				leveldb::Slice(r.second.data(), r.second.size())));

		}

		db_->GetApproximateSizes(&ranges[0], ranges.size(), &ret[0]);
		return ret;


	};

	void CompactRange(Slice begin, Slice end){
		leveldb::Slice b(begin.data(), begin.size());
		leveldb::Slice e(end.data(), end.size());
		db_->CompactRange(&b, &e);

	};

	void CompactAll(){
		db_->CompactRange(nullptr, nullptr);
	};



	DBImplementation(leveldb::DB* db):db_(db){

		auto imp = get_implementation<IDB>();

	}

	DBImplementation(use<IOptions> options, std::string name){
		leveldb::DB* db = 0;
		auto s = leveldb::DB::Open(*static_cast<leveldb::Options*>(options
			.QueryInterface<IGetNative>().get_native()),
			name, &db);
		db_.reset(db);
		if (!s.ok()){
			throw std::runtime_error("Unable to open file");

		}

	};


	static Status Open(use<IOptions> options, std::string name, out < use < IDB >> oret){
		leveldb::DB* db = 0;
		auto s = leveldb::DB::Open(*static_cast<leveldb::Options*>(options
			.QueryInterface<IGetNative>().get_native()),
			name, &db);
		oret.set(DBImplementation::create(db).QueryInterface<IDB>());
		return StatusFromLevelDBStatus(s);

	};


	static Status DestroyDB(std::string name, use<IOptions> options){
		auto s = leveldb::DestroyDB(name, *static_cast<leveldb::Options*>(options
			.QueryInterface<IGetNative>().get_native()));
		return StatusFromLevelDBStatus(s);
	};

	static Status RepairDB(std::string name, use<IOptions> options){
		auto s = leveldb::RepairDB(name, *static_cast<leveldb::Options*>(options
			.QueryInterface<IGetNative>().get_native()));
		return StatusFromLevelDBStatus(s);
	};

	static std::pair<int,int> GetVersion(){
		return std::make_pair(leveldb::kMajorVersion, leveldb::kMinorVersion);

	}



};





extern "C"{

	cppcomponents::error_code CPPCOMPONENTS_CALLING_CONVENTION get_cppcomponents_factory(const char* s,
		cppcomponents::portable_base** p){
			typedef cross_compiler_interface::type_list<BloomFilterPolicyImplementation,DBImplementation,LRUCacheImplementation,OptionsImplementation,
			ReadOptionsImplementation,WriteBatchImplementation,WriteOptionsImplementation> t;
			return cppcomponents::get_activation_factory(t(), s, p);
	}
}