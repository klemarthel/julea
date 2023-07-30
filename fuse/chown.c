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

#include <errno.h>

#include "file.h"
int
jfs_chown(char const* path, uid_t uid, gid_t gid, struct fuse_file_info* fi)
{
	gint ret = -ENOENT;
	g_autoptr(JBatch) batch;
	g_autoptr(JFileSelector) fs = NULL;
	g_autoptr(JFileMetadataOut) out = NULL;

	(void)fi;
	fs = j_file_selector_new(path);
	out = j_file_metadata_out_new_load_for_update(path,fs);
	if (out)
	{
		set_group(out, gid);
		set_owner(out, uid);
		batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);
		j_file_metadata_write(fs, out, batch);
		if (j_batch_execute(batch))
		{
			ret = 0;
		}
	}

	return ret;
}
