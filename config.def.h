/***
 *           __                               ______       ______  
 *          /  |                             /      \     /      \ 
 *      ____$$ | __   __   __  _____  ____  /$$$$$$  |   /$$$$$$  |
 *     /    $$ |/  | /  | /  |/     \/    \ $$ \__$$/    $$____$$ |
 *    /$$$$$$$ |$$ | $$ | $$ |$$$$$$ $$$$  |$$      \     /    $$/ 
 *    $$ |  $$ |$$ | $$ | $$ |$$ | $$ | $$ |$$$$$$$  |   /$$$$$$/  
 *    $$ \__$$ |$$ \_$$ \_$$ |$$ | $$ | $$ |$$ \__$$ |__ $$ |_____ 
 *    $$    $$ |$$   $$   $$/ $$ | $$ | $$ |$$    $$//  |$$       |
 *     $$$$$$$/  $$$$$/$$$$/  $$/  $$/  $$/  $$$$$$/ $$/ $$$$$$$$/ 
 *                                                                 
 *    falserpincess' patched dwm build.                                                            
 *    You can find this file at:
 *    https://github.com/falseprincess/dwm
 */   

#include "colors.h"
#include "autostart.c"
#include <X11/XF86keysym.h>

#define TERM "st"
#define EDITOR "emacs"

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 10;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 0;
static const Gap default_gap        = {.isgap = 1, .realgap = 10, .gappx = 10};
static const char *fonts[]          = { "dina:size=8" };
static const char dmenufont[]       = { "dina:size=8" };

/* Defining colors */
static const char *colors[][3]      = {
	/*               fg         bg         border   */
  [SchemeNorm] = { dark3, dark0_hard, dark1 }, // Normal window
  [SchemeSel] = { dark0, faded_blue, dark4, }, // Selected window
  [SchemeStatus] = { dark4, dark0_hard,  "#000000"  }, // Statusbar right 
  [SchemeTagsSel] = { dark0_hard, faded_green,  "#000000"  }, // Tagbar left selected 
  [SchemeTagsNorm] = { dark4, dark0_hard, "#000000"  }, // Tagbar left unselected 
  [SchemeInfoSel] = { dark0_hard, faded_green,  "#000000"  }, // infobar middle  selected 
  [SchemeInfoNorm] = { dark0_hard, dark0_hard,  "#000000"  }, // infobar middle  unselected 
};

/* tagging */
static const char *tags[] = { "term[1]", "www[2]", "dev[3]", "chat[4]", "virt[5]" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "discord",  NULL,       NULL,       1 << 3,            1,           -1 },
	{ "Gpick",    NULL,       NULL,       0,                 1,           -1 },
	{ "Qview",    NULL,       NULL,       0,                 1,           -1 },
	{ "Nitrogen",    NULL,    NULL,       0,                 1,           -1 },
	//	{ "Steam",    NULL,       NULL,       1 << 2,            1,           -1 },
	{ "Pavucontrol",  NULL,   NULL,       0,                 1,           -1 },
	{ "Lxappearance",  NULL,   NULL,      0,                 1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { TERM, NULL };

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },

	{ 0,                            XK_Print,  spawn,          SHCMD("gnome-screenshot -i") },

	{ 0,                            XK_F3,     spawn,          SHCMD("amixer set Master 5%+") },
	{ 0,                            XK_F2,     spawn,          SHCMD("amixer set Master 5%-") },
	{ 0,                            XK_F1,     spawn,          SHCMD("amixer set Master toggle") },

	{ 0,                            XK_F6,     spawn,          SHCMD("xbacklight -inc 10+") },
	{ 0,                            XK_F5,     spawn,          SHCMD("xbacklight -dec 10-") },

	{ MODKEY|ControlMask,           XK_s,      spawn,          SHCMD(TERM " -e spotify-tui") },
	{ MODKEY|ControlMask,           XK_h,      spawn,          SHCMD(TERM " -e htop") },

	{ MODKEY|ControlMask,           XK_d,      spawn,          SHCMD("Discord") },
	{ MODKEY|ControlMask,           XK_f,      spawn,          SHCMD("pcmanfm") },
	{ MODKEY|ControlMask,           XK_e,      spawn,          SHCMD("emacs") },
	{ MODKEY|ControlMask,           XK_b,      spawn,          SHCMD("qutebrowser") },
	
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      inplacerotate,  {.i = +1} },
	{ MODKEY|ShiftMask,             XK_k,      inplacerotate,  {.i = -1} },
	{ MODKEY|ShiftMask,             XK_h,      inplacerotate,  {.i = +2} },
	{ MODKEY|ShiftMask,             XK_l,      inplacerotate,  {.i = -2} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = GAP_TOGGLE} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_r,      quit,           {1} }, 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
