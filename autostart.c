/* Starting some programs with the Cool autostart patch */

static const char *const autostart[] = {
  "nitrogen", "--restore",  NULL,
  "spotifyd", "--no-daemon", NULL,
  //  "picom", "--config", "~/.picom.conf", "&", NULL,
  "lxsession", "&",
	NULL /* terminate */
};
