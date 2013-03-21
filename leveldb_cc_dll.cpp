
#include "leveldbwin\leveldb_src\include\leveldb\db.h"
#include "leveldbwin\leveldb_src\include\leveldb\comparator.h"

#include "level_db_interfaces.h"

struct SnapShotImplementation:public implement_unknown_interfaces<SnapShotImplementation,
	ISnapshot>
{
	leveldb::Snapshot* snapshot_;

	SnapShotImplementation(leveldb::Snapshot* s):snapshot_(s){
		get_implementation<ISnapshot>()->get_native = [this]()->void*{
			return snapshot_;
		};

		get_implementation<ISnapshot>()->get_ptr_to_snapshot_ptr = [this]()->void*{
			return &snapshot_;
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

struct OptionsImplementation
	:public implement_unknown_interfaces<OptionsImplementation,IOptions>
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
			options_.comparator = new ComparatorFromIComparator(ic);
		};
	
		imp->get_native = [this]()->void*{
			return &options_;
		};

	}




};


struct ReadOptionsImplementation
	:public implement_unknown_interfaces<ReadOptionsImplementation,IReadOptions>
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

	};


};

struct WriteOptionsImplementation
	:public implement_unknown_interfaces<WriteOptionsImplementation,
		IWriteOptions>
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

		imp->get_post_write_snapshot = [this]()->use_unknown<ISnapshot>{
			if(options_.post_write_snapshot){
				return SnapShotImplementation::create(
					const_cast<leveldb::Snapshot *>(*options_.post_write_snapshot))
					.QueryInterface<ISnapshot>();
			}
			else{
				return use_unknown<ISnapshot>();
			}

		};

		imp->set_post_write_snapshot = [this](use_unknown<ISnapshot> is){
			if(!!is){
				options_.post_write_snapshot = 
					const_cast<const leveldb::Snapshot**>( static_cast<leveldb::Snapshot**>
					(is.get_ptr_to_snapshot_ptr()));

			}

		};

		imp->get_native = [this]()->void*{
			return &options_;

		};
	}


};