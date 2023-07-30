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
#include "file.h"

#include <errno.h>
#include <julea.h>

int
jfs_create(char const* path, mode_t mode, struct fuse_file_info* fi)
{
	int ret = -ENOENT;
	guint64 object_number=0;
	guint64 max_iter=G_MAXUINT64;
	struct timespec time;
	g_autoptr(JBatch) batch = NULL;
	g_autoptr(JObject) object = NULL;
	
	g_autofree gchar* basename = NULL;
	
	guint64 size=0;
	guint64 owner=geteuid();
	guint64 group=getegid();
	
	
	g_autoptr(JFileMetadataOut) out=NULL;
	g_autoptr(JFileSelector) fs=NULL;
	clock_gettime(CLOCK_REALTIME_ALARM,&time);
	out=j_file_metadata_out_new(path);
	fs=j_file_selector_new(path);
	
	basename = g_path_get_basename(path);
	batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);
	
	
	object_number=g_str_hash(path);
	object = j_object_new("posix", "");
	while(max_iter)
	{
		gint64 time_tmp;
		guint64 size_tmp;
		char* object_name;
		object_number+=g_random_int();
		object_name=nr_to_object_name(object_number);
		j_object_unref(object);
		object = j_object_new("posix", object_name);
		j_object_status(object,&time_tmp,&size_tmp,batch);
		g_free(object_name);
		max_iter--;
		if (j_batch_execute(batch))
		{
			break;
		}
		
	}
	fi->fh=object_number;

	set_object(out,object_number);
	set_size(out,size);
	set_owner(out,owner); 
	set_group(out,group);
	set_mode(out,mode);
	set_atime(out,&time);
	set_mtime(out,&time);
	set_ctime(out,&time);
	j_file_metadata_create(fs,out,batch);

	j_object_create(object, batch);

	if (j_batch_execute(batch))
	{
		ret = 0;
	}
	/// \todo does not return 0 on success
	return ret;
}
