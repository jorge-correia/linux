#ifndef EXT4JAMES_H
#define EXT4JAMES_H

#define EXT4JAMES_ON

//#define EXT4JAMES_BENCHMARK_WAESLIB_ON
//#define EXT4JAMES_BENCHMARK_RIO_ON
//#define EXT4JAMES_BENCHMARK_WIO_ON
//#define EXT4JAMES_BENCHMARK_SLEEP_TIME
//#define EXT4JAMES_BENCHMARK_RPOSTIO_ON

#define JAMES_BLOCK_SIZE 4096
#include <linux/types.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>
#include <linux/rwsem.h>
#include <linux/rwlock_types.h>
#include <linux/blk_types.h>



struct james_inodeinfo
{
  struct james_ivcache *ivcache;
  struct mutex mutex_cache;
  struct rw_semaphore ivcache_lock;

  uint32_t inflight_read_blocks[4096];
  uint32_t inflight_write_blocks[4096];
  uint8_t inflight_read_bitmap[256];
  uint8_t inflight_write_bitmap[256];
  spinlock_t inflight_lock;


  struct inode *inode;

  uint64_t global_ivblock;
  uint64_t iv_ino;
  struct file *iv_filp;
  struct file *dio_iv_filp;
  struct rw_semaphore ivfile_lock;
  //struct mutex ivfile_lock;
  //struct mutex mutex_ivfile;
  int32_t last_file_iv;

  //struct mutex mutex_ivfile_ready;

  rwlock_t rwlock_update_masks;

  //atomic_t opened_count;
  atomic_t refcount;
  atomic_t blkwrite;
  atomic_t ivcount;
  //uint8_t free_flag;
  //uint8_t in_queue;

  uint8_t unlink_called;

  void (*james_on_dio_ptr) (struct bio **src_bio, struct inode *inode, loff_t pos, loff_t len);

  //struct mutex mutex_ivdir;
};

struct james_bioinfo
{
	struct james_inodeinfo *inodeinfo;
  struct bio *bio;
  struct work_struct work;
  uint32_t ctx_pool_num;
  __uint128_t *ivs;
  __uint128_t *orig_ivs;
  uint32_t no_pages;
  uint32_t first_page;
  bio_end_io_t *endio;
  struct bvec_iter iter;
  bool is_dio;
  atomic_t mask_count;
  atomic_t refcount;

#ifdef EXT4JAMES_BENCHMARK_RIO_ON
  void *rio_chrono;
#endif

#ifdef EXT4JAMES_BENCHMARK_RPOSTIO_ON
  void *rpostio_chrono;
#endif
};

#endif

