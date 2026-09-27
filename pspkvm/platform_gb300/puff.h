#ifndef _PUFF_H_
#define _PUFF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * puff() decompresses Deflate data (RFC 1951).
 * Returns 0 on success, negative error code on failure:
 *   2: stream exhausted before end of block
 *   1: invalid block type
 *   0: success
 *  -1: not enough output space
 *  -2: invalid literal/length or distance code
 *  -3: distance too far back
 *  -4: internal error
 */
int puff(unsigned char *dest,           /* pointer to destination pointer */
         unsigned long *destlen,        /* amount of output space */
         const unsigned char *source,   /* pointer to source data pointer */
         unsigned long *sourcelen);     /* amount of input available */

#ifdef __cplusplus
}
#endif

#endif /* _PUFF_H_ */
