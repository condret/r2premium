#include <r_util.h>

int install () {
	char *path_creepy = r_file_new (".", "fortunes", "fortunes.creepy", NULL);
	char *path_nsfw = r_file_new (".", "fortunes", "fortunes.nsfw", NULL);
	if (!r_file_is_regular (path_creepy) || !r_file_is_regular (path_nsfw)) {
		free (path_creepy);
		free (path_nsfw);
		eprintf ("missing fortune files\n");
		return -1;
	}
#ifdef	R2_HOME_FORTUNES
	char *fortune_dir = r_str_home (R2_HOME_FORTUNES);
#else
	char *fortune_dir = r_xdg_datadir ("fortunes");
#endif
	if (!r_file_is_directory (fortune_dir)) {
		if (r_file_is_regular (fortune_dir)) {
			free (path_creepy);
			free (path_nsfw);
			free (fortune_dir);
			eprintf ("wtf\n");
			return -1;
		} else {
			r_sys_mkdir (fortune_dir);
		}
	}
	r_file_copy (path_creepy, fortune_dir);
	r_file_copy (path_nsfw, fortune_dir);
	free (fortune_dir);
	free (path_creepy);
	free (path_nsfw);
	char *path_core_premium = r_file_new (".", "core_r2premium.so", NULL);
	if (!r_file_is_regular (path_core_premium)) {
		eprintf ("missing core_r2premium.so\n");
		free (path_core_premium);
		return 0;
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
			return 0;
		} else {
			r_sys_mkdir (plugin_dir);
		}
	}
	r_file_copy (path_core_premium, plugin_dir);
	free (plugin_dir);
	free (path_core_premium);
	return 0;
}

int uninstall () {
#ifdef	R2_HOME_FORTUNES
	char *path_creepy = r_file_new ("~", R2_HOME_FORTUNES, "fortunes.creepy", NULL);
#else
	char *path_fortunes = r_xdg_datadir ("fortunes");
	char *path_creepy = r_file_new (path_fortunes, "fortunes.creepy", NULL);
#endif
	if (r_file_is_regular (path_creepy)) {
		r_file_rm (path_creepy);
	}
	free (path_creepy);
#ifdef	R2_HOME_FORTUNES
	char *path_nsfw = r_file_new ("~", R2_HOME_FORTUNES, "fortunes.nsfw", NULL);
#else
	char *path_nsfw = r_file_new (path_fortunes, "fortunes.nsfw", NULL);
	free (path_fortunes);
#endif
	if (r_file_is_regular (path_nsfw)) {
		r_file_rm (path_nsfw);
	}
	free (path_nsfw);
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
