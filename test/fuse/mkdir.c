#include <julea-config.h>
#include <errno.h>
#include "../fuse/julea-fuse.h"
#include "../fuse/file.h"


static void
test_fuse_jfs_mkdir(void)
{
	
}
void
test_fuse_mkdir(void)
{
	g_test_add_func("/fuse/mkdir", test_fuse_jfs_mkdir);
}