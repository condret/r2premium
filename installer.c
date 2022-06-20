#include <r_util.h>

int install () {
#if 0
	// segfault area
	char *path_creepy = r_file_new (".", "fortunes", "fortunes.creepy");
	char *path_nsfw = r_file_new (".", "fortunes", "fortunes.nsfw");
#else
	char *path_creepy = strdup (R_JOIN_2_PATHS ("fortunes", "fortunes.creepy"));
	char *path_nsfw = strdup (R_JOIN_2_PATHS ("fortunes", "fortunes.nsfw"));
#endif	
	if (!r_file_is_regular (path_creepy) || !r_file_is_regular (path_nsfw)) {
		free (path_creepy);
		free (path_nsfw);
		eprintf ("missing fortune files\n");
		return -1;
	}
	char *fortune_dir = r_str_home (R2_HOME_FORTUNES);
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
	return 0;
}

int uninstall () {
	char *path_creepy = r_str_home (R_JOIN_2_PATHS (R2_HOME_FORTUNES, "fortunes.creepy"));
	char *path_nsfw = r_str_home (R_JOIN_2_PATHS (R2_HOME_FORTUNES, "fortunes.nsfw"));
	if (r_file_is_regular (path_creepy)) {
		r_file_rm (path_creepy);
	}
	free (path_creepy);
	if (r_file_is_regular (path_nsfw)) {
		r_file_rm (path_nsfw);
	}
	free (path_nsfw);
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
