#include <julea-config.h>
#include "unistd.h"
#include "../fuse/julea-fuse.h"

static void
test_fuse_jfs_init(void)
{
	
}
void
test_fuse_init(void)
{
	g_test_add_func("/fuse/init", test_fuse_jfs_init);
}