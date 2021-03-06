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
	char *path_creepy = r_file_new ("~", R2_HOME_FORTUNES, "fortunes.creepy", NULL);
	if (r_file_is_regular (path_creepy)) {
		r_file_rm (path_creepy);
	}
	free (path_creepy);
	char *path_nsfw = r_file_new ("~", R2_HOME_FORTUNES, "fortunes.nsfw", NULL);
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
