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
jfs_mkdir(char const* path, mode_t mode)
{
	int ret = -ENOENT;

	g_autoptr(JBatch) batch = NULL;
	g_autoptr(JFileMetadataOut) out=NULL;
	g_autoptr(JFileSelector) fs=NULL;
	struct timespec now;
	
	g_autofree gchar* basename = NULL;
	g_autofree gchar* directory = NULL;

	mode=mode|S_IFDIR;
	clock_gettime(CLOCK_REALTIME_ALARM,&now);
	fs=j_file_selector_new(path);
	out=j_file_metadata_out_new(path);
	basename = g_path_get_basename(path);
	directory = g_path_get_dirname(path);
	batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);
	
	//set_name(out,basename);
	set_object(out,0);
	set_size(out,0); //TODO!
	set_owner(out,geteuid()); // TODO difference between effective and real 
	set_group(out,getegid());
	set_mode(out,mode);
	set_ctime(out,&now);
	set_atime(out,&now);
	set_mtime(out,&now);
	

	j_file_metadata_create(fs,out,batch);

	if (j_batch_execute(batch))
	{
		ret = 0;
	}
	return ret;
}
