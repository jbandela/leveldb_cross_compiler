
#include "leveldb\db.h"
#include "leveldb\comparator.h"
#include "leveldb\write_batch.h"
#include "leveldb\filter_policy.h"
#include "leveldb\cache.h"

#include "level_db_interfaces.h"

#include<memory>


#if defined _DEBUG

#pragma comment(lib,"..\\..\\leveldbwin\\build\\msvc10\\_exports\\Debug\\leveldb_d.lib")

#else

#pragma comment(lib,"..\\..\\leveldbwin\\build\\msvc10\\_exports\\Release\\leveldb.lib")

#endif

// An interface to get the native representation
template<class T>
struct IGetNative
	:public cross_compiler_interface::define_interface_unknown<T,
	// {AAF3BE95-A1A8-4DCE-8672-CEF27CD7EEC9}
	cross_compiler_interface::uuid<
	0xAAF3BE95,0xA1A8,0x4DCE,0x86,0x72,0xCE,0xF2,0x7C,0xD7,0xEE,0xC9
	>
	>
{


	cross_function<IGetNative,0,void*()> get_native;


	IGetNative()
		:get_native(this)
	{}
};

using namespace leveldb_cc;
struct SnapShotImplementation:public implement_unknown_interfaces<SnapShotImplementation,
	ISnapshot,IGetNative>
{
	const leveldb::Snapshot* snapshot_;

	SnapShotImplementation(const leveldb::Snapshot* s):snapshot_(s){
		get_implementation<IGetNative>()->get_native = [this]()->void*{
			return const_cast<leveldb::Snapshot*>(snapshot_);
		};
	}


};

struct ComparatorFromIComparator:public leveldb::Comparator{
	use_unknown<IComparator> ic_;

