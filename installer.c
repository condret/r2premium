#include <stdio.h>
#include <r_util.h>
#include "fortunes.h"

int install_fortunes () {
#ifdef	R2_HOME_FORTUNES
	char *fortune_dir = r_str_home (R2_HOME_FORTUNES);
#else
	char *fortune_dir = r_xdg_datadir ("fortunes");
#endif
	if (!r_file_is_directory (fortune_dir)) {
		if (r_file_is_regular (fortune_dir)) {
			free (fortune_dir);
			eprintf ("wtf\n");
			return -1;
		} else {
			r_sys_mkdir (fortune_dir);
		}
	}
	int ret = 0;
	ut32 i;
	for (i = 0; fortunes[i]; i++) {
		char ffname[256];	//fortune file name
		snprintf (ffname, 256, "fortunes.%s", fortunes[i]);
		char *path = r_file_new (".", "fortunes", ffname, NULL);
		if (!path) {
			eprintf ("allocation failed\n");
			ret = -1;
			continue;
		}
		if (!r_file_is_regular (path)) {
			eprintf ("missing fortune file %s\n", path);
			free (path);
			ret = -1;
			continue;
		}
		r_file_copy (path, fortune_dir);
		free (path);
	}
	return ret;
}

int uninstall_fortunes () {
#ifdef	R2_HOME_FORTUNES
	char *fortune_dir = r_str_home (R2_HOME_FORTUNES);
#else
	char *fortune_dir = r_xdg_datadir ("fortunes");
#endif
	if (!r_file_is_directory (fortune_dir)) {
		return 0;
	}
	ut32 i;
	for (i = 0; fortunes[i]; i++) {
		char ffname[256];	//fortune file name
		snprintf (ffname, 256, "fortunes.%s", fortunes[i]);
		char *path = r_file_new (fortune_dir, ffname, NULL);
		if (!path) {
			eprintf ("allocation failed\n");
			continue;
		}
		if (r_file_is_regular (path)) {
			r_file_rm (path);
		}
		free (path);
	}
	return 0;
}

int install () {
	int ret = install_fortunes ();
	char *path_core_premium = r_file_new (".", "core_r2premium.so", NULL);
	if (!r_file_is_regular (path_core_premium)) {
		eprintf ("missing core_r2premium.so\n");
		free (path_core_premium);
		return -1;
	}
#ifdef	R2_HOME_PLUGINS
	char *plugin_dir = r_str_home (R2_HOME_PLUGINS);
#else
	char *plugin_dir = r_xdg_datadir ("plugins");
#endif
	if (!r_file_is_directory (plugin_dir)) {
		if (r_file_is_regular (plugin_dir)) {
			free (path_core_premium);
			free (plugin_dir);
			eprintf ("wtf2\n");
			return -1;
		} else {
			r_sys_mkdir (plugin_dir);
		}
	}
	r_file_copy (path_core_premium, plugin_dir);
	free (plugin_dir);
	free (path_core_premium);
	return ret;
}

int uninstall () {
	uninstall_fortunes ();
#ifdef	R2_HOME_PLUGINS
	char *path_core_premium = r_file_new ("~", R2_HOME_PLUGINS, "core_r2premium.so", NULL);
#else
	char *path_plugins = r_xdg_datadir ("plugins");
	char *path_core_premium = r_file_new (path_plugins, "core_r2premium.so", NULL);
	free (path_plugins);
#endif
	
	if (r_file_is_regular (path_core_premium)) {
		r_file_rm (path_core_premium);
	}
	free (path_core_premium);
	return 0;
}

int main (int argc, char *argv[]) {
	if (argc < 2 || (strcmp (argv[1], "-i") && strcmp (argv[1] , "-u"))) {
		eprintf ("Wat do?\n");
		return -1;
	}
	if (!strcmp (argv[1], "-i")) {
		return install ();
	}
	if (!strcmp (argv[1], "-u")) {
		return uninstall ();
	}
	return -1;
}
