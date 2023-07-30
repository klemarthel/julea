#include <julea-config.h>
#include <errno.h>
#include <string.h>
#include "../fuse/julea-fuse.h"
#include "../fuse/file.h"


static void
test_fuse_jfs_readdir(void)
{
	
}
void
test_fuse_readdir(void)
{
	g_test_add_func("/fuse/readdir", test_fuse_jfs_readdir);
}