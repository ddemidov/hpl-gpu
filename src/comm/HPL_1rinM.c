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

/*
 * Include files
 */
#include "hpl.h"

#ifdef HPL_NO_MPI_DATATYPE  /* The user insists to not use MPI types */
#ifndef HPL_COPY_L       /* and also want to avoid the copy of L ... */
#define HPL_COPY_L   /* well, sorry, can not do that: force the copy */
#endif
#endif
 
int HPL_binit_1rinM
(
   HPL_T_panel *              PANEL
)
{
#ifdef HPL_USE_MPI_DATATYPE
/*
 * .. Local Variables ..
 */
   int                        ierr;
#endif
/* ..
 * .. Executable Statements ..
 */
   if( PANEL == NULL )           { return( HPL_SUCCESS ); }
   if( PANEL->grid->npcol <= 1 ) { return( HPL_SUCCESS ); }
#ifdef HPL_USE_MPI_DATATYPE
#ifdef HPL_COPY_L
/*
 * Copy the panel into a contiguous buffer
 */
   HPL_copyL( PANEL );
#endif
/*
 * Create the MPI user-defined data type
 */
   ierr = HPL_packL( PANEL, 0, PANEL->len, 0 );

   return( ( ierr == MPI_SUCCESS ? HPL_SUCCESS : HPL_FAILURE ) );
#else
/*
 * Force the copy of the panel into a contiguous buffer
 */
   HPL_copyL( PANEL );
 
   return( HPL_SUCCESS );
#endif
}

#ifdef HPL_USE_MPI_DATATYPE
 
#define   _M_BUFF     PANEL->buffers[0]
#define   _M_COUNT    PANEL->counts[0]
#define   _M_TYPE     PANEL->dtypes[0]
 
#else
 
#define   _M_BUFF     (void *)(PANEL->L2)
#define   _M_COUNT    PANEL->len
#define   _M_TYPE     MPI_DOUBLE
 
#endif

int HPL_bcast_1rinM
(
   HPL_T_panel                * PANEL
)
{
/*
 * .. Local Variables ..
 */
   MPI_Comm                   comm;
   int                        ierr, next, msgid, partner, prev,
                              rank, root, size, IFLAG;
/* ..
 * .. Executable Statements ..
 */
/*
 * Cast phase:  If I am the root process,  then  send message to its two
 * next neighbors. Otherwise, probe for message. If the message is here,
 * then receive it,   and  if I am not the last process of the ring,  or
 * just after the root process, then forward it to the next.  Otherwise,
 * inform the caller that the panel has still not been received.
 */
   rank = PANEL->grid->mycol; comm  = PANEL->grid->row_comm;
   root = PANEL->pcol;        msgid = PANEL->msgid;
   size = PANEL->grid->npcol;
   next = MModAdd1( rank, size );
 
   if( rank == root )
   {
      ierr = MPI_Send( _M_BUFF, _M_COUNT, _M_TYPE, next, msgid, comm );
      if( ( ierr == MPI_SUCCESS ) && ( size > 2 ) )
      {
         ierr = MPI_Send( _M_BUFF, _M_COUNT, _M_TYPE, MModAdd1( next,
                          size ), msgid, comm );
      }
   }
   else
   {
      prev = MModSub1( rank, size );
      if( ( size > 2 ) && 
          ( MModSub1( prev, size ) == root ) ) partner = root;
      else                                     partner = prev;

            ierr = MPI_Recv( _M_BUFF, _M_COUNT, _M_TYPE, partner, msgid,
                             comm, &PANEL->status[0] );
            if( ( ierr == MPI_SUCCESS ) &&
                ( prev != root ) && ( next != root ) )
            {
               ierr = MPI_Send( _M_BUFF, _M_COUNT, _M_TYPE, next, msgid,
                                comm );
            }
   }
/*
 * If the message was received and being forwarded,  return HPL_SUCCESS.
 * If an error occured in an MPI call, return HPL_FAILURE.
 */
   if (ierr != MPI_SUCCESS) {fprintf(stderr, "ERROR - MPI Function returned error\n"); exit(1);}

/*
 * Release the arrays of request / status / data-types and buffers
 */
#ifdef HPL_USE_MPI_DATATYPE
   ierr = MPI_Type_free( &PANEL->dtypes[0] );
   return( ( ierr == MPI_SUCCESS ? HPL_SUCCESS : HPL_FAILURE ) );
#else
   return( HPL_SUCCESS );
#endif
}
