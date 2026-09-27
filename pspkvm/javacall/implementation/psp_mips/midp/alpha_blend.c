#include "alpha_blend.h"

// Channel masks for component blends
const javacall_pixel _sab_redmask = 0xf800;
const javacall_pixel _sab_greenmask = 0x7e0;
const javacall_pixel _sab_bluemask = 0x1f;

static inline void blend_component(unsigned char alpha_intensity,
	unsigned char n_alpha_intensity,
	javacall_pixel color_c,
	javacall_pixel tgt_cpy,
	javacall_pixel* tgt,
	javacall_pixel cmask) {
		javacall_pixel c2 = ((tgt_cpy & cmask) * n_alpha_intensity) >> 8;
		javacall_pixel c1 = (color_c * alpha_intensity) >> 8;
		javacall_pixel blend = c1+c2;
		blend &= cmask;
		*tgt |= blend;
}

void alpha_blend_smooth(unsigned char alpha_intensity,
	javacall_pixel color_r,
	javacall_pixel color_g,
	javacall_pixel color_b,
	javacall_pixel* tgt) {
		javacall_pixel tgt_cpy = * tgt;
		*tgt = 0;
		unsigned char n_alpha = 255-alpha_intensity;		
		blend_component(alpha_intensity, n_alpha, color_r, tgt_cpy, tgt, _sab_redmask);
		blend_component(alpha_intensity, n_alpha, color_g, tgt_cpy, tgt, _sab_greenmask);
		blend_component(alpha_intensity, n_alpha, color_b, tgt_cpy, tgt, _sab_bluemask);
}
