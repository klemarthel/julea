#include <julea-config.h>
#include "../fuse/julea-fuse.h"
#include <errno.h>
#include "../fuse/file.h"

static void
test_fuse_jfs_utimens(void)
{
	
}
void
test_fuse_utimens(void)
{
	g_test_add_func("/fuse/utimens", test_fuse_jfs_utimens);
}