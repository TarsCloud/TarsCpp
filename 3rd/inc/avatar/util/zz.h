/************************************************************************

        Copyright (c) 2005-2007 by Juphoon System Software, Inc.
                       All rights reserved.

     This software is confidential and proprietary to Juphoon System,
     Inc. No part of this software may be reproduced, stored, transmitted,
     disclosed or used in any form or by any means other than as expressly
     provided by the written license agreement between Juphoon and its
     licensee.

     THIS SOFTWARE IS PROVIDED BY JUPHOON "AS IS" AND ANY EXPRESS OR 
     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
     ARE DISCLAIMED. IN NO EVENT SHALL JUPHOON BE LIABLE FOR ANY DIRECT, 
     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
     OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
     POSSIBILITY OF SUCH DAMAGE. 

                    Juphoon System Software, Inc.
                    Email: support@juphoon.com
                    Web: http://www.juphoon.com

************************************************************************/
/*************************************************  
  File name     : zz.h
  Module        : zlib utilitis
  Author        : zocol   
  Version       : 0.1   
  Created on    : 2011-07-01    
  Description   :       
      Data structure and function definitions required by the zlib utilitis
    
  Modify History:   
  1. Date:        Author:         Modification:
*************************************************/
#ifndef _ZZ_H__
#define _ZZ_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef ZVOID * ZSTRM;

/* zlib return codes */
#define ZZ_STREAM_END    1
#define ZZ_NEED_DICT     2
#define ZZ_ERR_STRM      (-2)        /* stream error */
#define ZZ_ERR_DATA      (-3)        /* data was corrupted or incomplete error */
#define ZZ_ERR_MEM       (-4)        /* memory not enough error */
#define ZZ_ERR_BUF       (-5)        /* buffer not enough room error */
#define ZZ_ERR_VER       (-6)        /* version not support error */

/* zlib compression levels */
#define ZZ_COMPRESS_NO    0          /* no compression */
#define ZZ_COMPRESS_SPEED 1          /* best speed compression */
#define ZZ_COMPRESS_BEST  9          /* best compression */
#define ZZ_COMPRESS_DFT   (-1)       /* default compression */

/* zlib window bit */
#define ZZ_WINDOW_256B    8
#define ZZ_WINDOW_512B    9
#define ZZ_WINDOW_1KB     10
#define ZZ_WINDOW_2KB     11
#define ZZ_WINDOW_4KB     12
#define ZZ_WINDOW_8KB     13
#define ZZ_WINDOW_16KB    14
#define ZZ_WINDOW_32KB    15

/* zlib memory level */
#define ZZ_MEM_LEVEL1     1
#define ZZ_MEM_LEVEL2     2
#define ZZ_MEM_LEVEL3     3
#define ZZ_MEM_LEVEL4     4
#define ZZ_MEM_LEVEL5     5
#define ZZ_MEM_LEVEL6     6
#define ZZ_MEM_LEVEL7     7
#define ZZ_MEM_LEVEL8     8
#define ZZ_MEM_LEVEL9     9
#define ZZ_MEM_LEVEL_DEFUALT ZZ_MEM_LEVEL8

/**
 * @brief Create stream for compression.
 * @param  iLevel The compression level.
 * @param  iWinBit The window bits, @ref ZZ_WINDOW_32KB.
 * @param  iMemLevel The memory level, @ref ZZ_MEM_LEVEL_DEFUALT.
 * @return        The stream object.
 */
ZFUNC ZSTRM Zz_CompCreate(ZINT iLevel, ZINT iWinBit, ZINT iMemLevel);

/**
 * @brief Delete stream of compression.
 * @param  zStrm The stream object.
 */
ZFUNC ZVOID Zz_CompDelete(ZSTRM zStrm);

/**
 * @brief Copy a stream for compression.
 * @param  zStrm The source stream.
 * @return       The new stream. Otherwise ZNULL.
 */
ZFUNC ZSTRM Zz_CompCopy(ZSTRM zStrm);

/**
 * @brief Set dictionary of compression.
 * @param  zStrm   The stream object.
 * @param  pucData The dictionary data.
 * @param  iLen    The dictionary size in bytes.
 * @return         ZOK when succeed. Otherwise failed.
 */
ZFUNC ZINT Zz_CompSetDict(ZSTRM zStrm, ZCONST ZUCHAR *pucData, ZUINT iLen);

/**
 * @brief Write data to stream.
 * @param  zStrm   The stream object.
 * @param  pucData The data pointer.
 * @param  iLen    The data length in bytes.
 * @return         ZOK wnen succeed. Otherwise failed.
 */
ZFUNC ZINT Zz_CompWrite(ZSTRM zStrm, ZCONST ZUCHAR *pucData, ZUINT iLen);

/**
 * @brief Read data from stream.
 * @param  zStrm   The stream object.
 * @param  pucData The data pointer for output.
 * @param  piLen   The data buffer length in bytes. The actual data length when succeed.
 * @return         ZOK wnen succeed. Otherwise failed.
 */
ZFUNC ZINT Zz_CompRead(ZSTRM zStrm, ZBOOL bFinish, ZUCHAR *pucData, ZUINT *piLen);

/**
 * @brief Create stream for decompression.
 * @param  iWinBit The window bits, @ref ZZ_WINDOW_32KB.
 * @return        The stream object.
 */
ZFUNC ZSTRM Zz_DecompCreate(ZINT iWinBit);

