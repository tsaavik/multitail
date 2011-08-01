#define _LARGEFILE64_SOURCE	/* required for GLIBC to enable stat64 and friends */
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "mt.h"
#include "mem.h"
#include "help.h"
#include "term.h"
#include "utils.h"
#include "globals.h"

char *help_main_menu[] = {
		"MultiTail lets you create one or more windows in",
		"one terminal. In each window one can monitor one",
		"or more logfiles and(!)/or the output of external",
		"programs: when using multiple inputs, they get",
		"automatically merged.",
		"",
		"In the main menu one can do add/delete windows,",
		"move them, swap them, set regular expressions, set",
		"color(-schemes) etc.",
		"",
		"One can press the following keys:",
		" ^q^     quit the program",
		" ^F1^    this help",
		" ^/^     search in all windows",
		" ^shift^ + ^/^  highlight in all windows",
		" ^b^     scroll back",
		" ^B^     scroll back in ALL windows merged into",
		"       one window",
		" ^I^     toggle case sensitivity for searches",
		" ^a^     add a new window or add another file/",
		"       program to an existing window",
		" ^d^     delete a window (or a file/program from a",
		"       window)",
		" ^s^     swap the location of 2 windows",
		" ^e^     enter/edit regular expressions",
		" ^f^     enter/edit in line filter regexps",
		" ^c^     set/change colors (only works if your",
		"       terminal supports colors)",
		" ^C^     edit the r/g/b components of a color",
		" ^b^     scrollback in the buffer of a window",
		" ^v^     toggle vertical split",
		" ^0^...^9^ set a mark in a window so that you can",
		"       easily see what has changed since the last",
		"       time you looked",
		" ^R^     restart a window",
		" ^ctrl^ + ^v^  select conversion scheme(s)",
		" ^ctrl^ + ^t^  toggle subwindow before lines",
		" ^Y^     send a signal to a window (for commands)",
		" ^y^     set linewrap",
		" ^o^     wipe (clear) a window",
		" ^O^     wipe all windows",
		" ^T^     truncate a logfile",
		" ^g^     make screendump",
		" ^r^     redraw screen",
		" ^I^     toggle case sensitivity for searches",
		" ^l^     list keybindings",
		" ^i^     info",
		" ^t^     statistics",
		" ^j^     set window sizes",
		" ^z^     hide/unhide a window",
		" ^u^     hide all windows but the one selected",
		" ^U^     unhide all windows",
		" ^w^     write a script which starts multitail",
		"       with the current settings (the files that",
		"       are monitored, regular expressions, etc.)",
		" ^n^     clear the buffer of a window",
		" ^N^     clear all buffers of all windows",
		" ^m^     set the size of a buffer",
		" ^k^     enter a (limited) terminal-mode",
		" ^p^     pause all windows",
		" ^P^     (un-)pause one window",
		"",
		"",
		"",
		"^MultiTail^ " VERSION " was written by",
		"_folkert@vanheusden.com_.",
		"Check: _http://www.vanheusden.com/multitail/_",
		"for new versions.",
		NULL
	};

char *help_add_file_or_cmd[] = {
		"MultiTail lets you display logfiles and the",
		"output of commands. At this point you can select",
		"wether you want to add to the new window a file",
		"or the output of a command.",
		" ^f^     selects a file",
		" ^c^     selects a commands",
		" ^ctrl+g^     abort",
		"",
		"Commands are executed in their own virtual tty to",
		"force line-buffered output. They are executed by",
		"starting up */bin/sh -c* with as parameter the",
		"command you'll enter here.",
		NULL
	};

char *help_list_keybindings[] = {
		"This screen lists the keybindings you defined in",
		"the MultiTail configurationfile which is located",
		"in "CONFIG_FILE". The keybindings only",
		"work in the main screen.",
		"In this screen, you can press the followking",
		"keys:",
		" ^UP^    (cursor key UP) scroll up",
		" ^DOWN^  (cursor key DOWN) scroll down",
		" ^q^     abort",
		NULL
	};

char *help_enter_filename_to_monitor[] = {
		"Please enter the filename of the file you wish to",
		"monitor. If the file is not in the directory you",
		"started MultiTail from, please enter the complete",
		"pathname. As with bash, you can press the *TAB*-",
		"key for filename completion: if more then one file",
		"exists beginning with what you typed at that",
		"point, you'll get a list of files that match. With",
		"the cursor keys and the enter key you can then",
		"select the filename to use.",
		NULL
	};

char *help_enter_cmd_to_monitor[] = {
		"Please enter the command that you want to monitor",
		"with MultiTail. If the command is not in the",
		"directory you started MultiTail from, please enter",
		"the complete pathname. As with bash, you can press",
		"the *TAB* key for filename completion: if more",
		"then one file exists beginning with what you typed",
		"at that point, you'll get a list of files that",
		"match. With the cursor keys and the enter key you",
		"can then select the filename to use.",
		"",
		"Commands are executed in their own virtual tty to",
		"force line-buffered output. They are executed by",
		"starting up */bin/sh -c* with as parameter the",
		"command you'll enter here.",
		NULL
	};

