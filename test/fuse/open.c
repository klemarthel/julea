#include <julea-config.h>
#include <errno.h>
#include <julea.h>
#include "../fuse/julea-fuse.h"
#include "../fuse/file.h"


static void
test_fuse_jfs_open(void)
{
	
}
void
test_fuse_open(void)
{
	g_test_add_func("/fuse/open", test_fuse_jfs_open);
}