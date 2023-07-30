/*
 * JULEA - Flexible storage framework
 * Copyright (C) 2010-2022 Michael Kuhn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <julea-config.h>

#include "julea-fuse.h"

#include <glib.h>

#include <locale.h>

#include "file.h"

struct fuse_operations jfs_vtable = {
	.access = jfs_access,
	.chmod = jfs_chmod,
	.chown = jfs_chown,
	.create = jfs_create,
	.copy_file_range = jfs_copy_file_range,
	.destroy = jfs_destroy,
	.fallocate = jfs_fallocate,
	.getattr = jfs_getattr,
	.init = jfs_init,
	.mkdir = jfs_mkdir,
	.open = jfs_open,
	.read = jfs_read,
	.readdir = jfs_readdir,
	.rename = jfs_rename,
	.rmdir = jfs_rmdir,
	.truncate = jfs_truncate,
	.unlink = jfs_unlink,
	.utimens = jfs_utimens,
	.write = jfs_write,
};
#ifdef DATABASE_METADATA
JDBSchema* db_schema = NULL;

#else
#endif
int
main(int argc, char** argv)
{
	gint ret;

#ifdef DATABASE_METADATA
	g_autoptr(JBatch) batch = NULL;
	ret = EXIT_FAILURE;
#endif

	// Explicitly enable UTF-8 since functions such as g_format_size might return UTF-8 characters.
	setlocale(LC_ALL, "C.UTF-8");

#ifdef DATABASE_METADATA
	batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);
	db_schema = j_db_schema_new("posix", "metadata", NULL);
	if (j_db_schema_get(db_schema, batch, NULL))
	{
		if (!j_batch_execute(batch))
		{
			JDBSchema* insert = metadata_schema();
			j_db_schema_create(insert, batch, NULL);
			j_db_schema_unref(db_schema);
			db_schema = j_db_schema_new("posix", "metadata", NULL);
			j_db_schema_get(db_schema, batch, NULL);
			if (j_batch_execute(batch))
			{
				ret = fuse_main(argc, argv, &jfs_vtable, NULL);
			}
			j_db_schema_unref(insert);
		}
		else
		{
#endif

			ret = fuse_main(argc, argv, &jfs_vtable, NULL);

#ifdef DATABASE_METADATA
		}
	}
	j_db_schema_unref(db_schema);
#endif
	return ret;
}
