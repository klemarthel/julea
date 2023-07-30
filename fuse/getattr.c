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
#include "julea-fuse.h"

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "file.h"

int
jfs_getattr(char const* path, struct stat* stbuf, struct fuse_file_info* fi)
{
	int ret = -ENOENT;

	g_autoptr(JFileMetadataIn) fe=NULL;
	g_autoptr(JFileSelector) fs=NULL;
	(void)fi;
	

	if (g_strcmp0(path, "/") == 0)
	{
		stbuf->st_mode = S_IFDIR | S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IWOTH;
		stbuf->st_nlink = 1;
		stbuf->st_uid = 0;
		stbuf->st_gid = 0;
		stbuf->st_size = 0;
		stbuf->st_atime = stbuf->st_ctime = stbuf->st_mtime = g_get_real_time() / G_USEC_PER_SEC;
		return 0;
	}

	
	fs=j_file_selector_new(path);
	fe=j_file_metadata_new_load(fs);

	if (fe)
	{
		
		gint64 size = 0;
		struct timespec* atime=NULL;
		struct timespec* ctime=NULL;
		struct timespec* mtime=NULL;
		gint64 owner = 0;
		gint64 group = 0;
		gint32 mode = 0;

		size =get_size(fe);
		mtime =get_mtime(fe);
		atime =get_atime(fe);
		ctime =get_ctime(fe);
		owner =get_owner(fe);
		group =get_group(fe);
		mode=get_mode(fe);
		

		stbuf->st_nlink = 1;
		stbuf->st_uid = owner;
		stbuf->st_gid = group;

		stbuf->st_atime = atime->tv_sec;
		stbuf->st_atimensec = atime->tv_sec;

		stbuf->st_ctime = ctime->tv_sec;
		stbuf->st_ctimensec = ctime->tv_sec;
		
		stbuf->st_mtime = mtime->tv_sec;
		stbuf->st_mtimensec = mtime->tv_sec;
		
		stbuf->st_mode =  mode;
		stbuf->st_size = size;
		ret = 0;
		g_free(atime);
		g_free(mtime);
		g_free(ctime);
	}
	return ret;
}
