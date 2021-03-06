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

#ifndef HPL_PTEST_H
#define HPL_PTEST_H
/*
 * ---------------------------------------------------------------------
 * Include files
 * ---------------------------------------------------------------------
 */
#include "hpl_misc.h"
#include "hpl_blas.h"
#include "hpl_auxil.h"
#include "hpl_gesv.h"

#include "hpl_pmisc.h"
#include "hpl_pauxil.h"
#include "hpl_panel.h"
#include "hpl_pgesv.h"

#include "hpl_ptimer.h"
#include "hpl_pmatgen.h"
/*
 * ---------------------------------------------------------------------
 * Data Structures
 * ---------------------------------------------------------------------
 */
typedef struct HPL_S_test
{
	double              epsil;                      /* epsilon machine */
	double              thrsh;                            /* threshold */
	FILE *              outfp;       /* output stream (only in proc 0) */
	int                 kfail;                    /* # of tests failed */
	int                 kpass;                    /* # of tests passed */
	int                 kskip;                   /* # of tests skipped */
	int                 ktest;                /* total number of tests */
	float*              node_perf;		//Performance of participating nodes
} HPL_T_test;

/*
 * ---------------------------------------------------------------------
 * #define macro constants for testing only
 * ---------------------------------------------------------------------
 */
#define    HPL_LINE_MAX         256
#define    HPL_MAX_PARAM         20
#define    HPL_IDEFSEED         100              /* the default seed */
/*
 * ---------------------------------------------------------------------
 * global timers for timing analysis only
 * ---------------------------------------------------------------------
 */
#ifdef HPL_DETAILED_TIMING
#define    HPL_TIMING_BEG        11 /* timer 0 reserved, used by main */
#define    HPL_TIMING_N           6 /* number of timers defined below */
#define    HPL_TIMING_RPFACT     11 /* starting from here, contiguous */
#define    HPL_TIMING_PFACT      12
#define    HPL_TIMING_MXSWP      13
#define    HPL_TIMING_UPDATE     14
#define    HPL_TIMING_LASWP      15
#define    HPL_TIMING_PTRSV      16
#define    HPL_TIMING_DGEMM	     17
#define    HPL_TIMING_DTRSM	     18
#define    HPL_TIMING_DLATCPY	 19
#define    HPL_TIMING_BCAST	     20
#define    HPL_TIMING_BCASTUPD	 21
#define    HPL_TIMING_ITERATION  22
#define    HPL_TIMING_DLACPY     23
#define    HPL_TIMING_UBCAST     24
#define    HPL_TIMING_PIPELINE   25
#define    HPL_TIMING_PREPIPELINE   26
#endif
/*
 * ---------------------------------------------------------------------
 * Function prototypes
 * ---------------------------------------------------------------------
 */
void HPL_pdinfo( HPL_T_test *, int *, int *, int *, int *, HPL_T_ORDER *, int *, int *, int *, int *, HPL_T_FACT *, int *, int *, int *, int *, int *,
   HPL_T_FACT *, int *, HPL_T_TOP *, int *, int *, int *, int * );
void HPL_pdtest( HPL_T_test *, HPL_T_grid *, HPL_T_palg *, const int, const int, const int );

#endif
/*
 * End of hpl_ptest.h
 */
