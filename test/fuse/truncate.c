#include <julea-config.h>
#include "../fuse/julea-fuse.h"
#include <errno.h>
#include "../fuse/file.h"

static void
test_fuse_jfs_truncate(void)
{
	
}

void
test_fuse_truncate(void)
{
	g_test_add_func("/fuse/truncate", test_fuse_jfs_truncate);
}