char *help_add_window_merge_subwin[] = {
		"At this point there are already one or more",
		"windows on the screen. You can now indicate if",
		"you want the new file/command to be *merged* in an",
		"existing window. Lines from the logfiles/commands",
		"in a window are then interleaved as they are",
		"generated",
		"Enter ^y^ or ^n^ here or ^ctrl+g^ to abort.",
		NULL
	};

char *help_add_window_select_merge_window[] = {
		"At this point there are already one or more",
		"windows on the screen. You can now indicate if",
		"you want the new file/command to be *merged* in an",
		"existing window. Lines from the logfiles/commands",
		"in a window are then interleaved as they are",
		"generated",
		"Here, select the window to merge with.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_add_file_follow_filename[] = {
		"Here you can select wether to follow the filename",
		"(^y^) or the filedescriptor (^n^). When the file that",
		"you want to monitor gets, for example, \"rotated\"",
		"by *logrotate* every night, you want to select to",
		"follow the filename here. What this means is: if",
		"the filename gets renamed (or deleted) and a new",
		"file with the original filename is created, that",
		"new file will automatically be monitored instead",
		"of the original file.",
		NULL,
	};

char *help_ask_colors[] = {
		"Now you can select how MultiTail colors the text",
		"it displays.",
		" ^n^     do *not* use any coloring at all",
		" ^s^     this option expects that you're monitoring",
		"       a file in the syslog format, e.g: date",
		"       hostname, program, pid, message. It uses",
		"       the name of the program which logged the",
		"       message to select a color",
		" ^m^     this option evaluates the complete logged",
		"       line to select a color",
		" ^S^     this option lets you select a pre-defined",
		"       color scheme. That color scheme should be",
		"       defined in *multitail.conf* which can be",
		"       stored in your home directory or in */etc*",
		"       See the example file for details.",
		" ^f^     when selecting this option, you'll be",
		"       asked to enter the number of the field to",
		"       use for selecting a color. You'll also be",
		"       asked to enter the character that seperates",
		"       each field in a line",
		" ^ctrl+g^     abort",
		"",
		"Using colors only works if your terminal supports",
		"this.",
		NULL,
	};

char *help_colors_field_nr[] = {
		"Selecting a field works like the *cut* command:",
		"you select a delimiter and a field-nummer. The",
		"delimiter specifies where a field ends and where",
		"the next field starts. There's one important",
		"difference between MultiTail and *cut* field-",
		"delimiters can be more then one character in size.",
		"",
		"Here, please enter the number of the field which",
		"you want to use for the color-selection. The first",
		"field on a line has number 0.",
		NULL
	};

char *help_colors_field_delimiter[] = {
		"Selecting a field works like the *cut* command:",
		"you select a delimiter and a field-nummer. The",
		"delimiter specifies where a field ends and where",
		"the next field starts. There's one important",
		"difference between MultiTail and *cut* field-",
		"delimiters can be more then one character in size.",
		"",
		"Here, please enter the string that delimits each",
		"field. This delimiter must be at least one",
		"character in size.",
		NULL
	};

char *help_ask_colors_select_scheme[] = {
		"Here you can select a predefined color scheme.",
		"These color schemes are defined in *multitail.conf*",
		"which can be located in your homedirectory and in",
		"*/etc*.",
		"In this configuration file you select using",
		"regular expressions what color to use for what",
		"patterns.",
		"See the example *multitail.conf* which should've",
		"been included with the MultiTail when you",
		"installed it.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_add_window_repeat_interval[] = {
		"In the case that the external command you're",
		"monitoring with MultiTail exits, the window gets",
		"automatically closed. Press enter to indicate that",
		"that is ok with you. If you want the command to be",
		"automatically repeated instead, enter here the",
		"delay before restarting that command. The minimum",
		"is 0: in that case the program gets repeated",
		"immediately (that is: when MultiTail notices that",
		"it died!).",
		"Press enter on an empty line to abort.",
		NULL
	};

char *help_add_file_display_diff[] = {
		"In some situations (for example: when monitoring",
		"the output of '*netstat -t tcp*') you may only",
		"want to see the difference between each run. In",
		"that case press ^y^ here. To see all output every",
		"time, press ^n^.",
		"Press ^ctrl+g^ to abort.",
		"",
		"On the commandline you can select this behaviour",
		"with the ^-R^ switch.",
		NULL
	};

#define help_merge_another_window help_add_window_select_merge_window

