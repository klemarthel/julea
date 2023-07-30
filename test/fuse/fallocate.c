#include <julea-config.h>
#include <errno.h>
#include "../fuse/file.h"
#include "../fuse/julea-fuse.h"
static void
test_fuse_jfs_fallocate(void)
{
	
}
void
test_fuse_fallocate(void)
{
	g_test_add_func("/fuse/fallocate", test_fuse_jfs_fallocate);
}