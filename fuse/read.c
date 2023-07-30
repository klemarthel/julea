/*
 * JULEA - Flexible storage framework
 * Copyright (C) 2010-2023 Michael Kuhn
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
#include "file.h"

#include <errno.h>

int
jfs_read(char const* path, char* buf, size_t size, off_t offset, struct fuse_file_info* fi)
{
	int ret = -ENOENT;

	g_autoptr(JBatch) batch = NULL;
	g_autoptr(JObject) object = NULL;
	guint64 bytes_read;
	g_autofree gchar* object_name=nr_to_object_name(fi->fh);
	(void)path;
	//out=j_file_metadata_out_new_load_for_update(path,fs);
	batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);
	
	
	object = j_object_new("posix", object_name);

	j_object_read(object, buf, size, offset, &bytes_read, batch);

	if (j_batch_execute(batch))
	{
		ret = bytes_read;
	}
	
	
	return ret;
}
