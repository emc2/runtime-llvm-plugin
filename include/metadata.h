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
  GEN_TYPE_FUNC = 0,
  GEN_TYPE_STRUCT = 1,
  GEN_TYPE_ARRAY = 2,
  GEN_TYPE_NATIVEPTR = 3,
  GEN_TYPE_GCPTR = 4,
  GEN_TYPE_INT = 5,
  GEN_TYPE_FLOAT = 6,
  GEN_TYPE_NAMED = 7,
  GEN_TYPE_UNIT = 8
};

enum {
  TYPE_MUT_MUTABLE = 0,
  TYPE_MUT_WRITEONCE = 1,
  TYPE_MUT_IMMUTABLE = 2
};

enum {
  PTR_MOB_MOBILE = 0,
  PTR_MOB_IMMOBILE = 1
};

enum {
  PTRCLASS_STRONG = 0,
  PTRCLASS_SOFT = 1,
  PTRCLASS_WEAK = 2,
  PTRCLASS_FINALIZER = 3,
  PTRCLASS_PHANTOM = 4
};

#endif
