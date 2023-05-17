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

#include <errno.h>
#include <glib.h>
#include <string.h>
#include "file.h"

int
jfs_write(char const* path, char const* buf, size_t size, off_t offset, struct fuse_file_info* fi)
{
	int ret = -ENOENT;

	g_autoptr(JBatch) batch = NULL;
	g_autoptr(JObject) object = NULL;
	JFileSelector* fs = NULL;
	JFileMetadataIn* in = NULL;
	JFileMetadataOut* out = NULL;

	guint64 bytes_written;
	g_autofree gchar* object_name = nr_to_object_name(fi->fh);

	fs = j_file_selector_new(path);
	batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);
	object = j_object_new("posix", object_name);
	in = j_file_metadata_new(fs, batch);

	j_object_write(object, buf, size, offset, &bytes_written, batch);

	if (j_batch_execute(batch))
	{
		struct timespec time;

		guint64 new_size = offset + bytes_written;
		ret = bytes_written;
		out = j_file_metadata_in_to_out(in);
		if (get_size(in) < new_size)
		{
			set_size(out, new_size);
		}
		clock_gettime(CLOCK_REALTIME_ALARM, &time);
		set_atime(out, &time);
		set_mtime(out, &time);
		j_file_metadata_write(fs, out, batch);
		if (!j_batch_execute(batch))
		{
			ret = -EIO;
		}
	}
	j_file_metadata_in_destroy(in);
	j_file_metadata_out_destroy(out);
	j_file_selector_destroy(fs);
	return ret;
}
