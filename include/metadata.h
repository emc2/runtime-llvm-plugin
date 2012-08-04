/* Copyright (c) 2012 Eric McCorkle. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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
	GC_MD_PTRCLASS_FINAL,
	GC_MD_PTRCLASS_PHANTOM
};

#endif
