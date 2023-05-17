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
jfs_open(char const* path, struct fuse_file_info* fi)
{
	int ret = -ENOENT;
	g_autoptr(JFileMetadataIn) in=NULL;
	g_autoptr(JFileSelector) fs=NULL;

	fs=j_file_selector_new(path);
	in=j_file_metadata_new_load(fs);
	if(in){
		fi->fh=get_object(in);
		ret=0;
	}
	
	return ret;
}
