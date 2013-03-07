#include <strong-arm/asn1.h>


uint32_t der_encode_signature (uint8_t *const dst, uint32_t max_len, FF_NUM const *const r, FF_NUM const *const s)
{
	uint32_t len_r, len_s;

	if (max_len < 2) return 0;

	dst[0] = 0x30;     // type: SEQUENCE
	dst[1] = 0x00;     // length: unknown

	if ((len_r = ff_der_serialize (dst + 2, max_len - 2, r)) == 0)
		return 0;

	if ((len_s = ff_der_serialize (dst + 2 + len_r, max_len - 2 - len_r, s)) == 0)
		return 0;

	if ((len_r + len_s) > 127)
		return 0;    // Function not designed to handle encoding lengths greater than 127

	dst[1] = len_r + len_s;

	return 2 + len_r + len_s;
}