	ComparatorFromIComparator(use_unknown<IComparator> ic):ic_(ic){}

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

struct CacheImplementation
	:public implement_unknown_interfaces<CacheImplementation,ICache,
	IGetNative>
{
	std::unique_ptr<leveldb::Cache> cache_;

	CacheImplementation(leveldb::Cache* c):cache_(c){
		get_implementation<IGetNative>()->get_native = 
			[this]()->void*{return cache_.get();};

	}

};

struct FilterPolicyImplementation
	:public implement_unknown_interfaces<FilterPolicyImplementation,IFilterPolicy,
	IGetNative>
{
	std::unique_ptr<const leveldb::FilterPolicy> filter_;

	FilterPolicyImplementation(const leveldb::FilterPolicy* f):filter_(f){
		get_implementation<IGetNative>()->get_native = 
			[this]()->void*{return const_cast<leveldb::FilterPolicy*>(filter_.get());};

	}

};

struct OptionsImplementation
	:public implement_unknown_interfaces<OptionsImplementation,IOptions,
	IGetNative>
{
	leveldb::Options options_;

	OptionsImplementation(){
		auto imp = get_implementation<IOptions>();
		imp->get_create_if_missing = [this](){
			return options_.create_if_missing;
		};

		imp->set_create_if_missing = [this](bool b){
			options_.create_if_missing = b;

		};
		imp->get_error_if_exists = [this](){
			return options_.error_if_exists;
		};

		imp->set_error_if_exists = [this](bool b){
			options_.error_if_exists = b;

		};
		imp->get_paranoid_checks = [this](){
			return options_.paranoid_checks;
		};

		imp->set_paranoid_checks = [this](bool b){
			options_.paranoid_checks = b;

		};
		imp->get_write_buffer_size = [this]()->std::uint32_t{
			return options_.write_buffer_size;
		};

		imp->set_write_buffer_size = [this](std::uint32_t s){
			options_.write_buffer_size = s;

		};
		imp->get_max_open_files = [this]()->std::uint32_t{
			return options_.max_open_files;
		};

		imp->set_max_open_files = [this](std::uint32_t n){
			options_.max_open_files = n;

		};

		imp->get_block_size = [this]()->std::uint32_t{
			return options_.block_size;
		};

		imp->set_block_size = [this](std::uint32_t n){
			options_.block_size = n;

		};
		imp->get_block_restart_interval = [this]()->std::uint32_t{
			return options_.block_restart_interval;
		};

		imp->set_block_restart_interval = [this](std::uint32_t n){
			options_.block_restart_interval = n;

		};
		imp->get_compression = [this]()->std::uint32_t{
			return options_.compression;
		};

		imp->set_compression = [this](std::uint32_t n){
			options_.block_restart_interval = n;

		};

		imp->set_comparator = [this](use_unknown<IComparator> ic){
			if(!ic){
				throw cross_compiler_interface::error_pointer();
				//options_.comparator = nullptr;
			}else{
				options_.comparator = new ComparatorFromIComparator(ic);
			}
		};

		imp->set_block_cache = [this](use_unknown<ICache> ic){
			if(!ic){
				options_.block_cache = nullptr;
			}else{
			options_.block_cache = static_cast<leveldb::Cache*>(
				ic.QueryInterface<IGetNative>().get_native());
			}
		};
		imp->set_filter_policy = [this](use_unknown<IFilterPolicy> ic){
			if(!ic){
				options_.filter_policy = nullptr;
			}else{
			options_.filter_policy = static_cast<leveldb::FilterPolicy*>(
				ic.QueryInterface<IGetNative>().get_native());
			}
		};

		get_implementation<IGetNative>()->get_native = [this]()->void*{
			return &options_;
		};

	}




};


struct ReadOptionsImplementation
	:public implement_unknown_interfaces<ReadOptionsImplementation,IReadOptions,
	IGetNative>
{
	leveldb::ReadOptions options_;

	ReadOptionsImplementation(){
		auto imp = get_implementation<IReadOptions>();

		imp->get_verify_checksums = [this](){
			return options_.verify_checksums;
		};
		imp->set_verify_checksums = [this](bool b){
			options_.verify_checksums = b;
		};
		imp->get_verify_checksums = [this](){
			return options_.verify_checksums;
		};
		imp->set_verify_checksums = [this](bool b){
			options_.verify_checksums = b;
		};

		imp->set_snapshot = [this](use_unknown<ISnapshot> s){
			if(!s){
				options_.snapshot = nullptr;
			}else{
				options_.snapshot = static_cast<leveldb::Snapshot*>(s.QueryInterface<IGetNative>()
					.get_native());
			}

		};
		get_implementation<IGetNative>()->get_native = 
			[this](){return &options_;};

	};


};

struct WriteOptionsImplementation
	:public implement_unknown_interfaces<WriteOptionsImplementation,
	IWriteOptions,IGetNative>
{
	leveldb::WriteOptions options_;

	WriteOptionsImplementation(){
		auto imp = get_implementation<IWriteOptions>();

		imp->get_sync = [this](){

			return options_.sync;
		};
		imp->set_sync = [this](bool b){
			options_.sync = b;
		};


		get_implementation<IGetNative>()->get_native = [this]()->void*{
			return &options_;

		};
	}


};

struct HandlerFromIHandler:public leveldb::WriteBatch::Handler{
	use_unknown<IHandler> handler_;

	virtual void Put(const leveldb::Slice& key, const leveldb::Slice& value)override{
		Slice skey(key.data(),key.size());
		Slice svalue(value.data(),value.size());
		handler_.Put(skey,svalue);

	}
	virtual void Delete(const leveldb::Slice& key)override{
		Slice skey(key.data(),key.size());
		handler_.Delete(skey);

	}

