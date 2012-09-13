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
#ifndef _GC_PARAMS_H_
#define _GC_PARAMS_H_

/*!
 * This struct contains all the parameters for the GC generator.
 * These structures are intended to be constants, declared as a set of
 * parameters for a given GC implementation.
 *
 * \brief Parameters to the GC generation functions.
 */
struct GCParams {
public:

  /*!
   * This is the initializer for the GC parameter structure.  This is
   * little more than a direct initialization of the fields.
   *
   * \brief Initialize a GC param structure
   * \param writeLogging Whether or not to generate write logging.
   * \param readBarriers Whether or not to generate read barriers.
   * \param clusterize Whether or not to generate code to clusterize
   *                   objects.
   * \param generational Whether or not to add fields for generational
   *                     GC.
   * \param doublePtrs Whether or not to represent GC pointers with
   *                   double pointers.
   * \param copyFuncs Whether or not to generate copy functions.
   * \param moveFuncs Whether or not to generate move functions.
   * \param traceFuncs Whether or not to generate trace functions.
   */
  GCParams(const bool writeLogging,
	   const bool readBarriers,
	   const bool clusterize,
	   const bool generational,
	   const bool doublePtrs,
	   const bool copyFuncs,
	   const bool moveFuncs,
	   const bool traceFuncs) :
    writeLogging(writeLogging), readBarriers(readBarriers),
    clusterize(clusterize), generational(generational),
    doublePtrs(doublePtrs), copyFuncs(copyFuncs),
    moveFuncs(moveFuncs), traceFuncs(traceFuncs) {}

  /*!
   * This field determines whether or not to generate write logging.
   * If set, then the execution context structure will contain a write
   * log field.  Writes to any GC object will be logged, and then
   * propogated to the new heap at some point in the future.  This
   * will cause sync functions to be generated as well.
   *
   * \brief Whether or not to generate write logging.
   */
  const bool writeLogging;

  /*!
   * This is a placeholder at present for the (older) alternate
   * strategy of writing through to the new heap and reading from it.
   * I would imagine any experiment would show this to be an order of
   * magnitude slower than write logging.
   *
   * I think Cheyney's original algorithim did this, and the
   * Cheng/Blelloch collector opted for write logging.
   *
   * \brief Whether or not to generate read barriers.
   */
  const bool readBarriers;

  /*!
   * This field determines whether or not to clusterize objects.  If
   * set, then big objects (most likely arrays) will be split up into
   * clusters, which will possibly be handled by separate threads.
   *
   * \brief Whether or not to generate code to clusterize objects.
   */
  const bool clusterize;

  /*!
   * This field determines whether or not to add information for
   * generational GC into various strutures.  If set, then GC headers
   * and GC contexts will contain additional info, and GC allocators
   * will be an array of allocators for each generation.
   *
   * \brief Whether or not to add fields for generational GC.
   */
  const bool generational;

  /*!
   * This field controls whether or not to represent pointers as
   * double pointers.  If set, any pointer to a GC object will
   * actually be a 2 x array, and the GC context structure will
   * contain information indicating which one represents the current
   * heap.
   *
   * This trick is used in the Cheng/Blelloch and the lock-free
   * concurrent collector.
   *
   * \brief Whether or not to represent GC pointers with double
   *        pointers.
   */
  const bool doublePtrs;

  /*!
   * This field determines whether or not to generate copy code.  If
   * set, then copy functions will be generated for any appropriate
   * structure.
   *
   * \brief Whether or not to generate copy functions.
   */
  const bool copyFuncs;

  /*!
   * This field determines whether or not to generate move code.  If
   * set, then move functions will be generated for any appropriate
   * structure.  The difference between these functions and the copy
   * functions is the same as the difference between memcpy and
   * memmove.  Move functions are appropriate for sliding compacting
   * collectors, where copy functions are appropriate for copying
   * collectors.
   *
   * \brief Whether or not to generate move functions
   */
  const bool moveFuncs;

  /*!
   * This field determines whether or not to generate trace code.  If
   * set, then trace functions will be generated for any appropriate
   * structure.  This is necessary for both mark and sweep as well as
   * generational collectors.
   *
   * \brief Whether or not to generate trace functions.
   */
  const bool traceFunc;

  static const unsigned clusterSize;
};

#endif
