/* Copyright (c) 2012 Eric McCorkle.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */
#ifndef _METADATA_H_
#define _METADATA_H_

enum {
	GC_MD_TYPE_FUNC,
	GC_MD_TYPE_STRUCT,
	GC_MD_TYPE_ARRAY,
	GC_MD_TYPE_NATIVEPTR,
	GC_MD_TYPE_GCPTR,
	GC_MD_TYPE_INT,
	GC_MD_TYPE_FLOAT,
	GC_MD_TYPE_NAMED,
	GC_MD_TYPE_UNIT
};

enum {
	GC_MD_MUT_MUTABLE,
	GC_MD_MUT_WRITEONCE,
	GC_MD_MUT_IMMUTABLE
};

enum {
	GC_MD_MOB_MOBILE,
	GC_MD_MOB_IMMOBILE
};

enum {
	GC_MD_PTRCLASS_STRONG,
	GC_MD_PTRCLASS_SOFT,
	GC_MD_PTRCLASS_WEAK,
	GC_MD_PTRCLASS_FINALIZER,
	GC_MD_PTRCLASS_PHANTOM
};

#endif
