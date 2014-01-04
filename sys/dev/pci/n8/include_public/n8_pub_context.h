/*-
 * Copyright (C) 2001-2003 by NBMK Encryption Technologies.
 * All rights reserved.
 *
 * NBMK Encryption Technologies provides no support of any kind for
 * this software.  Questions or concerns about it may be addressed to
 * the members of the relevant open-source community at
 * <tech-crypto@netbsd.org>.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*****************************************************************************
 * @(#) n8_pub_context.h 1.5@(#)
 *****************************************************************************/

/*****************************************************************************/
/** @file n8_pub_context
 *  @brief Common declarations for context memory operations.
 *
 * Public header file for context memory operations.
 *
 *****************************************************************************/

/*****************************************************************************
 * Revision history:
 * 08/05/03 bac   Removed #define N8_CONTEXT_INDEX_MAX_VALUE 262143 as it wasn't
 *                being used any more.
 * 10/24/01 dkm   Moved max index define from n8_contextM.h.
 * 10/12/01 dkm   Original version.
 ****************************************************************************/
#ifndef N8_PUB_CONTEXT_H
#define N8_PUB_CONTEXT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "n8_pub_common.h"

/*****************************************************************************
 * Function prototypes
 *****************************************************************************/
N8_Status_t N8_AllocateContext (N8_ContextHandle_t *contextHandle_p,
                                N8_Unit_t unitID);

N8_Status_t N8_FreeContext (const N8_ContextHandle_t contextHandle,
                            N8_Event_t               *event_p);

N8_Status_t N8_WriteContext  (N8_ContextHandle_t contextHandle,
                              N8_Buffer_t        *context_p, 
                              uint32_t           contextLength, 
                              N8_Event_t         *event_p );

N8_Status_t N8_ReadContext  (N8_ContextHandle_t contextHandle,
                              N8_Buffer_t        *context_p, 
                              uint32_t           contextLength, 
                              N8_Event_t         *event_p );

#ifdef __cplusplus
}
#endif

#endif


