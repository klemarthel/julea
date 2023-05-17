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
#include "file.h"

int
jfs_unlink(char const* path)
{
	int ret = -ENOENT;

	g_autoptr(JBatch) batch = NULL;

	g_autoptr(JObject) obj = NULL;
	g_autoptr(JFileMetadataOut) out=NULL;
	g_autoptr(JFileMetadataIn) in=NULL;
	g_autoptr(JFileSelector) fs=NULL;
	guint64 object_nr;
	g_autofree gchar* object_name;
	fs = j_file_selector_new(path);
	in=j_file_metadata_new_load(fs);
	if (in)
	{
		object_name=get_object_name(in);
		out = j_file_metadata_in_to_out(in);
		
		batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);
		j_file_metadata_delete(fs, out, batch);

		obj = j_object_new("posix", object_name);

		// we do not support hard links so deleting here is safe
		j_object_delete(obj, batch);

		if (j_batch_execute(batch))
		{
			ret = 0;
		}
	}
	return ret;
}