/**
 * @brief Delete stream of decompression.
 * @param  zStrm The stream object.
 */
ZFUNC ZVOID Zz_DecompDelete(ZSTRM zStrm);

/**
 * @brief Copy a stream for decompression.
 * @param  zStrm The source stream.
 * @return       The new stream. Otherwise ZNULL.
 */
ZFUNC ZSTRM Zz_DecompCopy(ZSTRM zStrm);

/**
 * @brief Set dictionary of decompression.
 * @param  zStrm   The stream object.
 * @param  pucData The dictionary data.
 * @param  iLen    The dictionary size in bytes.
 * @return         ZOK when succeed. Otherwise failed.
 */
ZFUNC ZINT Zz_DecompSetDict(ZSTRM zStrm, ZCONST ZUCHAR *pucData, ZUINT iLen);

/**
 * @brief Write data to stream.
 * @param  zStrm   The stream object.
 * @param  pucData The data pointer.
 * @param  iLen    The data length in bytes.
 * @return         ZOK wnen succeed. Otherwise failed.
 */
ZFUNC ZINT Zz_DecompWrite(ZSTRM zStrm, ZCONST ZUCHAR *pucData, ZUINT iLen);

/**
 * @brief Read data from stream.
 * @param  zStrm   The stream object.
 * @param  pucData The data pointer for output.
 * @param  piLen   The data buffer length in bytes. The actual data length when succeed.
 * @return         ZOK wnen succeed. Otherwise failed.
 */
ZFUNC ZINT Zz_DecompRead(ZSTRM zStrm, ZUCHAR *pucData, ZUINT *piLen);


/* zlib compress the source buffer into the destination buffer

   Zz_Compress the source buffer into the destination buffer.  dwSrcLen is
   the byte length of the source buffer.  Upon entry, destLen is the total size
   of the destination buffer, which must be at least the value returned by
   Zz_CompressBound(dwSrcLen).  Upon exit, destLen is the actual size of the
   compressed buffer.

    compress returns ZOK if success, ZZ_ERR_MEM if there was not enough memory, 
    ZZ_ERR_BUF if there was not enough room in the output buffer.
*/
ZFUNC ZINT Zz_Compress(ZUCHAR *pucDst, ZUINT *piDstLen, 
                const ZUCHAR *pucSrc, ZUINT iSrcLen);

/* zlib  compresses the source buffer into the destination buffer.  

   The iLevel parameter see ZZ_COMPRESS_BEST etc.  
   dwSrcLen is the byte length of the source buffer.  

   Upon entry, pdwDstLen is the total size of the destination buffer, 
   which must be at least the value returned by Zz_CompressBound(dwSrcLen).  

   Upon exit, destLen is the actual size of the compressed buffer.

   Zz_Compress2 returns ZOK if success, 
   ZZ_ERR_MEM if there was not enough memory, 
   ZZ_ERR_BUF if there was not enough room in the output buffer,
   or ZZ_ERR_STRM if the level parameter is invalid.
*/
ZFUNC ZINT Zz_Compress2(ZUCHAR *pucDst, ZUINT *piDstLen, 
                const ZUCHAR *pucSrc, ZUINT iSrcLen, ZINT iLevel);

ZFUNC ZUINT Zz_Compressgz(ZUCHAR *pucDst, ZUINT *piDstLen,
                const ZUCHAR *pucSrc, ZUINT iSrcLen);

/* zlib get an upper bound on the compressed size after
   Zz_Compress() or Zz_Compress2() on dwSrcLen bytes. 

   It would be used before a Zz_Compress() or Zz_Compress2() call 
   to allocate the destination buffer.
*/
ZFUNC ZUINT Zz_CompressBound (ZUINT iSrcLen);

/* zlib decompresses the source buffer into the destination buffer
   dwSrcLen is the byte length of the source buffer.  

   Upon entry, pdwDstLen is the total size of the destination buffer, 
   which must be large enough to hold the entire uncompressed data.  
   (The size of the uncompressed data must have been saved
   previously by the compressor and transmitted to the decompressor by some
   mechanism outside the scope of this compression library.) 

   Upon exit, destLen is the actual size of the uncompressed buffer.

   uncompress returns ZOK if success, 
   ZZ_ERR_MEM if there was not enough memory, 
   ZZ_ERR_BUF if there was not enough room in the output buffer, 
   or ZZ_ERR_DATA if the input data was corrupted or incomplete.
*/
ZFUNC ZINT Zz_Uncompress(ZUCHAR *pucDst, ZUINT *piDstLen, 
                const ZUCHAR *pucSrc, ZUINT iSrcLen);

/* zlib decompresses the gzip source buffer into the destination buffer */
ZFUNC ZINT Zz_Uncompressgz(ZUCHAR *pucDst, ZUINT *piDstLen, 
                const ZUCHAR *pucSrc, ZUINT iSrcLen);

/* zlib ungzip the source data into the destination data,
    the destination data will be allocated by Zos_Malloc */
ZFUNC ZINT Zz_Ungzip(ZUCHAR *pucSrcData, ZUINT iSrcSize, 
            ZUCHAR **ppucDstData, ZUINT *piDstSize);

/* zlib ungzip the source file into the destination data,
    the destination data will be allocated by Zos_Malloc */
ZFUNC ZINT Zz_UngzipF(ZCHAR *pcFileName, ZUCHAR **ppucDstData, 
                ZUINT *piDstSize);

#ifdef __cplusplus
}
#endif

#endif /* _ZZ_H__ */