char *help_enter_regexp_select_window[] = {
		"In this menu one can enter/edit/delete regular",
		"expressions which are used to filter what is",
		"displayed in a window."
		"First you need to select the main-window. If more",
		"then one file/command is displayed in a window,",
		"you'll get another selection box after this one",
		"where you can select a \"sub-window\".",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_enter_regexp_select_subwindow[] = {
		"The window you selected contains the output of",
		"more then one file/command. You now have to",
		"select the \"sub-window\" to edit the regular",
		"expressions for.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_regexp_menu[] = {
		"This menu has a couple of options for editing",
		"regexps ('regular expressions'):",
		" ^a^     add a new regexp (one can have a maximum",
		"       of 10 per file/command)",
		" ^e^     edit an existing regexp",
		" ^d^     delete one of the existing regexps",
		" ^D^     move an entry down, this also changes",
		"       priority of this reg.exp.",
		" ^U^     move an entry up",
		" ^r^     reset counter",
		" ^q^/^x^   leave this menu",
		NULL
	};

char *help_enter_regexp[] = {
		"Enter a regular expression. This expression will",
		"be used for filtering.",
		"A regular expression is a method for defining",
		"text-patterns. See wikipedia for more info:",
		"_http://en.wikipedia.org/wiki/Regular__expression_",
		"or read my (Dutch!) article:",
		"_http://www.vanheusden.com/misc/regexp.html_",
		"Press enter on an empty line to abort.",
		NULL
	};

char *help_negate_regexp[] = {
		"Here you select how to handle the outcome of the",
		"regular expression processing: should it match",
		"when the regexp matches, or should it match when",
		"the regexp does *not* match?",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_regexp_usage[] = {
		"Here you select what to do when a regexp matches.",
		" ^m^     show the line if the regexp matches. If",
		"       selected to \"negate\" the outcome of the",
		"       regexp, ^m^ tells MultiTail to only display",
		"       the line if the regexp does *not* match.",
		" ^c^     if the regexp matches (see also ^m^),"
		"       the line is displayed and colored",
		" ^C^     the lines are always printed but if the",
		"       regexp matches, colors are used",
		" ^B^     if a regexp matches for a line, a bell-",
		"       sound is produced",
		" ^b^     combination of ^C^ and ^B^",
		" ^x^     if a regexp matches, a command is",
		"       executed",
		" ^ctrl+g^     abort",
		NULL
	};

char *help_enter_cmd[] = {
		"Enter the command which should be executed when a",
		"line matches a regular expression.",
		"The command is executed in the background and its",
		"output is redirected to */dev/null*.",
		"The command is started as a parameter of */bin/sh -c*",
		"Press enter on an empty line to abort.",
		NULL
	};

char *help_compile_regexp_failed[] = {
		"Compilation (\"pre-processing\") of the regular",
		"expression failed.",
		"Please check the wikipedia page for the correct",
		"syntax:",
		"_http://en.wikipedia.org/wiki/Regular__expression_",
		"or read my (Dutch!) article:",
		"_http://www.vanheusden.com/misc/regexp.html_",
		NULL
	};

char *help_delete_select_window[] = {
		"You've indicated that you want to delete a window.",
		"Select the window that you want to remove.",
		"If a window consists of more then one logfile or",
		"command, you'll be asked next if you want to",
		"remove the complete window including all",
		"\"subwindows\" or just the first one (the first",
		"file/command).",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_delete_window_delete_all_subwin[] = {
		"This window contains more then one logfile /",
		"command. Here you select if you want to remove all",
		"\"subwindows\" or just the first file/command.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_delete_select_subwindow[] = {
		"This window contains more then one logfile /",
		"command. Select the file / command you wish to",
		"remove from this window.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_swap_win1[] = {
		"You have indicated that you want to swap the",
		"location (on the screen) of 2 windows.",
		"Please select the first window.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_swap_win2[] = {
		"You have indicated that you want to swap the",
		"location (on the screen) of 2 windows.",
		"Please select the second window.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_toggle_colors_select_window[] = {
		"You've indicated that you want to change the",
		"colors of a window. Please select that window.",
		"If a window consists of more then one logfile or",
		"command, you'll be asked to select the file or",
		"command in that window to change the colors for.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_toggle_colors_select_subwindow[] = {
		"This window contains more then one logfile /",
		"command. Select the file / command you wish to",
		"change the colors for.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_failed_to_start_tail[] = {
		"There was a problem starting the '*tail*'-process",
		"(in case you wanted to monitor a logfile) or",
		"there was a problem starting the command you",
		"wanted to monitor.",
		NULL
	};

char *help_window_closed[] = {
		"A window has closed. This happens when you're",
		"monitoring the output of an external program. That",
		"program has died and when that happens the",
		"window is automatically closed.",
		"On the commandline you can set with the ^-r^ or",
		"the ^-R^ that a command should be repeated when it",
		"exits. This behaviour can also be set when you add",
		"a window by pressing the ^a^-key in the main menu.",
		NULL
	};

char *help_write_script[] = {
		"This function writes a script to disk which will",
		"start MultiTail in the same state it now is. This",
		"is usefull in case you did a lot of configuring",
		"with the menu-interface, you then don't have to",
		"find out what commandline switches to set.",
		"Here you have to enter the filename for the script",
		"to generate.",
		"Press enter on an empty line to abort.",
		NULL
	};

char *help_statistics[] = {
		"In this menu one can select the window or sub-",
		"window to display the statistics off.",
		" ^r^     reset ALL counters for ALL windows",
		" ^ctrl+g^     exit window",
		NULL,
	};

char *help_statistics_popup[] = {
		"Apart from how long MultiTail has been running, it",
		"also shows a couple of statistics for each window.",
		"It also shows some statistics on the interval",
		"between each show line in each window.",
		" *average*             average interval",
		" *standard deviation*  speaks for itself",
		"",
		" ^r^     reset the counters for this window",
		" ^ctrl+g^     exit window",
		NULL
	};

char *help_set_buffering[] = {
		"In this window you can control the buffering for",
		"each window. The first question lets you select",
		"on what window you want to set the buffering",
		"parameters:"
		" ^a^     set on all windows",
		" ^o^     set on one window",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_set_buffering_store_what[] = {
		"Here you set what should be buffered.",
		" ^a^     buffer all lines that are monitored",
		" ^m^     only buffer the lines that match the",
		"       configured regular expressions. If you have",
		"       not set any regexps, everything is stored.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_enter_number_of_lines_to_store[] = {
		"Set the size of the buffer. The default size is",
		"100 lines per window.",
		"Press enter on an empty line to abort.",
		NULL
	};

char *help_set_buffering_select_window[] = {
		"Here you select the window on which you want to",
		"set the buffer limits.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_hide_window[] = {
		"Select the window that you want to (un-)hide. When",
		"a window is hidden, it is not displayed: logging",
		"and filtering continues as usual.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_set_windowsizes[] = {
		"Here you can set the sizes of windows. The width",
		"can only set for *all* windows, the height can be",
		"set for each individual window.",
		" ^m^     manage columns: add a column, set width",
		" ^h^     set the height for a window",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_set_window_width[] = {
		"When setting the window width, you actually set",
		"the width of the windows on the left. So if you",
		"set it to 20, the windows on the left are 20",
		"characters in width. The windows on the right are",
		"then what is left in width. Beware: one column is",
		"used for the dividing line.",
		"This option can only be used when you've switched",
		"on \"vertical split\" in the main menu (^v^).",
		"Press enter on an empty line to abort.",
		NULL
	};

char *help_set_window_height[] = {
		"The height of a window must be at least 2 lines.",
		"That is excluding the statusline. The statusline",
		"can be switched off using the '^-D^' commandline",
		"switch.",
		"One can only set the window height if there are at",
		"least 2 windows on the screen.",
		"When you enter '0' MultiTail will automatically",
		"set the height. The height must be either 0 or",
		"more then one.",
		"Press enter on an empty line to abort.",
		NULL
	};

char *help_set_window_height_select_window[] = {
		"You're about to set the height of a window. The",
		"first step is to select the window which you want",
		"to resize.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_terminal_mode_select_window[] = {
		"MultiTail features a small terminal. With this",
		"terminal you can send commando's to a program you",
		"are monitoring. That way, one can put a telnet-",
		"session in a window, login and start a tail-",
		"process on the remote host.",
		"Warning: this doesn't work for ssh as ssh accesses",
		"the tty you've been logged on directly.",
		NULL
	};

char *help_scrollback_help[] = {
		"The scrollback menu lets you scrollback in the",
		"buffer. The default size of the buffer is 100",
		"lines. This size can be set with the ^-m^ command-",
		"line parameter or the ^m^-key in the main menu.",
		" ^c^     set colors",
		" ^f^/^/^   search for a string in the buffer",
		" ^n^     find the next occurence",
		" ^Y^     toggle linewrap. if linewrap is disabled,",
		"       one can scroll left/right with the cursor-",
		"       keys.",
		" ^ctrl^+^t^ show (sub-)window number in front of",
		"       each line",
		" ^t^     show a list of window-nrs and what they",
		"       contain. can also display statistics.",
		" ^s^     save the buffer to a file",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_scrollback_savefile[] = {
		"Save a buffer to a file.",
		" ^a^     write all lines to the file",
		" ^m^     write only the lines to the file which",
		"       match the regular expression(s) (if any)",
		NULL
};

char *help_scrollback_savefile_enter_filename[] = {
		"Save a buffer to a file. Enter the name of the",
		"file to write to.",
		"Press enter on an empty line to abort.",
		NULL
	};

char *help_scrollback_no_mark[] = {
		"You cannot scrollback in this window as it has no",
		"buffer. Press ^m^ in the main-menu to set the size",
		"of the buffer or use the ^-m^ commandline switch.",
		NULL
	};

char *help_scrollback_select_window[] = {
		"Select the window in which you want to scrollback.",
		"Press ^ctrl+g^ to abort.",
		NULL
	};

char *help_scrollback_edit_search_string[] = {
		"Enter the string to search for in the scrollback-",
		"buffer. The search-string can be a regular",
		"expression.",
		"Press enter on an empty line to abort.",
		NULL
	};

char *help_pause_a_window[] = {
		"Select the window to (un-)pause. When a window is",
		"paused, it won't be updated until the window is",
		"un-paused. NOTHING will be discarded.",
		NULL
	};

char *help_ask_colors_select_color[] = {
		"Select the color or attribute to use.",
		NULL
	};

char *help_set_vertical_split_n_win[] = {
		"Enter the number of columns to split the terminal",
		"in. If you press 'j' in the menu, you can also",
		"select the number of windows per column.",
		NULL
	};

char *help_wipe_window[] = {
		"Press the number of the window to wipe. The number",
		"can be found in the status-line of the windows.",
		"Press 'r' to redraw the windows so that you can",
		"see their contents again.",
		NULL
	};

char *help_select_colorschemes[] = {
		"Move the cursor with the cursor keys to the a",
		"colorscheme. Then press space to select and again",
		"to unselect. Press enter to submit or ^g to abort.",
		NULL
	};

char *help_manage_cols[] = {
		"Press ^a^ to add a column, ^d^ to delete one and press",
		"^e^ to set the number of windows in a column.",
		NULL
	};

char *help_set_linewrap[] = {
		"In this menu one can set how the wrap the text in",
		"windows.",
		" ^l^  start at the left and cut of at the right",
		" ^a^  try to display everything",
		" ^r^  cut off at the left",
		" ^s^  cut right before the syslog process name",
		" ^S^  cut right after the syslog process name",
		" ^o^  enter a position where to cut",
		" Press ctrl+g to abort.",
		NULL
	};

char *help_enter_stripper_select_window[] = {
		"Select window to apply the stripping to.",
		NULL
	};

char *help_enter_stripper_select_subwindow[] = {
		"Select the sub-window to apply the stripping to.",
		NULL
	};

char *help_redirect_failed[] = {
		"Sending the data which was retrieved from a tail-",
		"process to some other process or file failed.",
		"Maybe the disk is full or the process died?",
		NULL
	};

char *help_stripper_type[] = {
		" ^e^   filter by using a regular expression",
		" ^r^   filter by specifying the character-range",
		" ^c^   specify a column to strip - you'll be asked",
		"     to enter a delimiter as well",
		NULL
	};

char *help_stripper_start_offset[] = {
		"Please enter the offset from where to strip the",
		"string. The offset starts at 0.",
		NULL
	};

char *help_stripper_end_offset[] = {
		"Please enter the offset upto where the string will",
		"be filtered. Upto, not including!",
		NULL
	};

char *help_stripper_delimiter[] = {
		"When selecting stripping by column-number you need",
		"to enter the delimiter for each column. Usually",
		"you would enter a space here.",
		NULL
	};

char *help_stripper_col_nr[] = {
		"Enter the number of the column here. The first one",
		"is at offset 0.",
		NULL
	};

char *help_error_write_script_create_file[] = {
		"There was an error writing the scriptfile. Maybe",
		"the filesystem is full or the media is damaged?",
		NULL
	};

char *help_column_width[] = {
		"Enter the number of characters that will fit in a",
		"line for this column.",
		NULL
	};

char *help_n_win_per_col[] = {
		"Enter the number of windows that will fit in this",
		"column (vertically).",
		NULL
	};

char *help_send_signal_select_window[] = {
		"Select the window to send the signal to. Please",
		"note that sending a signal may cause the process",
		"(for example the tail-process when monitoring a",
		"logfile) to exit and thereby closing the window.",
		NULL
	};

char *help_send_signal_window_send_to_all_subwin[] = {
		"Would you like to send the signal to all sub-",
		"windows?",
		NULL
	};

char *help_send_signal_select_subwindow[] = {
		"Select the subwindow to send the signal to.",
		NULL
	};

char *help_send_signal_failed[] = {
		"Sending the signal to the process failed. This",
		"should not happen.",
		NULL
	};

char *help_select_signal[] = {
		"Select the signal to send. Please note that some",
		"signals may cause the process to exit and closing",
		"the window",
		NULL
	};

char *help_screendump_select_file[] = {
		"Select the file to write the screendump to.",
		"Existing files will be overwritten.",
		NULL
	};

char *help_screendump_select_win[] = {
		"Select the window to dump to a screendump-file.",
		"The output file will be plain ascii.",
		NULL
	};

char *help_truncate_file_select_window[] = {
		"Select window. By selecting a window you select",
		"the file to truncate.",
		NULL
	};

char *help_truncate_file_select_subwindow[] = {
		"As the window you selected contains several sub-",
		"windows, you need to select now the subwindow to",
		"truncate.",
		NULL
	};

char *help_truncate_areyousure[] = {
		"Are you really sure you want to truncate this",
		"file? This cannot be undone!",
		NULL
	};

char *help_truncate_failed[] = {
		"There was an error truncating the file. Please",
		"check the permissions on the file.",
		NULL
	};

char *help_truncate_only_logfiles[] = {
		"One can only truncate logfiles and not processes.",
		NULL
	};

char *help_select_color_and_attributes[] = {
		"Select the foreground and background color to use.",
		"You can select one foreground and one background-",
		"color but you can select multiple attributes.",
		"Press the space-bar to select or un-select.",
		NULL
	};

char *help_cannot_change_color[] = {
		"Your terminal doesn't support chaning the red,",
		"green and blue values of a color.",
		NULL
	};

char *help_edit_color_edit[] = {
		"Depending on the range you selected (by pressing",
		"the cursor up and down key), you can enter a value",
		"for either red, green or blue in the range of",
		"0-1000, 0-255 or 00-ff (hex value).",
		NULL
	};

char *help_edit_color[] = {
		"In this menu one can edit the values for the red,",
		"the green and the blue component of a color.",
		"Move the '>' with the up and down cursorkeys in",
		"front of the range in which you want to enter the",
		"value. The uppermost range being 0 upto 1000, the",
		"middle 0-255 and the lowest also 0-255 but entered",
		"in hexadecimal values.",
		" ^r^   edit the red component",
		" ^g^   edit the green component",
		" ^b^   edit the blue component",
		" ^c^   give the color a name/description",
		NULL
	};

char *help_edit_color_change_name[] = {
		"One can give this specific red, green and blue",
		"values a name. Like roses-red or so. This name can",
		"then be used in a colorscheme.",
		NULL
	};

char *help_hide_but_window[] = {
		"Select which window to keep open. All other",
		"windows will be closed.",
		NULL
	};

char *help_search_in_all_windows[] = {
		"This functionality lets you search in all windows.",
		"The result will be merged into one new window.",
		NULL
	};

char *help_highlight_in_all_windows[] = {
		"This functionality lets you search in all windows.",
		"All lines with the search-string will be",
		"highlighted (displayed in reverse).",
		NULL
	};

char *help_select_conversionschemes[] = {
		"With a conversionscheme one can let MultiTail",
		"automatically convert e.g. ip-addresses, time-",
		"stamps, etc. to something more readable. In this",
		"menu you can select what conversionscheme(s) to",
		"apply to a (sub-)window.",
		NULL
	};

char *help_select_restart_window[] = {
		"Restarting a window stops- and restarts the tail-",
		"or command running underneath a (sub-)window.",
		NULL
	};

char *help_select_restart_window_all[] = {
		"This chooses wether to select all sub-windows or",
		"just one specific.",
		NULL
	};

char *help_select_terminal_emulation[] = {
		"MultiTail can emulate a terminal. That way it can",
		"display color (escape-)codes that sometimes are",
		"in (log-)files.",
		NULL
	};

char *help_clear_buffer[] = {
		"Select window for which to clear the scrollback",
		"buffer.",
		NULL
	};

char *help_history[] = {
		/*2345678901234567890123456789012345678901234567890 */
		"Select a string from this list. These strings have",
		"been used in the past in MultiTail. They are",
		"stored in a file. Check the 'history_file' entry",
		"in the configuration-file to see in what file.",
		NULL
	};

#if 0
char *help_bla[] = {
		/*2345678901234567890123456789012345678901234567890 */
		"FIXME", /* FIXME */
		NULL
	};
#endif

help_t	help[] =
{
	{ HELP_MAIN_MENU,			help_main_menu },
	{ HELP_ADD_FILE_OR_CMD,			help_add_file_or_cmd },
	{ HELP_LIST_KEYBINDINGS,		help_list_keybindings },
	{ HELP_ENTER_FILENAME_TO_MONITOR,	help_enter_filename_to_monitor },
	{ HELP_ENTER_CMD_TO_MONITOR,		help_enter_cmd_to_monitor },
	{ HELP_ADD_WINDOW_MERGE_SUBWIN,		help_add_window_merge_subwin },
	{ HELP_ADD_WINDOW_SELECT_MERGE_WINDOW,	help_add_window_select_merge_window },
	{ HELP_ADD_FILE_FOLLOW_FILENAME,	help_add_file_follow_filename },
	{ HELP_ASK_COLORS,			help_ask_colors },
	{ HELP_COLORS_FIELD_NR,			help_colors_field_nr },
	{ HELP_COLORS_FIELD_DELIMITER,		help_colors_field_delimiter },
	{ HELP_ASK_COLORS_SELECT_SCHEME,	help_ask_colors_select_scheme },
	{ HELP_ADD_WINDOW_REPEAT_INTERVAL,	help_add_window_repeat_interval },
	{ HELP_ADD_FILE_DISPLAY_DIFF,		help_add_file_display_diff },
	{ HELP_MERGE_ANOTHER_WINDOW,		help_merge_another_window },
	{ HELP_ENTER_REGEXP_SELECT_WINDOW,	help_enter_regexp_select_window },
	{ HELP_ENTER_STRIPPER_SELECT_WINDOW,	help_enter_stripper_select_window },
	{ HELP_ENTER_REGEXP_SELECT_SUBWINDOW,	help_enter_regexp_select_subwindow },
	{ HELP_ENTER_STRIPPER_SELECT_SUBWINDOW,	help_enter_stripper_select_subwindow },
	{ HELP_REGEXP_MENU,			help_regexp_menu },
	{ HELP_ENTER_REGEXP,			help_enter_regexp },
	{ HELP_NEGATE_REGEXP,			help_negate_regexp },
	{ HELP_REGEXP_USAGE,			help_regexp_usage },
	{ HELP_ENTER_CMD,			help_enter_cmd },
	{ HELP_COMPILE_REGEXP_FAILED,		help_compile_regexp_failed },
	{ HELP_DELETE_SELECT_WINDOW,		help_delete_select_window },
	{ HELP_DELETE_SELECT_SUBWINDOW,		help_delete_select_subwindow },
	{ HELP_DELETE_WINDOW_DELETE_ALL_SUBWIN,	help_delete_window_delete_all_subwin },
	{ HELP_SWAP_WIN1,			help_swap_win1 },
	{ HELP_SWAP_WIN2,			help_swap_win2 },
	{ HELP_TOGGLE_COLORS_SELECT_WINDOW,	help_toggle_colors_select_window },
	{ HELP_TOGGLE_COLORS_SELECT_SUBWINDOW,	help_toggle_colors_select_subwindow },
	{ HELP_FAILED_TO_START_TAIL,		help_failed_to_start_tail },
	{ HELP_WINDOW_CLOSED,			help_window_closed },
	{ HELP_WRITE_SCRIPT,			help_write_script },
	{ HELP_STATISTICS,			help_statistics },
	{ HELP_SET_BUFFERING,			help_set_buffering },
	{ HELP_SET_BUFFERING_STORE_WHAT,	help_set_buffering_store_what },
	{ HELP_ENTER_NUMBER_OF_LINES_TO_STORE,	help_enter_number_of_lines_to_store },
	{ HELP_SET_BUFFERING_SELECT_WINDOW, 	help_set_buffering_select_window },
	{ HELP_HIDE_WINDOW,			help_hide_window },
	{ HELP_SET_WINDOWSIZES,			help_set_windowsizes },
	{ HELP_SET_WINDOW_WIDTH,		help_set_window_width },
	{ HELP_SET_WINDOW_HEIGHT,		help_set_window_height },
	{ HELP_SET_WINDOW_HEIGHT_SELECT_WINDOW,	help_set_window_height_select_window },
	{ HELP_TERMINAL_MODE_SELECT_WINDOW,	help_terminal_mode_select_window },
	{ HELP_SCROLLBACK_HELP,			help_scrollback_help },
	{ HELP_SCROLLBACK_SAVEFILE,		help_scrollback_savefile },
	{ HELP_SCROLLBACK_NO_MARK,		help_scrollback_no_mark },
	{ HELP_SCROLLBACK_SAVEFILE_ENTER_FILENAME,	help_scrollback_savefile_enter_filename },
	{ HELP_SCROLLBACK_SELECT_WINDOW,	help_scrollback_select_window },
	{ HELP_SCROLLBACK_EDIT_SEARCH_STRING,	help_scrollback_edit_search_string },
	{ HELP_PAUSE_A_WINDOW,			help_pause_a_window },
	{ HELP_ASK_COLORS_SELECT_COLOR,		help_ask_colors_select_color },
	{ HELP_SET_VERTICAL_SPLIT_N_WIN, 	help_set_vertical_split_n_win },
	{ HELP_MANAGE_COLS,			help_manage_cols },
	{ HELP_WIPE_WINDOW,			help_wipe_window },
	{ HELP_SELECT_COLORSCHEMES,		help_select_colorschemes },
	{ HELP_SET_LINEWRAP,			help_set_linewrap },
	{ HELP_REDIRECT_FAILED,			help_redirect_failed },
	{ HELP_STRIPPER_TYPE,			help_stripper_type },
	{ HELP_STRIPPER_START_OFFSET,		help_stripper_start_offset },
	{ HELP_STRIPPER_END_OFFSET,		help_stripper_end_offset },
	{ HELP_STRIPPER_DELIMITER,		help_stripper_delimiter },
	{ HELP_STRIPPER_COL_NR,			help_stripper_col_nr },
	{ HELP_ERROR_WRITE_SCRIPT_CREATE_FILE,	help_error_write_script_create_file },
	{ HELP_COLUMN_WIDTH,			help_column_width },
	{ HELP_N_WIN_PER_COL,			help_n_win_per_col },
	{ HELP_SEND_SIGNAL_SELECT_WINDOW,	help_send_signal_select_window },
	{ HELP_SEND_SIGNAL_WINDOW_SEND_TO_ALL_SUBWIN, help_send_signal_window_send_to_all_subwin },
	{ HELP_SEND_SIGNAL_SELECT_SUBWINDOW,	help_send_signal_select_subwindow },
	{ HELP_SEND_SIGNAL_FAILED,		help_send_signal_failed },
	{ HELP_SELECT_SIGNAL,			help_select_signal },
	{ HELP_SCREENDUMP_SELECT_FILE,		help_screendump_select_file },
	{ HELP_SCREENDUMP_SELECT_WIN,		help_screendump_select_win },
	{ HELP_TRUNCATE_FILE_SELECT_WINDOW,	help_truncate_file_select_window },
	{ HELP_TRUNCATE_FILE_SELECT_SUBWINDOW,	help_truncate_file_select_subwindow },
	{ HELP_TRUNCATE_AREYOUSURE,		help_truncate_areyousure },
	{ HELP_TRUNCATE_FAILED,			help_truncate_failed },
	{ HELP_TRUNCATE_ONLY_LOGFILES,		help_truncate_only_logfiles },
	{ HELP_SELECT_COLOR_AND_ATTRIBUTES,	help_select_color_and_attributes },
	{ HELP_CANNOT_EDIT_COLOR,               help_cannot_change_color },
	{ HELP_EDIT_COLOR_EDIT,			help_edit_color_edit },
	{ HELP_EDIT_COLOR,			help_edit_color },
	{ HELP_EDIT_COLOR_CHANGE_NAME,		help_edit_color_change_name },
	{ HELP_HIDE_BUT_WINDOW,			help_hide_but_window },
	{ HELP_SEARCH_IN_ALL_WINDOWS,		help_search_in_all_windows },
	{ HELP_HIGHLIGHT_IN_ALL_WINDOWS,	help_highlight_in_all_windows },
	{ HELP_SELECT_CONVERSIONSCHEMES,	help_select_conversionschemes },
	{ HELP_SELECT_RESTART_WINDOW,		help_select_restart_window },
	{ HELP_SELECT_RESTART_WINDOW_ALL,	help_select_restart_window_all },
	{ HELP_SELECT_TERMINAL_EMULATION,	help_select_terminal_emulation },
	{ HELP_CLEAR_BUFFER,			help_clear_buffer },
	{ HELP_HISTORY,				help_history },
	{ -1, NULL }
};

void show_help(int what_help)
{
	help_t *php = NULL;
	int index = 0;
	int line_cnt = 0;
	int ppos = -1, pos = 0;

	while(help[index].nr != -1)
	{
		if (help[index].nr == what_help)
		{
			php = &help[index];
			break;
		}

		index++;
	}

	if (php)
	{
		NEWWIN *winb = create_popup(15 + 2, 50 + 4);
		NEWWIN *win  = create_popup(15    , 50    );

		while(php -> text[line_cnt]) line_cnt++;

		wattron(winb -> win, A_STANDOUT);
		if (line_cnt > 15)
			mvwprintw(winb -> win, 0, 2, "Use cursor UP/DOWN to scroll, ctrl+g to exit");
		else
			mvwprintw(winb -> win, 0, 2, "Press ctrl+g to exit");
		wattroff(winb -> win, A_STANDOUT);
		mydoupdate();

		for(;;)
		{
			int c;

			if (ppos != pos)
			{
				int loop;

				werase(win -> win);
				for(loop=pos; loop<min(pos + 15, line_cnt); loop++)
				{
					escape_print(win, loop - pos, 0, help[index].text[loop]);
				}

				ppos = pos;

				mydoupdate();
			}

			c = wait_for_keypress(-1, 0, NULL, 1);	/* -1!!! or bad recursive loops will happen! */

			if (c == KEY_UP && pos > 0)
			{
				pos--;
			}
			else if ((c == KEY_DOWN || c == 13) && pos < (line_cnt - 1))
			{
				pos++;
			}
			else if (c == KEY_PPAGE && pos >= 15)
			{
				pos -= 15;
			}
			else if ((c == KEY_NPAGE || c == ' ') && (pos + 15) < (line_cnt - 1))
			{
				pos += 15;
			}
			else if (c == KEY_HOME && pos > 0)
			{
				pos = 0;
			}
			else if (tolower(c) == 'q' || c == abort_key)
			{
				break;
			}
			else if (c == -1)
			{
				/* a window got closed */
			}
			else
			{
				wrong_key();
			}
		}

		delete_popup(win);
		delete_popup(winb);
	}
	else
	{
		LOG("no help found for: %d\n", what_help);
		wrong_key();
	}
}
