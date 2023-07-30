#include <julea-config.h>
#include <errno.h>
#include "../fuse/file.h"
#include "../fuse/julea-fuse.h"

static void
test_fuse_jfs_copy_file_range(void)
{
	
}
void
test_fuse_copy_file_range(void)
{
	g_test_add_func("/fuse/copy_file_range", test_fuse_jfs_copy_file_range);
}