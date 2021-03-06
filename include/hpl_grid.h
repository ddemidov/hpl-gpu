/*
 *  -- High Performance Computing Linpack Benchmark (HPL-GPU)
 *     HPL-GPU - 1.1 - 2011
 *
 *     David Rohr
 *     Matthias Kretz
 *     Matthias Bach
 *     Goethe Universität, Frankfurt am Main
 *     Frankfurt Institute for Advanced Studies
 *     (C) Copyright 2010 All Rights Reserved
 *
 *     Antoine P. Petitet
 *     University of Tennessee, Knoxville
 *     Innovative Computing Laboratory
 *     (C) Copyright 2000-2008 All Rights Reserved
 *
 *  -- Copyright notice and Licensing terms:
 *
 *  Redistribution  and  use in  source and binary forms, with or without
 *  modification, are  permitted provided  that the following  conditions
 *  are met:
 *
 *  1. Redistributions  of  source  code  must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce  the above copyright
 *  notice, this list of conditions,  and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *
 *  3. All  advertising  materials  mentioning  features  or  use of this
 *  software must display the following acknowledgements:
 *  This  product  includes  software  developed  at  the  University  of
 *  Tennessee, Knoxville, Innovative Computing Laboratory.
 *  This product  includes software  developed at the Frankfurt Institute
 *  for Advanced Studies.
 *
 *  4. The name of the  University,  the name of the  Laboratory,  or the
 *  names  of  its  contributors  may  not  be used to endorse or promote
 *  products  derived   from   this  software  without  specific  written
 *  permission.
 *
 *  -- Disclaimer:
 *
 *  THIS  SOFTWARE  IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING,  BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY
 *  OR  CONTRIBUTORS  BE  LIABLE FOR ANY  DIRECT,  INDIRECT,  INCIDENTAL,
 *  SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES  (INCLUDING,  BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA OR PROFITS; OR BUSINESS INTERRUPTION)  HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT,  STRICT LIABILITY,  OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ======================================================================
 */

#ifndef HPL_GRID_H
#define HPL_GRID_H
/*
 * ---------------------------------------------------------------------
 * Include files
 * ---------------------------------------------------------------------
 */
#include "hpl_pmisc.h"
/*
 * ---------------------------------------------------------------------
 * #typedefs and data structures
 * ---------------------------------------------------------------------
 */
//typedef enum { HPL_INT = 100, HPL_DOUBLE = 101 } HPL_T_TYPE;
//#define HPL_2_MPI_TYPE( typ ) ( ( typ == HPL_INT ? MPI_INT : MPI_DOUBLE ) )
#define HPL_T_TYPE MPI_Datatype
#define HPL_2_MPI_TYPE( typ ) (typ)
#define HPL_INT MPI_INT
#define HPL_DOUBLE MPI_DOUBLE

 
typedef enum
{
	HPL_ROW_MAJOR = 201,
	HPL_COLUMN_MAJOR = 202
} HPL_T_ORDER;

typedef struct HPL_S_grid
{
	MPI_Comm all_comm; /* grid communicator */
	MPI_Comm row_comm; /* row communicator */
	MPI_Comm col_comm; /* column communicator */
	HPL_T_ORDER order; /* ordering of the procs in the grid */
	int iam; /* my rank in the grid */
	int myrow; /* my row number in the grid */
	int mycol; /* my column number in the grid */
	int nprow; /* the total # of rows in the grid */
	int npcol; /* the total # of columns in the grid */
	int nprocs; /* the total # of procs in the grid */
	int row_ip2; /* largest power of two <= nprow */
	int row_hdim; /* row_ip2 procs hypercube dimension */
	int row_ip2m1; /* largest power of two <= nprow-1 */
	int row_mask; /* row_ip2m1 procs hypercube mask */
	int col_ip2; /* largest power of two <= npcol */
	int col_hdim; /* col_ip2 procs hypercube dimension */
	int col_ip2m1; /* largest power of two <= npcol-1 */
	int col_mask; /* col_ip2m1 procs hypercube mask */
	int* col_mapping;
	int* mcols_per_pcol;
} HPL_T_grid;

/*
 * ---------------------------------------------------------------------
 * Data Structures
 * ---------------------------------------------------------------------
 */
typedef void (*HPL_T_OP)
( const int, const void *, void *, const HPL_T_TYPE );
/*
 * ---------------------------------------------------------------------
 * #define macros definitions
 * ---------------------------------------------------------------------
 */
/*
 * The following macros perform common modulo operations; All functions
 * except MPosMod assume arguments are < d (i.e., arguments are themsel-
 * ves within modulo range).
 */
 /* add two numbers */
#define MModAdd(I1, I2, d) \
 ( ( (I1) + (I2) < (d) ) ? (I1) + (I2) : (I1) + (I2) - (d) )
 /* add 1 to # */
#define MModAdd1(I, d) ( ((I) + 1 == (d)) ? 0 : (I) + 1 )
 /* subtract two numbers */
#define MModSub(I1, I2, d) \
 ( ( (I1) < (I2) ) ? (d) + (I1) - (I2) : (I1) - (I2) )
 /* sub 1 from # */
#define MModSub1(I, d) ( ((I)!=0) ? (I)-1 : (d)-1 )

#define MColToPCol(matrix_col, nb, grid) (grid->col_mapping[(matrix_col) / (nb)])
#define MColBlockToPCol(matrix_colblock, grid) (grid->col_mapping[matrix_colblock])
/*
 * ---------------------------------------------------------------------
 * grid function prototypes
 * ---------------------------------------------------------------------
 */
int HPL_grid_init( MPI_Comm, const HPL_T_ORDER, const int, const int, HPL_T_grid * );
int HPL_grid_exit( HPL_T_grid *);

int HPL_grid_info( const HPL_T_grid *, int *, int *, int *, int * );

int HPL_barrier( MPI_Comm );
int HPL_broadcast( void *, const int, const HPL_T_TYPE, const int, MPI_Comm );
int HPL_reduce( void *, const int, const HPL_T_TYPE, const HPL_T_OP , const int, MPI_Comm );
int HPL_all_reduce( void *, const int, const HPL_T_TYPE, const HPL_T_OP , MPI_Comm );

void HPL_max( const int, const void *, void *, const HPL_T_TYPE );
void HPL_min( const int, const void *, void *, const HPL_T_TYPE );
void HPL_sum( const int, const void *, void *, const HPL_T_TYPE );

#endif
/*
 * End of hpl_grid.h
 */