	HandlerFromIHandler(use_unknown<IHandler> h)
		:handler_(h){}


};


Status StatusFromLevelDBStatus(leveldb::Status& s){
	// On Purpose we made our Status the same as leveldb::Status
	// So we cast
	return *reinterpret_cast<Status*>(&s);

}

struct WriteBatchImplementation
	:public implement_unknown_interfaces<WriteBatchImplementation,
	IWriteBatch,IGetNative>
{
	leveldb::WriteBatch wb_;

	WriteBatchImplementation(){
		auto imp =  get_implementation<IWriteBatch>();


		imp->Put = [this](Slice key,Slice value){
			wb_.Put(leveldb::Slice(key.data(),key.size()),
				leveldb::Slice(value.data(),value.size()));

		};

		imp->Delete = [this](Slice key){
			wb_.Delete(leveldb::Slice(key.data(),key.size()));

		};

		imp->Clear = [this](){
			wb_.Clear();

		};
		imp->Iterate = [this](use_unknown<IHandler> h)->Status{
			leveldb::Status s = wb_.Iterate(new HandlerFromIHandler(h));
			return StatusFromLevelDBStatus(s);

		};

		get_implementation<IGetNative>()->get_native = 
			[this]()->void*{ return &wb_;};
	}



};

struct IteratorImplementation
	:public implement_unknown_interfaces<IteratorImplementation,IIterator>{
		std::unique_ptr<leveldb::Iterator> iter_;
		IteratorImplementation(leveldb::Iterator* iter):iter_(iter){
			auto imp = get_implementation<IIterator>();

			imp->Valid = [this]()->bool{return iter_->Valid();};

			imp->SeekToFirst = [this](){iter_->SeekToFirst();};

			imp->SeekToLast = [this](){iter_->SeekToLast();};

			imp->Seek = [this](Slice key){ iter_->Seek(leveldb::Slice(
				key.data(),key.size()));
			};

			imp->Next = [this](){ iter_->Next();};

			imp->Prev = [this](){ iter_->Prev();};

			imp->key = [this]()->Slice{
				auto s = iter_->key();
				return Slice(s.data(),s.size());
			};

			imp->value = [this]()->Slice{
				auto s = iter_->value();
				return Slice(s.data(),s.size());
			};

			imp->status = [this]()->Status{
				auto s = iter_->status();
				return StatusFromLevelDBStatus(s);
			};

		};


};


struct DBImplementation:public implement_unknown_interfaces<DBImplementation,
	IDB>
{
	std::unique_ptr<leveldb::DB> db_;

	DBImplementation(leveldb::DB* db):db_(db){

		auto imp = get_implementation<IDB>();

		imp->Put = [this](use_unknown<IWriteOptions> wo,Slice key,Slice value)
			->Status{
				auto s = db_->Put(*static_cast<leveldb::WriteOptions*>(wo
					.QueryInterface<IGetNative>().get_native()),
					leveldb::Slice(key.data(),key.size()),
					leveldb::Slice(value.data(),value.size()));
				return StatusFromLevelDBStatus(s);

		};

		imp->Delete = [this](use_unknown<IWriteOptions> wo,Slice key)
			->Status{
				auto s = db_->Delete(*static_cast<leveldb::WriteOptions*>(wo
					.QueryInterface<IGetNative>().get_native()),
					leveldb::Slice(key.data(),key.size()));
				return StatusFromLevelDBStatus(s);

		};

		imp->Write = [this](use_unknown<IWriteOptions> wo,
			use_unknown<IWriteBatch> wb)
			->Status{
				auto s = db_->Write(*static_cast<leveldb::WriteOptions*>(wo
					.QueryInterface<IGetNative>().get_native()),
					static_cast<leveldb::WriteBatch*>(wb
					.QueryInterface<IGetNative>().get_native()));
				return StatusFromLevelDBStatus(s);

		};


		imp->Get = [this](use_unknown<IReadOptions> ro,
			Slice key,out<std::string> oret)
		{
			std::string value;

			auto s = db_->Get(*static_cast<leveldb::ReadOptions*>(ro
				.QueryInterface<IGetNative>().get_native()),
				leveldb::Slice(key.data(),key.size()),
				&value);
			oret.set(value);
			return StatusFromLevelDBStatus(s);

		};

		imp->NewIterator = [this](use_unknown<IReadOptions> ro)->use_unknown<IIterator>{
			return IteratorImplementation::create(db_->NewIterator(
				*static_cast<leveldb::ReadOptions*>(ro
				.QueryInterface<IGetNative>().get_native())))
				.QueryInterface<IIterator>();

		};

		imp->GetSnapshot = [this]()->use_unknown<ISnapshot>{
			return SnapShotImplementation::create(db_->GetSnapshot())
				.QueryInterface<ISnapshot>();

		};

		imp->ReleaseSnapshot = [this](use_unknown<ISnapshot> s){
			db_->ReleaseSnapshot(static_cast<leveldb::Snapshot*>(s
				.QueryInterface<IGetNative>().get_native()));

		};

		imp->GetProperty = [this](Slice prop,out<std::string> oret){
			std::string value;
			auto b = db_->GetProperty(leveldb::Slice(prop.data(),prop.size()),
				&value);
			oret.set(value);
			return b;
		};

		imp->GetApproximateSizes = [this](std::vector<Range> v){
			std::vector<std::uint64_t> ret(v.size());
			std::vector<leveldb::Range> ranges;
			for(auto& r:v){
				ranges.push_back(leveldb::Range(

					leveldb::Slice(r.first.data(),r.first.size()),
					leveldb::Slice(r.second.data(),r.second.size())));

			}

			db_->GetApproximateSizes(&ranges[0],ranges.size(),&ret[0]);
			return ret;


		};

		imp->CompactRange = [this](Slice begin,Slice end){
			leveldb::Slice b(begin.data(),begin.size());
			leveldb::Slice e(end.data(),end.size());
			db_->CompactRange(&b,&e);

		};

		imp->CompactAll = [this](){
			db_->CompactRange(nullptr,nullptr);
		};

	}







};



struct LevelDBStaticFunctions
	:public implement_unknown_interfaces<LevelDBStaticFunctions
	,ILevelDBStaticFunctions>
{

	LevelDBStaticFunctions(){
		auto imp = get_implementation<ILevelDBStaticFunctions>();

		imp->Open = [](use_unknown<IOptions> options,std::string name,out<use_unknown<IDB>> oret){
			leveldb::DB* db = 0;
			auto s = leveldb::DB::Open(*static_cast<leveldb::Options*>(options
				.QueryInterface<IGetNative>().get_native()),
				name,&db);
			oret.set(DBImplementation::create(db).QueryInterface<IDB>());
			return StatusFromLevelDBStatus(s);

		};

		imp->CreateOptions = [](){
			return OptionsImplementation::create().QueryInterface<IOptions>();
		};

		imp->CreateReadOptions = [](){
			return ReadOptionsImplementation::create().QueryInterface<IReadOptions>();
		};

		imp->CreateWriteOptions = [](){
			return WriteOptionsImplementation::create().QueryInterface<IWriteOptions>();
		};
		imp->CreateWriteBatch = [](){
			return WriteBatchImplementation::create().QueryInterface<IWriteBatch>();
		};
		imp->DestroyDB = [](std::string name, use_unknown<IOptions> options){
			auto s = leveldb::DestroyDB(name,*static_cast<leveldb::Options*>(options
				.QueryInterface<IGetNative>().get_native()));
			return StatusFromLevelDBStatus(s);
		};

		imp->RepairDB = [](std::string name, use_unknown<IOptions> options){
			auto s = leveldb::RepairDB(name,*static_cast<leveldb::Options*>(options
				.QueryInterface<IGetNative>().get_native()));
			return StatusFromLevelDBStatus(s);
		};


		imp->NewLRUCache = [](std::uint64_t capacity){
			return CacheImplementation::create(leveldb::NewLRUCache(static_cast<std::size_t>(capacity)))
				.QueryInterface<ICache>();

		};

		imp->NewBloomFilterPolicy = [](std::int32_t bits){
			return FilterPolicyImplementation::create(leveldb::NewBloomFilterPolicy(bits))
				.QueryInterface<IFilterPolicy>();
		};
	}
};


extern "C"{

	cross_compiler_interface::portable_base* CROSS_CALL_CALLING_CONVENTION CreateLevelDBStaticFunctions(){
		auto ret_int = LevelDBStaticFunctions::create();

		auto ret = ret_int.get_portable_base();

		ret_int.reset_portable_base();


		return ret;
	}
}