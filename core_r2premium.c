#include <r_core.h>
#include <r_hash.h>
#include <r_cons.h>
#include <r_util.h>

#define PNUS	125000

ut8 magic[] = { 0x48, 0xf0, 0x23, 0x6d, 0x7d, 0xdc, 0x5e, 1, 0xc1, 0x0b, 0x7c,
	0x31, 0x7c, 0x52, 0xb5, 0x51, 0xda, 3, 0x18, 0x0c, 0xa4, 0x28, 0x6d,
	0x12, 0xb1, 0x70, 0xd9, 0xa8, 0xf8, 0x17, 0xd6, 0xf9, 0xb2, 0xba, 0xed,
	0xb9, 0xb6, 0xd0, 0x9c, 0x2c, 0xd3, 0xa5, 0x25, 0xb6, 0xab, 0x9a, 0x2e, 0xb4 };

static int r_cmd_r2premium_call(void *user, const char *input) {
	RCore *core = (RCore *)user;
	if (!core) {
		return false;
	}
	ut32 pnl = r_config_get_b (core->config, "cfg.bigendian") ? 11: 7;
	int argc = 0;
	char **argv = r_str_argv (input, &argc);
	if (strcmp (argv[0], "pn")) {
		r_str_argv_free (argv);
		return false;
	}
	RHash *ctx = r_hash_new (true, R_HASH_SHA384);
	if (ctx) {
		ut32 i;
		for (i = 1; i < argc; i++) {
			r_hash_calculate (ctx, R_HASH_SHA384, argv[i], strlen (argv[i]));
			if (!memcmp (ctx->digest, magic, sizeof (magic) / sizeof(ut8))) {
				pnl = pnl * 2;
			}
		}
		r_hash_free (ctx);
	}
	r_cons_print ("8");
	r_cons_flush ();
	do {
		r_sys_usleep (PNUS);
		r_cons_print ("=");
		r_cons_flush ();
	} while (--pnl);
	r_cons_print ("D ");
	pnl = 3;
	do {
		r_sys_usleep (PNUS);
		r_cons_print ("~");
		r_cons_flush ();
	} while (--pnl);
	r_sys_usleep (PNUS);
	r_cons_print ("\n");
	r_sys_usleep (PNUS);
	r_cons_flush ();
beach:
	r_str_argv_free (argv);
	return true;
}

RCorePlugin r_core_plugin_r2premium = {
	.name = "r2premium",
	.desc = "r2premium core plugin",
	.license = "BSD",
	.call = r_cmd_r2premium_call,
};

#ifndef CORELIB
RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_CORE,
	.data = &r_core_plugin_r2premium,
	.version = R2_VERSION
};
#endif
