/* Add two arbitrary precision integers, each with `n` 32-bit words.
 * Returns 1 on carry, otherwise 0.
 */
uint32_t _ap_add (uint32_t *const c, uint32_t const *const a, uint32_t const *const b, uint32_t const n);


/* Subtract two arbitrary precision integers, each with `n` 32-bit words.
 * Returns 1 on borrow, otherwise 0.
 */
uint32_t _ap_sub (uint32_t *const c, uint32_t const *const a, uint32_t const *const b, uint32_t const n);


/* Multiply two 256-bit integers, returning the 512-bit result.
 */
void _ap_mul_256 (uint32_t out[static 16], uint32_t const a[static 8], uint32_t const b[static 8]);
