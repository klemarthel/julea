#include <julea-config.h>
#include <errno.h>
#include <julea.h>
#include "../fuse/file.h"
#include "../fuse/julea-fuse.h"


static void
test_fuse_jfs_create(void)
{
	
}
void
test_fuse_create(void)
{
	g_test_add_func("/fuse/create", test_fuse_jfs_create);
}