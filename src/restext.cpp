// restext.cpp

#define STRICT
#define D3D_OVERLOADS

#include <stdio.h>
#include "struct.h"
#include "D3DEngine.h"
#include "language.h"
#include "misc.h"
#include "event.h"
#include "object.h"
#include "cbot\resource.h"
#include "restext.h"




// Donne le pointeur au moteur.

void SetEngine(CD3DEngine *engine)
{
	g_engine = engine;
}

// Donne le nom du joueur.

void SetGlobalGamerName(char *name)
{
	strcpy(g_gamerName, name);
}



typedef struct
{
	KeyRank		key;
	char		name[20];
}
KeyDesc;

static KeyDesc keyTable[15] =
{
	{ KEYRANK_LEFT,		"left;"    },
	{ KEYRANK_RIGHT,	"right;"   },
	{ KEYRANK_UP,		"up;"      },
	{ KEYRANK_DOWN,		"down;"    },
	{ KEYRANK_ROTCW,	"rotcw;"   },
	{ KEYRANK_ROTCCW,	"rotccw;"  },
	{ KEYRANK_STOP,		"stop;"    },
	{ KEYRANK_HELP,		"help;"    },
	{ KEYRANK_NEAR,		"near;"    },
	{ KEYRANK_AWAY,		"away;"    },
	{ KEYRANK_QUIT,		"quit;"    },
	{ KEYRANK_SPEED10,	"speed10;" },
	{ KEYRANK_SPEED15,	"speed15;" },
	{ KEYRANK_SPEED20,	"speed20;" },
	{ KEYRANK_SPEED30,	"speed30;" },
};

// Cherche une touche.

BOOL SearchKey(char *cmd, KeyRank &key)
{
	int		i;

	for ( i=0 ; i<15 ; i++ )
	{
		if ( strstr(cmd, keyTable[i].name) == cmd )
		{
			key = keyTable[i].key;
			return TRUE;
		}
	}
	return FALSE;
}

// Remplace les commandes "\key name;" dans un texte.

void PutKeyName(char* dst, char* src)
{
	KeyRank	key;
	char	name[500];
	int		s, d, n, res;

	s = d = 0;
	while ( src[s] != 0 )
	{
		if ( src[s+0] == '\\' &&
			 src[s+1] == 'k'  &&
			 src[s+2] == 'e'  &&
			 src[s+3] == 'y'  &&
			 src[s+4] == ' '  )
		{
			if ( SearchKey(src+s+5, key) )
			{
				res = g_engine->RetKey(key, 0);
				if ( res != 0 )
				{
					if ( GetResource(RES_KEY, res, name) )
					{
						n = 0;
						while ( name[n] != 0 )
						{
							dst[d++] = name[n++];
						}
						while ( src[s++] != ';' );
						continue;
					}
				}
			}
		}

		dst[d++] = src[s++];
	}
	dst[d++] = 0;
}


// Retourne le texte d'une ressource.

BOOL GetResource(ResType type, int num, char* text)
{
	char	buffer[500];

	if ( !GetResourceBase(type, num, buffer) )
	{
		text[0] = 0;
		return FALSE;
	}

	PutKeyName(text, buffer);
	return TRUE;
}


// Retourne le texte d'une ressource.

BOOL GetResourceBase(ResType type, int num, char* text)
{
	text[0] = 0;

#if _ENGLISH
	if ( type == RES_TEXT )
	{
		#if _EGAMES
		#if _FULL
		if ( num == RT_VERSION_ID          )  strcpy(text, "Full 1.3 /e");
		#endif
		#if _SE
		if ( num == RT_VERSION_ID          )  strcpy(text, "VW 1.3 /e");
		#endif
		#if _DEMO
		if ( num == RT_VERSION_ID          )  strcpy(text, "Demo 1.3 /e");
		#endif
		#else
		#if _FULL
		#if _EDU
		if ( num == RT_VERSION_ID          )  strcpy(text, "EDU 1.3 /e");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "1.3 /e");
		#endif
		#endif
		#if _DEMO
		if ( num == RT_VERSION_ID          )  strcpy(text, "Demo 1.3 /e");
		#endif
		#endif
		if ( num == RT_WINDOW_MAXIMIZED    )  strcpy(text, "Maximize");
		if ( num == RT_WINDOW_MINIMIZED    )  strcpy(text, "Minimize");
		if ( num == RT_WINDOW_STANDARD     )  strcpy(text, "Normal size");
		if ( num == RT_WINDOW_CLOSE        )  strcpy(text, "Close");

		if ( num == RT_KEY_OR              )  strcpy(text, " or ");

		if ( num == RT_PLAY_LISTm          )  strcpy(text, " Select a puzzle:");
		if ( num == RT_PLAY_LISTd          )  strcpy(text, " Challenges:");
		if ( num == RT_PLAY_LISTu          )  strcpy(text, " %s's workshop:");
		if ( num == RT_PLAY_LISTp          )  strcpy(text, " Prototypes:");

		if ( num == RT_SETUP_DEVICE        )  strcpy(text, " Drivers:");
		if ( num == RT_SETUP_MODE          )  strcpy(text, " Resolution:");
		if ( num == RT_SETUP_KEY1          )  strcpy(text, "1) First click on the key you want to redefine.");
		if ( num == RT_SETUP_KEY2          )  strcpy(text, "2) Then push the key you want to use instead.");

		if ( num == RT_PERSO_LIST          )  strcpy(text, "Player list:");

		#if _EGAMES
		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Do you want to quit Speedy Eggbert Mania?");
		#else
		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Do you want to quit BlupiMania?");
		#endif
		if ( num == RT_DIALOG_YES          )  strcpy(text, "Abort\\Abort the puzzle");
		if ( num == RT_DIALOG_NO           )  strcpy(text, "Continue\\Continue the current puzzlr");
		#if _EGAMES
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Quit\\Quit Speedy Eggbert Mania");
		#else
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Quit\\Quit BlupiMania");
		#endif
		if ( num == RT_DIALOG_NOQUIT       )  strcpy(text, "Continue\\Continue the game");
		if ( num == RT_DIALOG_DELGAME      )  strcpy(text, "Do you want to delete %s's progression and workshop?");
		if ( num == RT_DIALOG_DELFILE      )  strcpy(text, "Delete the existing file %s?");
		if ( num == RT_DIALOG_YESDEL       )  strcpy(text, "Delete");
		if ( num == RT_DIALOG_NODEL        )  strcpy(text, "Cancel");
		if ( num == RT_DIALOG_NEWGAME      )  strcpy(text, "Player name:");
		if ( num == RT_DIALOG_YESNEW       )  strcpy(text, "Create");
		if ( num == RT_DIALOG_NONEW        )  strcpy(text, "Cancel");
		if ( num == RT_DIALOG_PLAY         )  strcpy(text, "Start puzzle...");
		if ( num == RT_DIALOG_REPLAY       )  strcpy(text, "Play again just for fun...");
		if ( num == RT_DIALOG_QUITEDIT     )  strcpy(text, "Save modifications?");
		if ( num == RT_DIALOG_YESQUITEDIT  )  strcpy(text, "Yes\\Save and quit");
		if ( num == RT_DIALOG_NOQUITEDIT   )  strcpy(text, "No\\Quit without saving");
		if ( num == RT_DIALOG_CANQUITEDIT  )  strcpy(text, "Cancel\\Continue workshop");
		if ( num == RT_DIALOG_NEWPUZZLE    )  strcpy(text, "Do you want to create a new puzzle \"%s\"?");
		if ( num == RT_DIALOG_YESNEWP      )  strcpy(text, "Create");
		if ( num == RT_DIALOG_NONEWP       )  strcpy(text, "Cancel");
		if ( num == RT_DIALOG_DELPUZ       )  strcpy(text, "Do you want to delete the puzzle?");
		if ( num == RT_DIALOG_DELPUZd      )  strcpy(text, "(for all players)");
		if ( num == RT_DIALOG_DELPUZu      )  strcpy(text, "(only for your workshop)");
		if ( num == RT_DIALOG_YESDELPUZ    )  strcpy(text, "Delete");
		if ( num == RT_DIALOG_NODELPUZ     )  strcpy(text, "Cancel");
		if ( num == RT_DIALOG_RESUMEINFO   )  strcpy(text, "Instructions");
		if ( num == RT_DIALOG_AUTHORINFO   )  strcpy(text, "Author");
		if ( num == RT_DIALOG_SIGNINFO     )  strcpy(text, "Hints");
		if ( num == RT_DIALOG_YESINFO      )  strcpy(text, "OK");
		if ( num == RT_DIALOG_YESERROR     )  strcpy(text, "OK");
		if ( num == RT_DIALOG_EXPORTGAME   )  strcpy(text, "Where do you want do export the puzzle?");
		if ( num == RT_DIALOG_YESEXPORT    )  strcpy(text, "Export");
		if ( num == RT_DIALOG_NOEXPORT     )  strcpy(text, "Cancel");
		#if _EGAMES
		if ( num == RT_DIALOG_IMPORTGAME   )  strcpy(text, "Puzzles in \"My documents/SpeedyEggbertMania\":");
		#else
		if ( num == RT_DIALOG_IMPORTGAME   )  strcpy(text, "Puzzles in \"My documents/Blupimania2\":");
		#endif
		if ( num == RT_DIALOG_YESIMPORT    )  strcpy(text, "Import");
		if ( num == RT_DIALOG_NOIMPORT     )  strcpy(text, "Cancel");
		if ( num == RT_DIALOG_TEST         )  strcpy(text, "Test");
		if ( num == RT_DIALOG_DEFI         )  strcpy(text, "Take up the challenge...");
		if ( num == RT_DIALOG_FILE         )  strcpy(text, "File");
		if ( num == RT_DIALOG_RENAMEGAME   )  strcpy(text, "Rename puzzle file.");
		if ( num == RT_DIALOG_YESRENAME    )  strcpy(text, "Rename");
		if ( num == RT_DIALOG_NORENAME     )  strcpy(text, "Cancel");
		if ( num == RT_DIALOG_RENAMEOLD    )  strcpy(text, "Current name");
		if ( num == RT_DIALOG_RENAMENEW    )  strcpy(text, "New name");

		#if _EGAMES
		if ( num == RT_GENERIC_DEV1        )  strcpy(text, "www.egames.com");
		if ( num == RT_GENERIC_DEV2        )  strcpy(text, "www.blupi.com");
		#if _SE
		if ( num == RT_GENERIC_DEMO1       )  strcpy(text, "You have just played with the V.W. version of Speedy Eggbert Mania.");
		#else
		if ( num == RT_GENERIC_DEMO1       )  strcpy(text, "You have just played with the DEMO version of Speedy Eggbert Mania.");
		#endif
		#else
		if ( num == RT_GENERIC_DEV1        )  strcpy(text, "www.epsitec.com");
		if ( num == RT_GENERIC_DEV2        )  strcpy(text, "Developed by EPSITEC");
		if ( num == RT_GENERIC_DEMO1       )  strcpy(text, "You have just played with the DEMO version of Blupimania II.");
		#endif
		if ( num == RT_GENERIC_DEMO2       )  strcpy(text, "The full version is now available ...");
		if ( num == RT_GENERIC_DEMO3       )  strcpy(text, "... and contains 80 puzzles and 40 challenges...");
		if ( num == RT_GENERIC_DEMO4       )  strcpy(text, "... as well a built-in editor where you can create your own puzzles.");
		#if _EGAMES
		if ( num == RT_GENERIC_DEMO5       )  strcpy(text, "More details on www.egames.com!");
		#else
		if ( num == RT_GENERIC_DEMO5       )  strcpy(text, "More details on www.epsitec.com!");
		#endif

		if ( num == RT_ACTION_GOTO         )  strcpy(text, "Go to");
		if ( num == RT_ACTION_PUSH1        )  strcpy(text, "Push once");
		if ( num == RT_ACTION_PUSH2        )  strcpy(text, "Push twice");
		if ( num == RT_ACTION_PUSHx        )  strcpy(text, "Push %d times");
		if ( num == RT_ACTION_PUSHn        )  strcpy(text, "Push more");
		if ( num == RT_ACTION_PULL1        )  strcpy(text, "Pull once");
		if ( num == RT_ACTION_ROLL         )  strcpy(text, "Roll");
		if ( num == RT_ACTION_DOCKm        )  strcpy(text, "Activate");
		if ( num == RT_ACTION_DOCKm2       )  strcpy(text, "Activate twice");
		if ( num == RT_ACTION_DOCKmx       )  strcpy(text, "Activate %d times");
		if ( num == RT_ACTION_DOCKg        )  strcpy(text, "Grab/drop");
		if ( num == RT_ACTION_TRAXa        )  strcpy(text, "Move forward");
		if ( num == RT_ACTION_TRAXa2       )  strcpy(text, "Move forward twice");
		if ( num == RT_ACTION_TRAXax       )  strcpy(text, "Move forward %d times");
		if ( num == RT_ACTION_TRAXl        )  strcpy(text, "Turn right");
		if ( num == RT_ACTION_TRAXl2       )  strcpy(text, "Turn right twice");
		if ( num == RT_ACTION_TRAXlx       )  strcpy(text, "Turn right %d times");
		if ( num == RT_ACTION_TRAXr        )  strcpy(text, "Turn left");
		if ( num == RT_ACTION_TRAXr2       )  strcpy(text, "Turn left twice");
		if ( num == RT_ACTION_TRAXrx       )  strcpy(text, "Turn left %d times");
		if ( num == RT_ACTION_GUNa         )  strcpy(text, "Move forward then pull");
		if ( num == RT_ACTION_GUNa2        )  strcpy(text, "Move forward then pull twice");
		if ( num == RT_ACTION_GUNax        )  strcpy(text, "Move forward then pull %d times");
		if ( num == RT_ACTION_GUNl         )  strcpy(text, "Turn left then pull");
		if ( num == RT_ACTION_GUNl2        )  strcpy(text, "Turn left then pull twice");
		if ( num == RT_ACTION_GUNlx        )  strcpy(text, "Turn left then pull %d times");
		if ( num == RT_ACTION_GUNr         )  strcpy(text, "Turn right then pull");
		if ( num == RT_ACTION_GUNr2        )  strcpy(text, "Turn right then pull twice");
		if ( num == RT_ACTION_GUNrx        )  strcpy(text, "Turn right then pull %d times");
		if ( num == RT_ACTION_CATAPULT     )  strcpy(text, "Throw");

		if ( num == RT_ADVISE_LITGOTO      )  strcpy(text, "...");
		if ( num == RT_ADVISE_GOTOPUSH     )  strcpy(text, "...");
		if ( num == RT_ADVISE_NOSIGN       )  strcpy(text, "No hints available for this puzzle.");

		if ( num == RT_USER_HEADER         )  strcpy(text, "File\tUniverse\tTested\tInstructions");
		if ( num == RT_DEFI_HEADER         )  strcpy(text, "File\tUniverse\tAuthor\tInstructions");
		if ( num == RT_IMPORT_HEADER       )  strcpy(text, "File\tUniverse\tAuthor\tInstructions");

		#if _EGAMES
		if ( num == RT_DEMO                )  strcpy(text, "Not available in this version");
		#else
		if ( num == RT_DEMO                )  strcpy(text, "Not available in the DEMO");
		#endif

		if ( num == RT_UNIVERS0            )  strcpy(text, "Castella");
		if ( num == RT_UNIVERS1            )  strcpy(text, "Paradiso");
		if ( num == RT_UNIVERS2            )  strcpy(text, "Volcano");
		if ( num == RT_UNIVERS3            )  strcpy(text, "Mechano");
		if ( num == RT_UNIVERS4            )  strcpy(text, "Sahara");
		if ( num == RT_UNIVERS5            )  strcpy(text, "Glacia");
		if ( num == RT_UNIVERS6            )  strcpy(text, "Samba");
		if ( num == RT_UNIVERS7            )  strcpy(text, "Medieva");
		if ( num == RT_UNIVERS8            )  strcpy(text, "Vegeta");
		if ( num == RT_UNIVERS9            )  strcpy(text, "Inferno");
		if ( num == RT_UNIVERS10           )  strcpy(text, "Futura");
		if ( num == RT_UNIVERS11           )  strcpy(text, "Alibaba");
		if ( num == RT_UNIVERS12           )  strcpy(text, "Studio");
		if ( num == RT_UNIVERS13           )  strcpy(text, "Cata");
		if ( num == RT_UNIVERS14           )  strcpy(text, "Allegro");
		if ( num == RT_UNIVERS15           )  strcpy(text, "Concreto");
		if ( num == RT_UNIVERS16           )  strcpy(text, "Labo");
		if ( num == RT_UNIVERS17           )  strcpy(text, "Mystica");
		if ( num == RT_UNIVERS18           )  strcpy(text, "Fantasia");
		if ( num == RT_UNIVERS19           )  strcpy(text, "Natura");
	}

	if ( type == RES_EVENT )
	{
		if ( num == EVENT_BUTTON_OK        )  strcpy(text, "OK");
		if ( num == EVENT_BUTTON_CANCEL    )  strcpy(text, "Cancel");
		if ( num == EVENT_BUTTON_QUIT      )  strcpy(text, "Menu (\\key quit;)");

		if ( num == EVENT_DIALOG_OK        )  strcpy(text, "OK");
		if ( num == EVENT_DIALOG_CANCEL    )  strcpy(text, "Cancel");

		if ( num == EVENT_INTERFACE_PUZZLE )  strcpy(text, "Puzzles\\Progressive puzzles");
		if ( num == EVENT_INTERFACE_DEFI   )  strcpy(text, "Challenges\\Other player's puzzles");
		if ( num == EVENT_INTERFACE_USER   )  strcpy(text, "Workshop\\Create your own puzzles");
		if ( num == EVENT_INTERFACE_PROTO  )  strcpy(text, "Proto\\Prototypes under development");
		if ( num == EVENT_INTERFACE_NAME   )  strcpy(text, "Change player\\Change the current player");
		if ( num == EVENT_INTERFACE_SETUP  )  strcpy(text, "Options\\Options");
		if ( num == EVENT_INTERFACE_AGAIN  )  strcpy(text, "Restart\\Restart the puzzle from the beginning");
		#if _EGAMES
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Return to Speedy Eggbert Mania");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Quit\\Quit Speedy Eggbert Mania");
		#else
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Return to BlupiMania");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Quit\\Quit BlupiMania");
		#endif
		if ( num == EVENT_INTERFACE_BACK   )  strcpy(text, "Cancel\\Back to previous screen");
		if ( num == EVENT_INTERFACE_TERM   )  strcpy(text, "Menu\\Return to the main menu");
		if ( num == EVENT_INTERFACE_PLAY   )  strcpy(text, "Play\\Start the selected puzzle");
		if ( num == EVENT_INTERFACE_NEW    )  strcpy(text, "New\\Create a new puzzle");
		if ( num == EVENT_INTERFACE_DELETE )  strcpy(text, "Delete\\Delete the puzzle");
		if ( num == EVENT_INTERFACE_RENAME )  strcpy(text, "Rename\\Rename the puzzle");
		if ( num == EVENT_INTERFACE_EXPORT )  strcpy(text, "Export\\Export the puzzle");
		if ( num == EVENT_INTERFACE_IMPORT )  strcpy(text, "Import\\Import the puzzle");
		if ( num == EVENT_INTERFACE_EDIT   )  strcpy(text, "Modify\\Modify th puzzle");
		if ( num == EVENT_INTERFACE_OK     )  strcpy(text, "OK\\Back to previous screen");

		if ( num == EVENT_INTERFACE_EXPORTdefi )  strcpy(text, "In the challanges (for all players)");
		#if _EGAMES
		if ( num == EVENT_INTERFACE_EXPORTdoc  )  strcpy(text, "In \"My documents/SpeedyEggbertMania\" folder");
		#else
		if ( num == EVENT_INTERFACE_EXPORTdoc  )  strcpy(text, "In the \"My documents/Blupimania2\" folder");
		#endif

		if ( num == EVENT_INTERFACE_SETUPd )  strcpy(text, "Device\\Driver and resolution settings");
		if ( num == EVENT_INTERFACE_SETUPg )  strcpy(text, "Graphics\\Graphics settings");
		if ( num == EVENT_INTERFACE_SETUPp )  strcpy(text, "Game\\Game settings");
		if ( num == EVENT_INTERFACE_SETUPc )  strcpy(text, "Controls\\Keyboard, wheel and gamepad settings");
		if ( num == EVENT_INTERFACE_SETUPs )  strcpy(text, "Sound\\Volume of music and sound effects");
		if ( num == EVENT_INTERFACE_RESOL  )  strcpy(text, "Resolution");
		if ( num == EVENT_INTERFACE_FULL   )  strcpy(text, "Full screen\\Full screen or windowed mode");
		if ( num == EVENT_INTERFACE_APPLY  )  strcpy(text, "Apply changes\\Activates the changed settings");

		if ( num == EVENT_INTERFACE_SHADOW      )  strcpy(text, "Shadows\\Display shadows on the ground");
		if ( num == EVENT_INTERFACE_DIRTY       )  strcpy(text, "Dirt\\Display dirt on objects");
		if ( num == EVENT_INTERFACE_SUNBEAM     )  strcpy(text, "Sunbeams\\Display sun beams above water");
		if ( num == EVENT_INTERFACE_LENSFLARE   )  strcpy(text, "Lens flare\\Lens flare of the virtual camera");
		if ( num == EVENT_INTERFACE_DECOR       )  strcpy(text, "Decorations\\Trees, plants, animals, ducts, and other stuff.");
		if ( num == EVENT_INTERFACE_DETAIL      )  strcpy(text, "Detailed objects\\Displays objects with much more details");
		if ( num == EVENT_INTERFACE_METEO       )  strcpy(text, "Weather\\Rain and snow");
		if ( num == EVENT_INTERFACE_AMBIANCE    )  strcpy(text, "Effects on water\\");
		if ( num == EVENT_INTERFACE_EXPLOVIB    )  strcpy(text, "Screen tilting during explosions\\");
		if ( num == EVENT_INTERFACE_SPEEDSCH    )  strcpy(text, "Horizontal scroll speed\\");
		if ( num == EVENT_INTERFACE_SPEEDSCV    )  strcpy(text, "Vertical scroll speed\\");
		if ( num == EVENT_INTERFACE_MOUSESCROLL )  strcpy(text, "Mouse scrolling\\When the mouse pointer bumps against the edge of the screen.");
		if ( num == EVENT_INTERFACE_INVSCH      )  strcpy(text, "Horizontal scrolling inverted\\When the mouse pointer bumps into the edges");
		if ( num == EVENT_INTERFACE_INVSCV      )  strcpy(text, "Vertical scrolling inverted\\When the mouse pointer bumps into the edges");
		if ( num == EVENT_INTERFACE_MOVIE       )  strcpy(text, "Film sequences\\Show non interactive film  sequences");
		if ( num == EVENT_INTERFACE_HELP        )  strcpy(text, "Show hints\\Show hints after some time");
		if ( num == EVENT_INTERFACE_TOOLTIPS    )  strcpy(text, "Tooltips\\Show tooltips when the mouse pointer stops over controls");
		if ( num == EVENT_INTERFACE_NICEMOUSE   )  strcpy(text, "Mouse pointer with shadow\\");
		if ( num == EVENT_INTERFACE_ACCEL       )  strcpy(text, "Accelerate when puzzle is long to solve\\Blupi runs faster after 10 minutes in the same puzzle");
		if ( num == EVENT_INTERFACE_VOLBLUPI    )  strcpy(text, "Blupi's voices\\");
		if ( num == EVENT_INTERFACE_VOLSOUND    )  strcpy(text, "Sound effects\\");
		if ( num == EVENT_INTERFACE_VOLAMBIANCE )  strcpy(text, "Ambiant sound\\");
		if ( num == EVENT_INTERFACE_SOUND3D     )  strcpy(text, "3D sound\\3D sound");

		if ( num == EVENT_INTERFACE_KDEF   )  strcpy(text, "Default controls\\Resets all controls to default values");
		if ( num == EVENT_INTERFACE_KLEFT  )  strcpy(text, "Scroll left\\Scroll left when mouse pointer bumps against edge");
		if ( num == EVENT_INTERFACE_KRIGHT )  strcpy(text, "Scroll right\\Scroll right when mouse pointer bumps against edge");
		if ( num == EVENT_INTERFACE_KUP    )  strcpy(text, "Scroll up\\Scroll up when mouse pointer bumps against edge");
		if ( num == EVENT_INTERFACE_KDOWN  )  strcpy(text, "Scroll down\\Scroll down when mouse pointer bumps against edge");
		if ( num == EVENT_INTERFACE_KROTCW )  strcpy(text, "Rotate CW\\Rotate camera clockwise");
		if ( num == EVENT_INTERFACE_KROTCCW)  strcpy(text, "Rotate CCW\\Rotate camera counter clockwise");
		if ( num == EVENT_INTERFACE_KSTOP  )  strcpy(text, "Stop action\\Stop Blupi's current action");
		if ( num == EVENT_INTERFACE_KQUIT  )  strcpy(text, "Quit\\Quit the current puzzle");
		if ( num == EVENT_INTERFACE_KHELP  )  strcpy(text, "Instructions\\Show instructions for the current puzzle");

		if ( num == EVENT_INTERFACE_MIN    )  strcpy(text, "Lowest\\Minimum graphic quality (highest frame rate)");
		if ( num == EVENT_INTERFACE_NORM   )  strcpy(text, "Normal\\Normal graphic quality");
		if ( num == EVENT_INTERFACE_MAX    )  strcpy(text, "Highest\\Highest graphic quality (lowest frame rate)");

		if ( num == EVENT_INTERFACE_SILENT )  strcpy(text, "Silent\\No sound");
		if ( num == EVENT_INTERFACE_NOISY  )  strcpy(text, "Normal\\Normal sound volume");

		if ( num == EVENT_INTERFACE_NEDIT  )  strcpy(text, "\\New player name");
		if ( num == EVENT_INTERFACE_NOK    )  strcpy(text, "Select\\Choose the selected player");
		if ( num == EVENT_INTERFACE_NCREATE)  strcpy(text, "New\\Create a new player");
		if ( num == EVENT_INTERFACE_NDELETE)  strcpy(text, "Delete\\Delete the selected player from the list");

		if ( num == EVENT_INTERFACE_PREV   )  strcpy(text, "\\Previous screen");
		if ( num == EVENT_INTERFACE_NEXT   )  strcpy(text, "\\Next screen");
		#if _EGAMES
		if ( num == EVENT_INTERFACE_EXIT   )  strcpy(text, "\\Quit Speedy Eggbert Mania");
		#else
		if ( num == EVENT_INTERFACE_EXIT   )  strcpy(text, "\\Quit BlupiMania");
		#endif

		if ( num == EVENT_INTERFACE_UNDO   )  strcpy(text, "Undo last action");
		if ( num == EVENT_INTERFACE_STOP   )  strcpy(text, "Stop (\\key stop;)");
		if ( num == EVENT_INTERFACE_SIGN   )  strcpy(text, "Get hints");
		if ( num == EVENT_LABEL_SIGN       )  strcpy(text, "Hints");

		if ( num == EVENT_EDIT_WATER       )  strcpy(text, "Water");
		if ( num == EVENT_EDIT_GROUND      )  strcpy(text, "Ground");
		if ( num == EVENT_EDIT_OBJECT      )  strcpy(text, "Objects");
		if ( num == EVENT_EDIT_INFO        )  strcpy(text, "Informations about the puzzle");
		if ( num == EVENT_EDIT_CLEAR       )  strcpy(text, "Delete");

		if ( num == EVENT_CMD              )  strcpy(text, "Command line");
		if ( num == EVENT_SPEED            )  strcpy(text, "Game speed");
	}

	if ( type == RES_OBJECT )
	{
		if ( num == OBJECT_BLUPI           )  strcpy(text, "Blupi");
		if ( num == OBJECT_BOX1            )  strcpy(text, "Crate");
		if ( num == OBJECT_BOX2            )  strcpy(text, "Crate");
		if ( num == OBJECT_BOX3            )  strcpy(text, "Crate");
		if ( num == OBJECT_BOX4            )  strcpy(text, "Crate");
		if ( num == OBJECT_BOX5            )  strcpy(text, "Crate");
		if ( num == OBJECT_BOX6            )  strcpy(text, "Crate");
		if ( num == OBJECT_BOX7            )  strcpy(text, "Big bowl");
		if ( num == OBJECT_BOX8            )  strcpy(text, "Heavy column");
		if ( num == OBJECT_BOX9            )  strcpy(text, "Cage");
		if ( num == OBJECT_BOX10           )  strcpy(text, "Bomb");
		if ( num == OBJECT_BOX11           )  strcpy(text, "Crate with wheels");
		if ( num == OBJECT_BOX12           )  strcpy(text, "Crate with wheels");
		if ( num == OBJECT_BOX13           )  strcpy(text, "Crate on air cushion");
		if ( num == OBJECT_KEY1            )  strcpy(text, "Orange key");
		if ( num == OBJECT_KEY2            )  strcpy(text, "Blue jey");
		if ( num == OBJECT_DOOR1           )  strcpy(text, "Orange door");
		if ( num == OBJECT_DOOR2           )  strcpy(text, "Blue door");
		if ( num == OBJECT_DOOR3           )  strcpy(text, "Orange door");
		if ( num == OBJECT_DOOR4           )  strcpy(text, "Blue door");
		if ( num == OBJECT_DOCK            )  strcpy(text, "Crane");
		if ( num == OBJECT_CATAPULT        )  strcpy(text, "Catapult");
		if ( num == OBJECT_TRAX            )  strcpy(text, "Pusher");
		if ( num == OBJECT_PERFO           )  strcpy(text, "Perforator");
		if ( num == OBJECT_GUN             )  strcpy(text, "Crate blaster");
		if ( num == OBJECT_MINE            )  strcpy(text, "Mine");
		if ( num == OBJECT_GLASS1          )  strcpy(text, "Reinforced pane");
		if ( num == OBJECT_GLASS2          )  strcpy(text, "Reinforced pane");
		if ( num == OBJECT_FIOLE           )  strcpy(text, "Magic potion");
		if ( num == OBJECT_GLU             )  strcpy(text, "Glue potion");
		if ( num == OBJECT_GOAL            )  strcpy(text, "Goal");
		if ( num == OBJECT_CRAZY           )  strcpy(text, "Crazy bot");
		if ( num == OBJECT_BOT1            )  strcpy(text, "Bot 1");
		if ( num == OBJECT_BOT2            )  strcpy(text, "Bot 2");
		if ( num == OBJECT_BOT3            )  strcpy(text, "Bot 3");
		if ( num == OBJECT_BOT4            )  strcpy(text, "Bot 4");
		if ( num == OBJECT_BOT5            )  strcpy(text, "Bot 5");
	}

	if ( type == RES_ERR )
	{
		strcpy(text, "Error");
		if ( num == ERR_CMD             )  strcpy(text, "Unknown command");
		#if _EGAMES
		if ( num == ERR_INSTALL         )  strcpy(text, "Speedy Eggbert Mania has not been installed correctly.");
		if ( num == ERR_NOCD            )  strcpy(text, "Please insert the Speedy Eggbert Mania CD\nand re-run the game.");
		#else
		if ( num == ERR_INSTALL         )  strcpy(text, "BlupiMania has not been installed correctly.");
		if ( num == ERR_NOCD            )  strcpy(text, "Please insert the BlupiMania CD\nand re-run the game.");
		#endif
		if ( num == ERR_MOVE_IMPOSSIBLE )  strcpy(text, "Error in instruction move");
		if ( num == ERR_GOTO_IMPOSSIBLE )  strcpy(text, "Goto: destination inaccessible");
		if ( num == ERR_GOTO_ITER       )  strcpy(text, "Goto: destination inaccessible");
		if ( num == ERR_GOTO_BUSY       )  strcpy(text, "Goto: destination occupied");
		if ( num == ERR_FIRE_VEH        )  strcpy(text, "Bot inappropriate");
		if ( num == ERR_FIRE_ENERGY     )  strcpy(text, "Not enough energy");
		if ( num == ERR_MISSION_NOTERM  )  strcpy(text, "The puzzle has not yet been finished (push \\key help; for more details)");
		if ( num == ERR_PUZZLE_ZEROBLUPI)  strcpy(text, "There must be at least one Blupi.");
		if ( num == ERR_PUZZLE_ZEROGOAL )  strcpy(text, "There must be at leat one balloon.");
		if ( num == ERR_PUZZLE_LOGOAL   )  strcpy(text, "There are not enough balloons.");
		if ( num == ERR_PUZZLE_HIGOAL   )  strcpy(text, "There are too many balloons.");
		if ( num == ERR_PUZZLE_MAXBLUPI )  strcpy(text, "You can have at most 4 Blupis or robots per puzzle.");
		if ( num == ERR_PUZZLE_MAXGOAL  )  strcpy(text, "You can have at most 8 ballons per puzzle.");
		if ( num == ERR_PUZZLE_MAX      )  strcpy(text, "You cannot create any more objects of this type.");
		if ( num == ERR_CREATE          )  strcpy(text, "File already exist.");

		if ( num == INFO_WIN            )  strcpy(text, "<< Well done, mission accomplished >>");
		if ( num == INFO_LOST           )  strcpy(text, "<< Sorry, you have failed >>");
		if ( num == INFO_LOSTq          )  strcpy(text, "<< Sorry, mission failed >>");
	}

	if ( type == RES_CBOT )
	{
		strcpy(text, "Error");
		if ( num == TX_OPENPAR       ) strcpy(text, "Opening bracket missing");
		if ( num == TX_CLOSEPAR      ) strcpy(text, "Closing bracket missing ");
		if ( num == TX_NOTBOOL       ) strcpy(text, "The expression must return a boolean value");
		if ( num == TX_UNDEFVAR      ) strcpy(text, "Variable not declared");
		if ( num == TX_BADLEFT       ) strcpy(text, "Assignment impossible");
		if ( num == TX_ENDOF         ) strcpy(text, "Semicolon terminator missing");
		if ( num == TX_OUTCASE       ) strcpy(text, "Instruction ""case"" outside a block ""switch""");
		if ( num == TX_NOTERM        ) strcpy(text, "Instructions after the final closing brace");
		if ( num == TX_CLOSEBLK      ) strcpy(text, "End of block missing");
		if ( num == TX_ELSEWITHOUTIF ) strcpy(text, "Instruction ""else"" without corresponding ""if"" ");
		if ( num == TX_OPENBLK       ) strcpy(text, "Opening brace missing ");//d�but d'un bloc attendu?
		if ( num == TX_BADTYPE       ) strcpy(text, "Wrong type for the assignment");
		if ( num == TX_REDEFVAR      ) strcpy(text, "A variable can not be declared twice");
		if ( num == TX_BAD2TYPE      ) strcpy(text, "The types of the two operands are incompatible ");
		if ( num == TX_UNDEFCALL     ) strcpy(text, "Unknown function");
		if ( num == TX_MISDOTS       ) strcpy(text, "Sign "" : "" missing");
		if ( num == TX_WHILE         ) strcpy(text, "Keyword ""while"" missing");
		if ( num == TX_BREAK         ) strcpy(text, "Instruction ""break"" outside a loop");
		if ( num == TX_LABEL         ) strcpy(text, "A label must be followed by ""for"", ""while"", ""do"" or ""switch""");
		if ( num == TX_NOLABEL       ) strcpy(text, "This label does not exist");// Cette �tiquette n'existe pas
		if ( num == TX_NOCASE        ) strcpy(text, "Instruction ""case"" missing");
		if ( num == TX_BADNUM        ) strcpy(text, "Number missing");
		if ( num == TX_VOID          ) strcpy(text, "Void parameter");
		if ( num == TX_NOTYP         ) strcpy(text, "Type declaration missing");
		if ( num == TX_NOVAR         ) strcpy(text, "Variable name missing");
		if ( num == TX_NOFONC        ) strcpy(text, "Function name missing");
		if ( num == TX_OVERPARAM     ) strcpy(text, "Too many parameters");
		if ( num == TX_REDEF         ) strcpy(text, "Function already exists");
		if ( num == TX_LOWPARAM      ) strcpy(text, "Parameters missing ");
		if ( num == TX_BADPARAM      ) strcpy(text, "No function of this name accepts this kind of parameter");
		if ( num == TX_NUMPARAM      ) strcpy(text, "No function of this name accepts this number of parameters");
		if ( num == TX_NOITEM        ) strcpy(text, "This is not a member of this class");
		if ( num == TX_DOT           ) strcpy(text, "This object is not a member of a class");
		if ( num == TX_NOCONST       ) strcpy(text, "Appropriate constructor missing");
		if ( num == TX_REDEFCLASS    ) strcpy(text, "This class already exists");
		if ( num == TX_CLBRK         ) strcpy(text, """ ] "" missing");
		if ( num == TX_RESERVED      ) strcpy(text, "Reserved keyword of CBOT language");
		if ( num == TX_BADNEW        ) strcpy(text, "Bad argument for ""new""");
		if ( num == TX_OPBRK         ) strcpy(text, """ [ "" expected");
		if ( num == TX_BADSTRING     ) strcpy(text, "String missing");
		if ( num == TX_BADINDEX      ) strcpy(text, "Incorrect index type");
		if ( num == TX_PRIVATE       ) strcpy(text, "Private element");
		if ( num == TX_NOPUBLIC      ) strcpy(text, "Public required");
		if ( num == TX_DIVZERO       ) strcpy(text, "Dividing through zero");
		if ( num == TX_NOTINIT       ) strcpy(text, "Variable not initialized");
		if ( num == TX_BADTHROW      ) strcpy(text, "Negative value rejected by ""throw""");//C'est quoi, �a?
		if ( num == TX_NORETVAL      ) strcpy(text, "The function returned no value ");
		if ( num == TX_NORUN         ) strcpy(text, "No function running");
		if ( num == TX_NOCALL        ) strcpy(text, "Calling an unknown function");
		if ( num == TX_NOCLASS       ) strcpy(text, "This class does not exist");
		if ( num == TX_NULLPT        ) strcpy(text, "Unknown Object");
		if ( num == TX_OPNAN         ) strcpy(text, "Operation impossible with value ""nan""");
		if ( num == TX_OUTARRAY      ) strcpy(text, "Access beyond array limit");
		if ( num == TX_STACKOVER     ) strcpy(text, "Stack overflow");
		if ( num == TX_DELETEDPT     ) strcpy(text, "Illegal object");
		if ( num == TX_FILEOPEN      ) strcpy(text, "Can't open file");
		if ( num == TX_NOTOPEN       ) strcpy(text, "File not open");
		if ( num == TX_ERRREAD       ) strcpy(text, "Read error");
		if ( num == TX_ERRWRITE      ) strcpy(text, "Write error");
	}

	if ( type == RES_KEY )
	{
		if ( num == 0                   )  strcpy(text, "< none >");
		if ( num == VK_LEFT             )  strcpy(text, "Left arrow");
		if ( num == VK_RIGHT            )  strcpy(text, "Right arrow");
		if ( num == VK_UP               )  strcpy(text, "Arrow up");
		if ( num == VK_DOWN             )  strcpy(text, "Arrow down");
		if ( num == VK_CANCEL           )  strcpy(text, "Control-break");
		if ( num == VK_BACK             )  strcpy(text, "<--");
		if ( num == VK_TAB              )  strcpy(text, "Tab");
		if ( num == VK_CLEAR            )  strcpy(text, "Clear");
		if ( num == VK_RETURN           )  strcpy(text, "Enter");
		if ( num == VK_SHIFT            )  strcpy(text, "Shift");
		if ( num == VK_CONTROL          )  strcpy(text, "Ctrl");
		if ( num == VK_MENU             )  strcpy(text, "Alt");
		if ( num == VK_PAUSE            )  strcpy(text, "Pause");
		if ( num == VK_CAPITAL          )  strcpy(text, "Caps Lock");
		if ( num == VK_ESCAPE           )  strcpy(text, "Esc");
		if ( num == VK_SPACE            )  strcpy(text, "Space bar");
		if ( num == VK_PRIOR            )  strcpy(text, "Page Up");
		if ( num == VK_NEXT             )  strcpy(text, "Page Down");
		if ( num == VK_END              )  strcpy(text, "End");
		if ( num == VK_HOME             )  strcpy(text, "Home");
		if ( num == VK_SELECT           )  strcpy(text, "Select");
		if ( num == VK_EXECUTE          )  strcpy(text, "Execute");
		if ( num == VK_SNAPSHOT         )  strcpy(text, "Print Scrn");
		if ( num == VK_INSERT           )  strcpy(text, "Insert");
		if ( num == VK_DELETE           )  strcpy(text, "Delete");
		if ( num == VK_HELP             )  strcpy(text, "Help");
		if ( num == VK_LWIN             )  strcpy(text, "Left Windows");
		if ( num == VK_RWIN             )  strcpy(text, "Right Windows");
		if ( num == VK_APPS             )  strcpy(text, "Application key");
		if ( num == VK_NUMPAD0          )  strcpy(text, "NumPad 0");
		if ( num == VK_NUMPAD1          )  strcpy(text, "NumPad 1");
		if ( num == VK_NUMPAD2          )  strcpy(text, "NumPad 2");
		if ( num == VK_NUMPAD3          )  strcpy(text, "NumPad 3");
		if ( num == VK_NUMPAD4          )  strcpy(text, "NumPad 4");
		if ( num == VK_NUMPAD5          )  strcpy(text, "NumPad 5");
		if ( num == VK_NUMPAD6          )  strcpy(text, "NumPad 6");
		if ( num == VK_NUMPAD7          )  strcpy(text, "NumPad 7");
		if ( num == VK_NUMPAD8          )  strcpy(text, "NumPad 8");
		if ( num == VK_NUMPAD9          )  strcpy(text, "NumPad 9");
		if ( num == VK_MULTIPLY         )  strcpy(text, "NumPad *");
		if ( num == VK_ADD              )  strcpy(text, "NumPad +");
		if ( num == VK_SEPARATOR        )  strcpy(text, "NumPad sep");
		if ( num == VK_SUBTRACT         )  strcpy(text, "NumPad -");
		if ( num == VK_DECIMAL          )  strcpy(text, "NumPad .");
		if ( num == VK_DIVIDE           )  strcpy(text, "NumPad /");
		if ( num == VK_F1               )  strcpy(text, "F1");
		if ( num == VK_F2               )  strcpy(text, "F2");
		if ( num == VK_F3               )  strcpy(text, "F3");
		if ( num == VK_F4               )  strcpy(text, "F4");
		if ( num == VK_F5               )  strcpy(text, "F5");
		if ( num == VK_F6               )  strcpy(text, "F6");
		if ( num == VK_F7               )  strcpy(text, "F7");
		if ( num == VK_F8               )  strcpy(text, "F8");
		if ( num == VK_F9               )  strcpy(text, "F9");
		if ( num == VK_F10              )  strcpy(text, "F10");
		if ( num == VK_F11              )  strcpy(text, "F11");
		if ( num == VK_F12              )  strcpy(text, "F12");
		if ( num == VK_F13              )  strcpy(text, "F13");
		if ( num == VK_F14              )  strcpy(text, "F14");
		if ( num == VK_F15              )  strcpy(text, "F15");
		if ( num == VK_F16              )  strcpy(text, "F16");
		if ( num == VK_F17              )  strcpy(text, "F17");
		if ( num == VK_F18              )  strcpy(text, "F18");
		if ( num == VK_F19              )  strcpy(text, "F19");
		if ( num == VK_F20              )  strcpy(text, "F20");
		if ( num == VK_NUMLOCK          )  strcpy(text, "Num Lock");
		if ( num == VK_SCROLL           )  strcpy(text, "Scroll");
		if ( num == VK_ATTN             )  strcpy(text, "Attn");
		if ( num == VK_CRSEL            )  strcpy(text, "CrSel");
		if ( num == VK_EXSEL            )  strcpy(text, "ExSel");
		if ( num == VK_EREOF            )  strcpy(text, "Erase EOF");
		if ( num == VK_PLAY             )  strcpy(text, "Play");
		if ( num == VK_ZOOM             )  strcpy(text, "Zoom");
		if ( num == VK_PA1              )  strcpy(text, "PA1");
		if ( num == VK_OEM_CLEAR        )  strcpy(text, "Clear");
		if ( num == VK_BUTTON1          )  strcpy(text, "Button 1");
		if ( num == VK_BUTTON2          )  strcpy(text, "Button 2");
		if ( num == VK_BUTTON3          )  strcpy(text, "Button 3");
		if ( num == VK_BUTTON4          )  strcpy(text, "Button 4");
		if ( num == VK_BUTTON5          )  strcpy(text, "Button 5");
		if ( num == VK_BUTTON6          )  strcpy(text, "Button 6");
		if ( num == VK_BUTTON7          )  strcpy(text, "Button 7");
		if ( num == VK_BUTTON8          )  strcpy(text, "Button 8");
		if ( num == VK_BUTTON9          )  strcpy(text, "Button 9");
		if ( num == VK_BUTTON10         )  strcpy(text, "Button 10");
		if ( num == VK_BUTTON11         )  strcpy(text, "Button 11");
		if ( num == VK_BUTTON12         )  strcpy(text, "Button 12");
		if ( num == VK_BUTTON13         )  strcpy(text, "Button 13");
		if ( num == VK_BUTTON14         )  strcpy(text, "Button 14");
		if ( num == VK_BUTTON15         )  strcpy(text, "Button 15");
		if ( num == VK_BUTTON16         )  strcpy(text, "Button 16");
		if ( num == VK_BUTTON17         )  strcpy(text, "Button 17");
		if ( num == VK_BUTTON18         )  strcpy(text, "Button 18");
		if ( num == VK_BUTTON19         )  strcpy(text, "Button 19");
		if ( num == VK_BUTTON20         )  strcpy(text, "Button 20");
		if ( num == VK_BUTTON21         )  strcpy(text, "Button 21");
		if ( num == VK_BUTTON22         )  strcpy(text, "Button 22");
		if ( num == VK_BUTTON23         )  strcpy(text, "Button 23");
		if ( num == VK_BUTTON24         )  strcpy(text, "Button 24");
		if ( num == VK_BUTTON25         )  strcpy(text, "Button 25");
		if ( num == VK_BUTTON26         )  strcpy(text, "Button 26");
		if ( num == VK_BUTTON27         )  strcpy(text, "Button 27");
		if ( num == VK_BUTTON28         )  strcpy(text, "Button 28");
		if ( num == VK_BUTTON29         )  strcpy(text, "Button 29");
		if ( num == VK_BUTTON30         )  strcpy(text, "Button 30");
		if ( num == VK_BUTTON31         )  strcpy(text, "Button 31");
		if ( num == VK_BUTTON32         )  strcpy(text, "Button 32");
		if ( num == VK_WHEELUP          )  strcpy(text, "Wheel up");
		if ( num == VK_WHEELDOWN        )  strcpy(text, "Wheel down");
	}
#endif

#if _DEUTSCH
	if ( type == RES_TEXT )
	{
		#if _FULL
		#if _EDU
		if ( num == RT_VERSION_ID          )  strcpy(text, "EDU 1.3 /d");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "1.3 /d");
		#endif
		#endif
		#if _DEMO
		if ( num == RT_VERSION_ID          )  strcpy(text, "Demo 1.3 /d");
		#endif
		if ( num == RT_WINDOW_MAXIMIZED    )  strcpy(text, "Maximale Gr��e");
		if ( num == RT_WINDOW_MINIMIZED    )  strcpy(text, "Reduzierte Gr��e");
		if ( num == RT_WINDOW_STANDARD     )  strcpy(text, "Normale Gr��e");
		if ( num == RT_WINDOW_CLOSE        )  strcpy(text, "Schlie�en");

		if ( num == RT_KEY_OR              )  strcpy(text, " oder ");

		if ( num == RT_PLAY_LISTm          )  strcpy(text, " W�hlen Sie ein R�tsel :");
		if ( num == RT_PLAY_LISTd          )  strcpy(text, " D�fis :");
		if ( num == RT_PLAY_LISTu          )  strcpy(text, " %s's Werkstatt :");
		if ( num == RT_PLAY_LISTp          )  strcpy(text, " Prototypen :");

		if ( num == RT_SETUP_DEVICE        )  strcpy(text, " Display-Driver :");
		if ( num == RT_SETUP_MODE          )  strcpy(text, " Aufl�sung :");
		if ( num == RT_SETUP_KEY1          )  strcpy(text, "1) Klicken Sie auf den neu zu belegenden Befehl.");
		if ( num == RT_SETUP_KEY2          )  strcpy(text, "2) Dr�cken Sie auf die Taste, die benutzt werden soll.");

		if ( num == RT_PERSO_LIST          )  strcpy(text, "Spielerliste :");

		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Wollen Sie BlupiMania verlassen ?");
		if ( num == RT_DIALOG_YES          )  strcpy(text, "Aufgeben\\R�tsel aufgeben");
		if ( num == RT_DIALOG_NO           )  strcpy(text, "Weiterspielen\\R�tsel weiterspielen");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Beenden\\BlupiMania beenden");
		if ( num == RT_DIALOG_NOQUIT       )  strcpy(text, "Weiter spielen\\Weiter spielen");
		if ( num == RT_DIALOG_DELGAME      )  strcpy(text, "Wollen Sie den Spielstand von %s l�schen ?");
		if ( num == RT_DIALOG_DELFILE      )  strcpy(text, "Wollen Sie die Datei %s l�schen ?");
		if ( num == RT_DIALOG_YESDEL       )  strcpy(text, "L�schen");
		if ( num == RT_DIALOG_NODEL        )  strcpy(text, "Abbrechen");
		if ( num == RT_DIALOG_NEWGAME      )  strcpy(text, "Name des neuen Spielers :");
		if ( num == RT_DIALOG_YESNEW       )  strcpy(text, "OK");
		if ( num == RT_DIALOG_NONEW        )  strcpy(text, "Abbrechen");
		if ( num == RT_DIALOG_PLAY         )  strcpy(text, "R�tsel l�sen...");
		if ( num == RT_DIALOG_REPLAY       )  strcpy(text, "R�tsel nochmal spielen...");
		if ( num == RT_DIALOG_QUITEDIT     )  strcpy(text, "�nderungen speichern ?");
		if ( num == RT_DIALOG_YESQUITEDIT  )  strcpy(text, "Ja\\Speichern und beenden");
		if ( num == RT_DIALOG_NOQUITEDIT   )  strcpy(text, "Nein\\Beenden ohne zu speichern");
		if ( num == RT_DIALOG_CANQUITEDIT  )  strcpy(text, "Abbrechen\\R�tsel weiterbearbeiten");
		if ( num == RT_DIALOG_NEWPUZZLE    )  strcpy(text, "Wollen Sie ein neues R�tsel vom Typ \"%s\" erstellen ?");
		if ( num == RT_DIALOG_YESNEWP      )  strcpy(text, "Neu erstellen");
		if ( num == RT_DIALOG_NONEWP       )  strcpy(text, "Abbrechen");
		if ( num == RT_DIALOG_DELPUZ       )  strcpy(text, "Wollen Sie das R�tsel l�schen ?");
		if ( num == RT_DIALOG_DELPUZd      )  strcpy(text, "(f�r alle Spieler)");
		if ( num == RT_DIALOG_DELPUZu      )  strcpy(text, "(nur in Ihrer Werkstatt)");
		if ( num == RT_DIALOG_YESDELPUZ    )  strcpy(text, "L�schen");
		if ( num == RT_DIALOG_NODELPUZ     )  strcpy(text, "Abbrechen");
		if ( num == RT_DIALOG_RESUMEINFO   )  strcpy(text, "Anweisungen");
		if ( num == RT_DIALOG_AUTHORINFO   )  strcpy(text, "Autor");
		if ( num == RT_DIALOG_SIGNINFO     )  strcpy(text, "Hinweise");
		if ( num == RT_DIALOG_YESINFO      )  strcpy(text, "OK");
		if ( num == RT_DIALOG_YESERROR     )  strcpy(text, "OK");
		if ( num == RT_DIALOG_EXPORTGAME   )  strcpy(text, "Wohin wollen Sie das R�tsel exportieren ?");
		if ( num == RT_DIALOG_YESEXPORT    )  strcpy(text, "Exportieren");
		if ( num == RT_DIALOG_NOEXPORT     )  strcpy(text, "Abbrechen");
		if ( num == RT_DIALOG_IMPORTGAME   )  strcpy(text, "R�tsel im Ordner \"Eigene Dateien\\Blupimania2\" :");
		if ( num == RT_DIALOG_YESIMPORT    )  strcpy(text, "Importieren");
		if ( num == RT_DIALOG_NOIMPORT     )  strcpy(text, "Abbrechen");
		if ( num == RT_DIALOG_TEST         )  strcpy(text, "Testen");
		if ( num == RT_DIALOG_DEFI         )  strcpy(text, "Challenge annehmen...");
		if ( num == RT_DIALOG_FILE         )  strcpy(text, "Datei");
		if ( num == RT_DIALOG_RENAMEGAME   )  strcpy(text, "Umbenennung der R�tseldatei.");
		if ( num == RT_DIALOG_YESRENAME    )  strcpy(text, "Umbenennen");
		if ( num == RT_DIALOG_NORENAME     )  strcpy(text, "Abbrechen");
		if ( num == RT_DIALOG_RENAMEOLD    )  strcpy(text, "Aktueller Name");
		if ( num == RT_DIALOG_RENAMENEW    )  strcpy(text, "Neuer Name");

		if ( num == RT_GENERIC_DEV1        )  strcpy(text, "www.epsitec.com");
		if ( num == RT_GENERIC_DEV2        )  strcpy(text, "Entwickelt von EPSITEC");
		if ( num == RT_GENERIC_DEMO1       )  strcpy(text, "Sie haben mit der DEMO-Version von BlupiMania II gespielt.");
		if ( num == RT_GENERIC_DEMO2       )  strcpy(text, "Die Vollversion ist ab sofort erh�ltlich ...");
		if ( num == RT_GENERIC_DEMO3       )  strcpy(text, "... sie enth�lt 80 R�tsel und 40 Challenges ...");
		if ( num == RT_GENERIC_DEMO4       )  strcpy(text, "... sowie eine Werkstatt um Ihre eigenen R�tsel zu erstellen.");
		if ( num == RT_GENERIC_DEMO5       )  strcpy(text, "Mehr Details auf www.epsitec.com !");

		if ( num == RT_ACTION_GOTO         )  strcpy(text, "Dort hin gehen");
		if ( num == RT_ACTION_PUSH1        )  strcpy(text, "Einmal schieben");
		if ( num == RT_ACTION_PUSHx        )  strcpy(text, "%d mal schieben");
		if ( num == RT_ACTION_PUSHn        )  strcpy(text, "Immer schieben");
		if ( num == RT_ACTION_PULL1        )  strcpy(text, "Einmal ziehen");
		if ( num == RT_ACTION_ROLL         )  strcpy(text, "Ansto�en");
		if ( num == RT_ACTION_DOCKm        )  strcpy(text, "Bet�tigen");
		if ( num == RT_ACTION_DOCKmx       )  strcpy(text, "%d mal bet�tigen");
		if ( num == RT_ACTION_DOCKg        )  strcpy(text, "Aufnehmen/Ablegen");
		if ( num == RT_ACTION_TRAXa        )  strcpy(text, "Nach vorne");
		if ( num == RT_ACTION_TRAXax       )  strcpy(text, "%d mal nach vorne");
		if ( num == RT_ACTION_TRAXl        )  strcpy(text, "Nach rechts drehen");
		if ( num == RT_ACTION_TRAXlx       )  strcpy(text, "%d mal nach rechts drehen");
		if ( num == RT_ACTION_TRAXr        )  strcpy(text, "Nach links drehen");
		if ( num == RT_ACTION_TRAXrx       )  strcpy(text, "%d mal nach links drehen");
		if ( num == RT_ACTION_GUNa         )  strcpy(text, "Nach vorne und schie�en");
		if ( num == RT_ACTION_GUNax        )  strcpy(text, "Nach vorne und %d mal schie�en");
		if ( num == RT_ACTION_GUNl         )  strcpy(text, "Nach rechts drehen und schie�en");
		if ( num == RT_ACTION_GUNlx        )  strcpy(text, "Nach rechts drehen und %d mal schie�en");
		if ( num == RT_ACTION_GUNr         )  strcpy(text, "Nach links drehen und schie�en");
		if ( num == RT_ACTION_GUNrx        )  strcpy(text, "Nach links drehen und %d mal schie�en");
		if ( num == RT_ACTION_CATAPULT     )  strcpy(text, "Katapultieren");

		if ( num == RT_ADVISE_LITGOTO      )  strcpy(text, "...");
		if ( num == RT_ADVISE_GOTOPUSH     )  strcpy(text, "...");
		if ( num == RT_ADVISE_NOSIGN       )  strcpy(text, "Keine Hinweise f�r dieses R�tsel.");

		if ( num == RT_USER_HEADER         )  strcpy(text, "Datei\tUniversum\tGetestet\tAnweisungen");
		if ( num == RT_DEFI_HEADER         )  strcpy(text, "Datei\tUniversum\tAutor\tAnweisungen");
		if ( num == RT_IMPORT_HEADER       )  strcpy(text, "Datei\tUniversum\tAutor\tAnweisungen");

		if ( num == RT_DEMO                )  strcpy(text, "In der Demoversion nicht verf�gbar");

		if ( num == RT_UNIVERS0            )  strcpy(text, "Castella");
		if ( num == RT_UNIVERS1            )  strcpy(text, "Paradiso");
		if ( num == RT_UNIVERS2            )  strcpy(text, "Vulkano");
		if ( num == RT_UNIVERS3            )  strcpy(text, "Mechano");
		if ( num == RT_UNIVERS4            )  strcpy(text, "Sahara");
		if ( num == RT_UNIVERS5            )  strcpy(text, "Hiverna");
		if ( num == RT_UNIVERS6            )  strcpy(text, "Samba");
		if ( num == RT_UNIVERS7            )  strcpy(text, "Medieva");
		if ( num == RT_UNIVERS8            )  strcpy(text, "Vegeta");
		if ( num == RT_UNIVERS9            )  strcpy(text, "Inferno");
		if ( num == RT_UNIVERS10           )  strcpy(text, "Futura");
		if ( num == RT_UNIVERS11           )  strcpy(text, "Alibaba");
		if ( num == RT_UNIVERS12           )  strcpy(text, "Studio");
		if ( num == RT_UNIVERS13           )  strcpy(text, "Cata");
		if ( num == RT_UNIVERS14           )  strcpy(text, "Allegro");
		if ( num == RT_UNIVERS15           )  strcpy(text, "Zemento");
		if ( num == RT_UNIVERS16           )  strcpy(text, "Labo");
		if ( num == RT_UNIVERS17           )  strcpy(text, "Mystica");
		if ( num == RT_UNIVERS18           )  strcpy(text, "Fantasia");
		if ( num == RT_UNIVERS19           )  strcpy(text, "Natura");
	}

	if ( type == RES_EVENT )
	{
		if ( num == EVENT_BUTTON_OK        )  strcpy(text, "OK");
		if ( num == EVENT_BUTTON_CANCEL    )  strcpy(text, "Abbrechen");
		if ( num == EVENT_BUTTON_QUIT      )  strcpy(text, "Men� (\\key quit;)");

		if ( num == EVENT_DIALOG_OK        )  strcpy(text, "OK");
		if ( num == EVENT_DIALOG_CANCEL    )  strcpy(text, "Abbrechen");

		if ( num == EVENT_INTERFACE_PUZZLE )  strcpy(text, "R�tsel\\Standard R�tsel");
		if ( num == EVENT_INTERFACE_DEFI   )  strcpy(text, "Challenges\\R�tsel von anderen Spielern");
		if ( num == EVENT_INTERFACE_USER   )  strcpy(text, "Werkstatt\\Machen Sie Ihren eigenen R�tsel");
		if ( num == EVENT_INTERFACE_PROTO  )  strcpy(text, "Proto\\Prototypen");
		if ( num == EVENT_INTERFACE_NAME   )  strcpy(text, "Neuer Spieler\\Neuen Spieler ausw�hlen");
		if ( num == EVENT_INTERFACE_SETUP  )  strcpy(text, "Einstellungen\\S�mtliche Einstellungen f�r das Spiel");
		if ( num == EVENT_INTERFACE_AGAIN  )  strcpy(text, "Neustart\\R�tsel von vorne nocheinmal anfangen");
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Zu BlupiMania zur�ckkehren");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Beenden\\BlupiMania beenden");
		if ( num == EVENT_INTERFACE_BACK   )  strcpy(text, "Abbrechen\\Zur�ck zum Hauptmen�");
		if ( num == EVENT_INTERFACE_TERM   )  strcpy(text, "Menu\\Zur�ck zum Hauptmenu");
		if ( num == EVENT_INTERFACE_PLAY   )  strcpy(text, "Spielen\\Ausgew�hltes R�tsel jetzt spielen");
		if ( num == EVENT_INTERFACE_NEW    )  strcpy(text, "Neu\\Neues R�tsel erstellen");
		if ( num == EVENT_INTERFACE_DELETE )  strcpy(text, "L�schen\\Ausgew�hltes R�tsel l�schen");
		if ( num == EVENT_INTERFACE_RENAME )  strcpy(text, "Umbenennen\\Ausgew�hltes R�tsel umbenennen");
		if ( num == EVENT_INTERFACE_EXPORT )  strcpy(text, "Exportieren\\Ausgew�hltes R�tsel exportieren");
		if ( num == EVENT_INTERFACE_IMPORT )  strcpy(text, "Importieren\\Ein R�tsel importieren");
		if ( num == EVENT_INTERFACE_EDIT   )  strcpy(text, "Ver�ndern\\Ausgew�hltes R�tsel ver�ndern");
		if ( num == EVENT_INTERFACE_OK     )  strcpy(text, "OK\\Zur�ck zum vorhergehenden Bildschirm");

		if ( num == EVENT_INTERFACE_EXPORTdefi )  strcpy(text, "In die Challenges (f�r alle Spieler)");
		if ( num == EVENT_INTERFACE_EXPORTdoc  )  strcpy(text, "Im den Ordner \"Eigene Dateien\\Blupimania2\"");

		if ( num == EVENT_INTERFACE_SETUPd )  strcpy(text, "3D-Karte\\Treiber und 3D-Einstellungen");
		if ( num == EVENT_INTERFACE_SETUPg )  strcpy(text, "Grafik\\Grafikoptionen");
		if ( num == EVENT_INTERFACE_SETUPp )  strcpy(text, "Spiel\\Spieleinstellungen");
		if ( num == EVENT_INTERFACE_SETUPc )  strcpy(text, "Steuerung\\Einstellungen f�r die Taststur");
		if ( num == EVENT_INTERFACE_SETUPs )  strcpy(text, "Sound\\Soundeinstellungen");
		if ( num == EVENT_INTERFACE_RESOL  )  strcpy(text, "Aufl�sung");
		if ( num == EVENT_INTERFACE_FULL   )  strcpy(text, "Vollbildschirm\\Vollbildschirmmodus oder Fenstermodus");
		if ( num == EVENT_INTERFACE_APPLY  )  strcpy(text, "�nderungen �bernehmen\\�bernimmt die vorgenommennen �nderrungen");

		if ( num == EVENT_INTERFACE_SHADOW      )  strcpy(text, "Schatten\\Schlagschatten auf dem Boden");
		if ( num == EVENT_INTERFACE_DIRTY       )  strcpy(text, "Schmutz\\Schmutz auf den Gegenst�nden");
		if ( num == EVENT_INTERFACE_SUNBEAM     )  strcpy(text, "Strahlen\\Senkrechte Strahlen �ber Wasserfl�chen");
		if ( num == EVENT_INTERFACE_LENSFLARE   )  strcpy(text, "Lichtreflexe\\Lichereflexe im Objektiv der virtuellen Kamera");
		if ( num == EVENT_INTERFACE_DECOR       )  strcpy(text, "Ziergegenst�nde\\Plfanzen, B�ume, Tiere, Rohre usw.");
		if ( num == EVENT_INTERFACE_DETAIL      )  strcpy(text, "Detaillierte Gegenst�nde\\Detaillierte Darstellung bestimmter Gegenst�nde");
		if ( num == EVENT_INTERFACE_METEO       )  strcpy(text, "Wetter\\Darstellung von Regen und Schnee");
		if ( num == EVENT_INTERFACE_AMBIANCE    )  strcpy(text, "Wassereffekte\\Wellen und sonstige Effekte auf Wasseroberfl�chen");
		if ( num == EVENT_INTERFACE_EXPLOVIB    )  strcpy(text, "Beben bei Explosionen\\Die Kamera bebt bei Explosionen");
		if ( num == EVENT_INTERFACE_SPEEDSCH    )  strcpy(text, "Horizontale Scrollgeschwindigkeit\\");
		if ( num == EVENT_INTERFACE_SPEEDSCV    )  strcpy(text, "Vertikale Scrollgeschwindigkeit\\");
		if ( num == EVENT_INTERFACE_MOUSESCROLL )  strcpy(text, "Scrollen mit der Maus\\Wenn die Maus den Rand des Bildschirms erreicht wird gescrollt");

		if ( num == EVENT_INTERFACE_INVSCH      )  strcpy(text, "Horizontal scrollen invertiert\\Wenn die Maus den Rand des Bildschirms erreicht wird anders herum gescrollt");
		if ( num == EVENT_INTERFACE_INVSCV      )  strcpy(text, "Vertikal scrollen invertiert\\Wenn die Maus den Rand des Bildschirms erreicht wird anders herum gescrollt");
		if ( num == EVENT_INTERFACE_MOVIE       )  strcpy(text, "Filmsequenzen\\Filmsequenzen vor und nach den R�tseln");
		if ( num == EVENT_INTERFACE_HELP        )  strcpy(text, "Hilfstexte\\Anweisungen und Hinweise f�r die L�sung der R�tsel");
		if ( num == EVENT_INTERFACE_TOOLTIPS    )  strcpy(text, "Hilfsblasen\\Zeigt Hilfsblasen solabd die Maus �ber einer Kontrollfl�chst stehen bleibt");
		if ( num == EVENT_INTERFACE_NICEMOUSE   )  strcpy(text, "Schatten unter der Maus\\Ein Schatten erscheint unter dem Mauszeiger");
		if ( num == EVENT_INTERFACE_ACCEL       )  strcpy(text, "Beschleunigt wenn es lang dauert\\Wenn ein R�tsel nach 10 Minuten noch nicht gel�st ist, dann geht Blupi schneller");
		if ( num == EVENT_INTERFACE_VOLBLUPI    )  strcpy(text, "Laust�rke von Blupis Stimme\\");
		if ( num == EVENT_INTERFACE_VOLSOUND    )  strcpy(text, "Lautst�rke der Ger�uscheffekte\\");
		if ( num == EVENT_INTERFACE_VOLAMBIANCE )  strcpy(text, "Ger�uschkulisse\\");
		if ( num == EVENT_INTERFACE_SOUND3D     )  strcpy(text, "3D-Sound\\Schaltet die 3D-Sound der Soundkarte ein");

		if ( num == EVENT_INTERFACE_KDEF   )  strcpy(text, "Standardeinstellungen\\Benutzt wieder die Standardeinstellungen f�r die Steuerung");
		if ( num == EVENT_INTERFACE_KLEFT  )  strcpy(text, "Scroll links\\Nach links scrollen");
		if ( num == EVENT_INTERFACE_KRIGHT )  strcpy(text, "Scroll rechts\\Nach rechts scrollen");
		if ( num == EVENT_INTERFACE_KUP    )  strcpy(text, "Scroll oben\\Nach oben scrollen");
		if ( num == EVENT_INTERFACE_KDOWN  )  strcpy(text, "Scroll unten\\Nach unten scrollen");
		if ( num == EVENT_INTERFACE_KROTCW )  strcpy(text, "Nach rechts drehen\\Dreht die Kamera nach rechts");
		if ( num == EVENT_INTERFACE_KROTCCW)  strcpy(text, "Nach links drehen\\Dreht die Kamera nach links");
		if ( num == EVENT_INTERFACE_KSTOP  )  strcpy(text, "Handlung beenden\\Beendet die laufende Handlung");
		if ( num == EVENT_INTERFACE_KQUIT  )  strcpy(text, "Beenden\\R�tsel beenden");
		if ( num == EVENT_INTERFACE_KHELP  )  strcpy(text, "Anweisungen\\Zeigt die Anweisungen f�r das R�tsel");

		if ( num == EVENT_INTERFACE_MIN    )  strcpy(text, "Mini\\Minimale Grafikqualit�t (schneller)");
		if ( num == EVENT_INTERFACE_NORM   )  strcpy(text, "Normal\\Standard Grafikqualit�t");
		if ( num == EVENT_INTERFACE_MAX    )  strcpy(text, "Maxi\\H�chste Grafikqualit�t (langsamer)");

		if ( num == EVENT_INTERFACE_SILENT )  strcpy(text, "Ruhe\\Keinerlei Ger�usche mehr");
		if ( num == EVENT_INTERFACE_NOISY  )  strcpy(text, "Normal\\Normale Laust�rke");

		if ( num == EVENT_INTERFACE_NEDIT  )  strcpy(text, "\\Name des Spielers");
		if ( num == EVENT_INTERFACE_NOK    )  strcpy(text, "Ausw�hlen\\W�hlt den Spieler in der Liste aus");
		if ( num == EVENT_INTERFACE_NCREATE)  strcpy(text, "Neuer Spieler\\Erstellt einen neuen Spieler");
		if ( num == EVENT_INTERFACE_NDELETE)  strcpy(text, "Spieler l�schen\\L�scht einen Spieler aus der Liste");

		if ( num == EVENT_INTERFACE_PREV   )  strcpy(text, "\\Vorhergehender Bildschirm");
		if ( num == EVENT_INTERFACE_NEXT   )  strcpy(text, "\\N�chster Bildschirm");
		if ( num == EVENT_INTERFACE_EXIT   )  strcpy(text, "\\BlupiMania Beenden");

		if ( num == EVENT_INTERFACE_UNDO   )  strcpy(text, "Letzte Handlung widerrufen");
		if ( num == EVENT_INTERFACE_STOP   )  strcpy(text, "Laufende Handlung anhalten (\\key stop;)");
		if ( num == EVENT_INTERFACE_SIGN   )  strcpy(text, "Hinweise bekommen");
		if ( num == EVENT_LABEL_SIGN       )  strcpy(text, "Hinweise");

		if ( num == EVENT_EDIT_WATER       )  strcpy(text, "Wasser");
		if ( num == EVENT_EDIT_GROUND      )  strcpy(text, "Boden");
		if ( num == EVENT_EDIT_OBJECT      )  strcpy(text, "Gegenst�nde");
		if ( num == EVENT_EDIT_INFO        )  strcpy(text, "R�tselinfo bearbeiten");

		if ( num == EVENT_CMD              )  strcpy(text, "Befehlskonsole");
		if ( num == EVENT_SPEED            )  strcpy(text, "Spielgeschwindigkeit");
	}

	if ( type == RES_OBJECT )
	{
		if ( num == OBJECT_BLUPI           )  strcpy(text, "Blupi");
		if ( num == OBJECT_BOX1            )  strcpy(text, "Kiste");
		if ( num == OBJECT_BOX2            )  strcpy(text, "Kiste");
		if ( num == OBJECT_BOX3            )  strcpy(text, "Kiste");
		if ( num == OBJECT_BOX4            )  strcpy(text, "Kiste");
		if ( num == OBJECT_BOX5            )  strcpy(text, "Kiste");
		if ( num == OBJECT_BOX6            )  strcpy(text, "Kiste");
		if ( num == OBJECT_BOX7            )  strcpy(text, "Kugel");
		if ( num == OBJECT_BOX8            )  strcpy(text, "S�ule");
		if ( num == OBJECT_BOX9            )  strcpy(text, "K�fig");
		if ( num == OBJECT_BOX10           )  strcpy(text, "Bombe");
		if ( num == OBJECT_BOX11           )  strcpy(text, "Wagen");
		if ( num == OBJECT_BOX12           )  strcpy(text, "Wagen");
		if ( num == OBJECT_BOX13           )  strcpy(text, "Kiste mit Luftkissen");
		if ( num == OBJECT_KEY1            )  strcpy(text, "Orangener Schl�ssel");
		if ( num == OBJECT_KEY2            )  strcpy(text, "Blauer Schl�ssel");
		if ( num == OBJECT_DOOR1           )  strcpy(text, "Orangene T�r");
		if ( num == OBJECT_DOOR2           )  strcpy(text, "Blaue T�r");
		if ( num == OBJECT_DOOR3           )  strcpy(text, "Orangene T�r");
		if ( num == OBJECT_DOOR4           )  strcpy(text, "Blaue T�r");
		if ( num == OBJECT_DOCK            )  strcpy(text, "Kran");
		if ( num == OBJECT_CATAPULT        )  strcpy(text, "Katapult");
		if ( num == OBJECT_TRAX            )  strcpy(text, "Schiebemaschine");
		if ( num == OBJECT_PERFO           )  strcpy(text, "Bohrer");
		if ( num == OBJECT_GUN             )  strcpy(text, "Blaster");
		if ( num == OBJECT_MINE            )  strcpy(text, "Mine");
		if ( num == OBJECT_GLASS1          )  strcpy(text, "Fensterscheibe");
		if ( num == OBJECT_GLASS2          )  strcpy(text, "Fensterscheibe");
		if ( num == OBJECT_FIOLE           )  strcpy(text, "Zaubertrank");
		if ( num == OBJECT_GLU             )  strcpy(text, "Klebetrank");
		if ( num == OBJECT_GOAL            )  strcpy(text, "Ballon");
		if ( num == OBJECT_CRAZY           )  strcpy(text, "Ver�ckter Roboter");
		if ( num == OBJECT_BOT1            )  strcpy(text, "Bot 1");
		if ( num == OBJECT_BOT2            )  strcpy(text, "Bot 2");
		if ( num == OBJECT_BOT3            )  strcpy(text, "Bot 3");
		if ( num == OBJECT_BOT4            )  strcpy(text, "Bot 4");
		if ( num == OBJECT_BOT5            )  strcpy(text, "Bot 5");
	}

	if ( type == RES_ERR )
	{
		strcpy(text, "Erreur");
		if ( num == ERR_CMD             )  strcpy(text, "Unbekannter Befehl");
		if ( num == ERR_INSTALL         )  strcpy(text, "BlupiMania ist nicht korrekt installiert worden.");
		if ( num == ERR_NOCD            )  strcpy(text, "F�gen Sie die BlupiMania CD-Rom\nin das Laufwerk ein und starten\nSie das Spiel erneut.");
		if ( num == ERR_MOVE_IMPOSSIBLE )  strcpy(text, "Bewegung nicht m�glich");
		if ( num == ERR_GOTO_IMPOSSIBLE )  strcpy(text, "Weg kann nicht gefunden werden");
		if ( num == ERR_GOTO_ITER       )  strcpy(text, "Position nicht erreichbar");
		if ( num == ERR_GOTO_BUSY       )  strcpy(text, "Position schon besetzt");
		if ( num == ERR_FIRE_VEH        )  strcpy(text, "Roboter nicht geeignet");
		if ( num == ERR_FIRE_ENERGY     )  strcpy(text, "Nicht gen�gend Energie");
		if ( num == ERR_MISSION_NOTERM  )  strcpy(text, "Das R�tsel ist noch nicht beendet (dr�cken Sie auf \\key help; f�r mehr Details)");
		if ( num == ERR_PUZZLE_ZEROBLUPI)  strcpy(text, "Es muss mindestens eine Blupi vorhanden sein");
		if ( num == ERR_PUZZLE_ZEROGOAL )  strcpy(text, "Es muss mindestens eine Ballon vorhanden sein");
		if ( num == ERR_PUZZLE_LOGOAL   )  strcpy(text, "Es sind nicht gen�gend Ballons");
		if ( num == ERR_PUZZLE_HIGOAL   )  strcpy(text, "Es hat zu viele Ballons.");
		if ( num == ERR_PUZZLE_MAXBLUPI )  strcpy(text, "Maximal 4 Blupis oder Roboter pro R�tsel.");
		if ( num == ERR_PUZZLE_MAXGOAL  )  strcpy(text, "Maximal 8 Ballons pro R�tsel.");
		if ( num == ERR_PUZZLE_MAX      )  strcpy(text, "Es k�nnen keine Gegenst�nde dieses Typs mehr erstellt werden.");
		if ( num == ERR_CREATE          )  strcpy(text, "Diese Datei existiert schon.");

		if ( num == INFO_WIN            )  strcpy(text, "<< Bravo Sie haben das R�tsel erfolgreich gel�st >>");
		if ( num == INFO_LOST           )  strcpy(text, "<< Tut mir Leid, Sie haben versagt >>");
		if ( num == INFO_LOSTq          )  strcpy(text, "<< Tut mir Leid, Sie haben versagt >>");
	}

	if ( type == RES_CBOT )
	{
		strcpy(text, "Erreur");
		if ( num == TX_OPENPAR       ) strcpy(text, "Il manque une parenth�se ouvrante");
		if ( num == TX_CLOSEPAR      ) strcpy(text, "Il manque une parenth�se fermante");
		if ( num == TX_NOTBOOL       ) strcpy(text, "L'expression doit �tre un boolean");
		if ( num == TX_UNDEFVAR      ) strcpy(text, "Variable non d�clar�e");
		if ( num == TX_BADLEFT       ) strcpy(text, "Assignation impossible");
		if ( num == TX_ENDOF         ) strcpy(text, "Terminateur point-virgule non trouv�");
		if ( num == TX_OUTCASE       ) strcpy(text, "Instruction ""case"" hors d'un bloc ""switch""");
		if ( num == TX_NOTERM        ) strcpy(text, "Instructions apr�s la fin");
		if ( num == TX_CLOSEBLK      ) strcpy(text, "Il manque la fin du bloc");
		if ( num == TX_ELSEWITHOUTIF ) strcpy(text, "Instruction ""else"" sans ""if"" correspondant");
		if ( num == TX_OPENBLK       ) strcpy(text, "D�but d'un bloc attendu");
		if ( num == TX_BADTYPE       ) strcpy(text, "Mauvais type de r�sultat pour l'assignation");
		if ( num == TX_REDEFVAR      ) strcpy(text, "Red�finition d'une variable");
		if ( num == TX_BAD2TYPE      ) strcpy(text, "Les deux op�randes ne sont pas de types compatibles");
		if ( num == TX_UNDEFCALL     ) strcpy(text, "Routine inconnue");
		if ( num == TX_MISDOTS       ) strcpy(text, "S�parateur "" : "" attendu");
		if ( num == TX_WHILE         ) strcpy(text, "Manque le mot ""while""");
		if ( num == TX_BREAK         ) strcpy(text, "Instruction ""break"" en dehors d'une boucle");
		if ( num == TX_LABEL         ) strcpy(text, "Un label ne peut se placer que devant un ""for"", un ""while"", un ""do"" ou un ""switch""");
		if ( num == TX_NOLABEL       ) strcpy(text, "Cette �tiquette n'existe pas");
		if ( num == TX_NOCASE        ) strcpy(text, "Manque une instruction ""case""");
		if ( num == TX_BADNUM        ) strcpy(text, "Un nombre est attendu");
		if ( num == TX_VOID          ) strcpy(text, "Param�tre void");
		if ( num == TX_NOTYP         ) strcpy(text, "D�claration de type attendu");
		if ( num == TX_NOVAR         ) strcpy(text, "Nom d'une variable attendu");
		if ( num == TX_NOFONC        ) strcpy(text, "Nom de la fonction attendu");
		if ( num == TX_OVERPARAM     ) strcpy(text, "Trop de param�tres");
		if ( num == TX_REDEF         ) strcpy(text, "Cette fonction existe d�j�");
		if ( num == TX_LOWPARAM      ) strcpy(text, "Pas assez de param�tres");
		if ( num == TX_BADPARAM      ) strcpy(text, "Aucune fonction de ce nom n'accepte ce(s) type(s) de param�tre(s)");
		if ( num == TX_NUMPARAM      ) strcpy(text, "Aucune fonction de ce nom n'accepte ce nombre de param�tres");
		if ( num == TX_NOITEM        ) strcpy(text, "Cet �l�ment n'existe pas dans cette classe");
		if ( num == TX_DOT           ) strcpy(text, "L'objet n'est pas une instance d'une classe");
		if ( num == TX_NOCONST       ) strcpy(text, "Il n'y a pas de constructeur appropri�");
		if ( num == TX_REDEFCLASS    ) strcpy(text, "Cette classe existe d�j�");
		if ( num == TX_CLBRK         ) strcpy(text, """ ] "" attendu");
		if ( num == TX_RESERVED      ) strcpy(text, "Ce mot est r�serv�");
		if ( num == TX_BADNEW        ) strcpy(text, "Mauvais argument pour ""new""");
		if ( num == TX_OPBRK         ) strcpy(text, """ [ "" attendu");
		if ( num == TX_BADSTRING     ) strcpy(text, "Une cha�ne de caract�re est attendue");
		if ( num == TX_BADINDEX      ) strcpy(text, "Mauvais type d'index");
		if ( num == TX_PRIVATE       ) strcpy(text, "El�ment prot�g�");
		if ( num == TX_NOPUBLIC      ) strcpy(text, "Public requis");
		if ( num == TX_DIVZERO       ) strcpy(text, "Division par z�ro");
		if ( num == TX_NOTINIT       ) strcpy(text, "Variable non initialis�e");
		if ( num == TX_BADTHROW      ) strcpy(text, "Valeur n�gative refus�e pour ""throw""");
		if ( num == TX_NORETVAL      ) strcpy(text, "La fonction n'a pas retourn� de r�sultat");
		if ( num == TX_NORUN         ) strcpy(text, "Pas de fonction en ex�cution");
		if ( num == TX_NOCALL        ) strcpy(text, "Appel d'une fonction inexistante");
		if ( num == TX_NOCLASS       ) strcpy(text, "Cette classe n'existe pas");
		if ( num == TX_NULLPT        ) strcpy(text, "Objet n'existe pas");
		if ( num == TX_OPNAN         ) strcpy(text, "Op�ration sur un ""nan""");
		if ( num == TX_OUTARRAY      ) strcpy(text, "Acc�s hors du tableau");
		if ( num == TX_STACKOVER     ) strcpy(text, "D�bordement de la pile");
		if ( num == TX_DELETEDPT     ) strcpy(text, "Objet inaccessible");
		if ( num == TX_FILEOPEN      ) strcpy(text, "Ouverture du fichier impossible");
		if ( num == TX_NOTOPEN       ) strcpy(text, "Le fichier n'est pas ouvert");
		if ( num == TX_ERRREAD       ) strcpy(text, "Erreur � la lecture");
		if ( num == TX_ERRWRITE      ) strcpy(text, "Erreur � l'�criture");
	}

	if ( type == RES_KEY )
	{
		if ( num == 0                   )  strcpy(text, "< nicht belegt >");
		if ( num == VK_LEFT             )  strcpy(text, "Pfeil nach links");
		if ( num == VK_RIGHT            )  strcpy(text, "Pfeil nach rechts");
		if ( num == VK_UP               )  strcpy(text, "Pfeil nach oben");
		if ( num == VK_DOWN             )  strcpy(text, "Pfeil nach unten");
		if ( num == VK_CANCEL           )  strcpy(text, "Strg-Pause");
		if ( num == VK_BACK             )  strcpy(text, "<--");
		if ( num == VK_TAB              )  strcpy(text, "Tab");
		if ( num == VK_CLEAR            )  strcpy(text, "Clear");
		if ( num == VK_RETURN           )  strcpy(text, "Eingabe");
		if ( num == VK_SHIFT            )  strcpy(text, "Umschalt");
		if ( num == VK_CONTROL          )  strcpy(text, "Ctrl/Strg");
		if ( num == VK_MENU             )  strcpy(text, "Alt");
		if ( num == VK_PAUSE            )  strcpy(text, "Pause");
		if ( num == VK_CAPITAL          )  strcpy(text, "Caps Lock");
		if ( num == VK_ESCAPE           )  strcpy(text, "Esc");
		if ( num == VK_SPACE            )  strcpy(text, "Leertaste");
		if ( num == VK_PRIOR            )  strcpy(text, "Page Up");
		if ( num == VK_NEXT             )  strcpy(text, "Page Down");
		if ( num == VK_END              )  strcpy(text, "End");
		if ( num == VK_HOME             )  strcpy(text, "Home");
		if ( num == VK_SELECT           )  strcpy(text, "Select");
		if ( num == VK_EXECUTE          )  strcpy(text, "Execute");
		if ( num == VK_SNAPSHOT         )  strcpy(text, "Print Scrn");
		if ( num == VK_INSERT           )  strcpy(text, "Insert");
		if ( num == VK_DELETE           )  strcpy(text, "Delete");
		if ( num == VK_HELP             )  strcpy(text, "Help");
		if ( num == VK_LWIN             )  strcpy(text, "Left Windows");
		if ( num == VK_RWIN             )  strcpy(text, "Right Windows");
		if ( num == VK_APPS             )  strcpy(text, "Application key");
		if ( num == VK_NUMPAD0          )  strcpy(text, "NumPad 0");
		if ( num == VK_NUMPAD1          )  strcpy(text, "NumPad 1");
		if ( num == VK_NUMPAD2          )  strcpy(text, "NumPad 2");
		if ( num == VK_NUMPAD3          )  strcpy(text, "NumPad 3");
		if ( num == VK_NUMPAD4          )  strcpy(text, "NumPad 4");
		if ( num == VK_NUMPAD5          )  strcpy(text, "NumPad 5");
		if ( num == VK_NUMPAD6          )  strcpy(text, "NumPad 6");
		if ( num == VK_NUMPAD7          )  strcpy(text, "NumPad 7");
		if ( num == VK_NUMPAD8          )  strcpy(text, "NumPad 8");
		if ( num == VK_NUMPAD9          )  strcpy(text, "NumPad 9");
		if ( num == VK_MULTIPLY         )  strcpy(text, "NumPad *");
		if ( num == VK_ADD              )  strcpy(text, "NumPad +");
		if ( num == VK_SEPARATOR        )  strcpy(text, "NumPad sep");
		if ( num == VK_SUBTRACT         )  strcpy(text, "NumPad -");
		if ( num == VK_DECIMAL          )  strcpy(text, "NumPad .");
		if ( num == VK_DIVIDE           )  strcpy(text, "NumPad /");
		if ( num == VK_F1               )  strcpy(text, "F1");
		if ( num == VK_F2               )  strcpy(text, "F2");
		if ( num == VK_F3               )  strcpy(text, "F3");
		if ( num == VK_F4               )  strcpy(text, "F4");
		if ( num == VK_F5               )  strcpy(text, "F5");
		if ( num == VK_F6               )  strcpy(text, "F6");
		if ( num == VK_F7               )  strcpy(text, "F7");
		if ( num == VK_F8               )  strcpy(text, "F8");
		if ( num == VK_F9               )  strcpy(text, "F9");
		if ( num == VK_F10              )  strcpy(text, "F10");
		if ( num == VK_F11              )  strcpy(text, "F11");
		if ( num == VK_F12              )  strcpy(text, "F12");
		if ( num == VK_F13              )  strcpy(text, "F13");
		if ( num == VK_F14              )  strcpy(text, "F14");
		if ( num == VK_F15              )  strcpy(text, "F15");
		if ( num == VK_F16              )  strcpy(text, "F16");
		if ( num == VK_F17              )  strcpy(text, "F17");
		if ( num == VK_F18              )  strcpy(text, "F18");
		if ( num == VK_F19              )  strcpy(text, "F19");
		if ( num == VK_F20              )  strcpy(text, "F20");
		if ( num == VK_NUMLOCK          )  strcpy(text, "Num Lock");
		if ( num == VK_SCROLL           )  strcpy(text, "Scroll");
		if ( num == VK_ATTN             )  strcpy(text, "Attn");
		if ( num == VK_CRSEL            )  strcpy(text, "CrSel");
		if ( num == VK_EXSEL            )  strcpy(text, "ExSel");
		if ( num == VK_EREOF            )  strcpy(text, "Erase EOF");
		if ( num == VK_PLAY             )  strcpy(text, "Play");
		if ( num == VK_ZOOM             )  strcpy(text, "Zoom");
		if ( num == VK_PA1              )  strcpy(text, "PA1");
		if ( num == VK_OEM_CLEAR        )  strcpy(text, "Clear");
		if ( num == VK_BUTTON1          )  strcpy(text, "Knopf 1");
		if ( num == VK_BUTTON2          )  strcpy(text, "Knopf 2");
		if ( num == VK_BUTTON3          )  strcpy(text, "Knopf 3");
		if ( num == VK_BUTTON4          )  strcpy(text, "Knopf 4");
		if ( num == VK_BUTTON5          )  strcpy(text, "Knopf 5");
		if ( num == VK_BUTTON6          )  strcpy(text, "Knopf 6");
		if ( num == VK_BUTTON7          )  strcpy(text, "Knopf 7");
		if ( num == VK_BUTTON8          )  strcpy(text, "Knopf 8");
		if ( num == VK_BUTTON9          )  strcpy(text, "Knopf 9");
		if ( num == VK_BUTTON10         )  strcpy(text, "Knopf 10");
		if ( num == VK_BUTTON11         )  strcpy(text, "Knopf 11");
		if ( num == VK_BUTTON12         )  strcpy(text, "Knopf 12");
		if ( num == VK_BUTTON13         )  strcpy(text, "Knopf 13");
		if ( num == VK_BUTTON14         )  strcpy(text, "Knopf 14");
		if ( num == VK_BUTTON15         )  strcpy(text, "Knopf 15");
		if ( num == VK_BUTTON16         )  strcpy(text, "Knopf 16");
		if ( num == VK_BUTTON17         )  strcpy(text, "Knopf 17");
		if ( num == VK_BUTTON18         )  strcpy(text, "Knopf 18");
		if ( num == VK_BUTTON19         )  strcpy(text, "Knopf 19");
		if ( num == VK_BUTTON20         )  strcpy(text, "Knopf 20");
		if ( num == VK_BUTTON21         )  strcpy(text, "Knopf 21");
		if ( num == VK_BUTTON22         )  strcpy(text, "Knopf 22");
		if ( num == VK_BUTTON23         )  strcpy(text, "Knopf 23");
		if ( num == VK_BUTTON24         )  strcpy(text, "Knopf 24");
		if ( num == VK_BUTTON25         )  strcpy(text, "Knopf 25");
		if ( num == VK_BUTTON26         )  strcpy(text, "Knopf 26");
		if ( num == VK_BUTTON27         )  strcpy(text, "Knopf 27");
		if ( num == VK_BUTTON28         )  strcpy(text, "Knopf 28");
		if ( num == VK_BUTTON29         )  strcpy(text, "Knopf 29");
		if ( num == VK_BUTTON30         )  strcpy(text, "Knopf 30");
		if ( num == VK_BUTTON31         )  strcpy(text, "Knopf 31");
		if ( num == VK_BUTTON32         )  strcpy(text, "Knopf 32");
		if ( num == VK_WHEELUP          )  strcpy(text, "Mausrad nach oben");
		if ( num == VK_WHEELDOWN        )  strcpy(text, "Mausrad nach unten");
	}
#endif

#if _FRENCH
	if ( type == RES_TEXT )
	{
		#if _FULL
		#if _EDU
		if ( num == RT_VERSION_ID          )  strcpy(text, "EDU 1.3 /f");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "1.3 /f");
		#endif
		#endif
		#if _DEMO
		if ( num == RT_VERSION_ID          )  strcpy(text, "Demo 1.3 /f");
		#endif
		if ( num == RT_WINDOW_MAXIMIZED    )  strcpy(text, "Taille maximale");
		if ( num == RT_WINDOW_MINIMIZED    )  strcpy(text, "Taille r�duite");
		if ( num == RT_WINDOW_STANDARD     )  strcpy(text, "Taille normale");
		if ( num == RT_WINDOW_CLOSE        )  strcpy(text, "Fermer");

		if ( num == RT_KEY_OR              )  strcpy(text, " ou ");

		if ( num == RT_PLAY_LISTm          )  strcpy(text, " Choix du casse-t�te � r�soudre :");
		if ( num == RT_PLAY_LISTd          )  strcpy(text, " D�fis :");
		if ( num == RT_PLAY_LISTu          )  strcpy(text, " Atelier de %s :");
		if ( num == RT_PLAY_LISTp          )  strcpy(text, " Liste des prototypes :");

		if ( num == RT_SETUP_DEVICE        )  strcpy(text, " Pilotes :");
		if ( num == RT_SETUP_MODE          )  strcpy(text, " R�solutions :");
		if ( num == RT_SETUP_KEY1          )  strcpy(text, "1) Cliquez d'abord sur la touche � red�finir.");
		if ( num == RT_SETUP_KEY2          )  strcpy(text, "2) Appuyez ensuite sur la nouvelle touche souhait�e.");

		if ( num == RT_PERSO_LIST          )  strcpy(text, "Liste des joueurs :");

		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Voulez-vous quitter BlupiMania ?");
		if ( num == RT_DIALOG_YES          )  strcpy(text, "Abandonner\\Abandonner le casse-t�te en cours");
		if ( num == RT_DIALOG_NO           )  strcpy(text, "Continuer\\Continuer le casse-t�te en cours");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Quitter\\Quitter BlupiMania");
		if ( num == RT_DIALOG_NOQUIT       )  strcpy(text, "Continuer\\Continuer de jouer");
		if ( num == RT_DIALOG_DELGAME      )  strcpy(text, "Voulez-vous supprimer la progression et l'atelier de %s ?");
		if ( num == RT_DIALOG_DELFILE      )  strcpy(text, "Voulez-vous supprimer le fichier %s ?");
		if ( num == RT_DIALOG_YESDEL       )  strcpy(text, "Supprimer");
		if ( num == RT_DIALOG_NODEL        )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_NEWGAME      )  strcpy(text, "Nom du joueur � cr�er :");
		if ( num == RT_DIALOG_YESNEW       )  strcpy(text, "Cr�er");
		if ( num == RT_DIALOG_NONEW        )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_PLAY         )  strcpy(text, "R�soudre le casse-t�te...");
		if ( num == RT_DIALOG_REPLAY       )  strcpy(text, "Rejouer pour le plaisir...");
		if ( num == RT_DIALOG_QUITEDIT     )  strcpy(text, "Enregistrer les modifications ?");
		if ( num == RT_DIALOG_YESQUITEDIT  )  strcpy(text, "Oui\\Enregistrer et quitter");
		if ( num == RT_DIALOG_NOQUITEDIT   )  strcpy(text, "Non\\Quitter sans enregistrer");
		if ( num == RT_DIALOG_CANQUITEDIT  )  strcpy(text, "Annuler\\Continuer l'�dition");
		if ( num == RT_DIALOG_NEWPUZZLE    )  strcpy(text, "Voulez-vous cr�er un nouveau casse-t�te de type \"%s\" ?");
		if ( num == RT_DIALOG_YESNEWP      )  strcpy(text, "Cr�er");
		if ( num == RT_DIALOG_NONEWP       )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_DELPUZ       )  strcpy(text, "Voulez-vous supprimer le casse-t�te ?");
		if ( num == RT_DIALOG_DELPUZd      )  strcpy(text, "(pour tous les joueurs)");
		if ( num == RT_DIALOG_DELPUZu      )  strcpy(text, "(seulement dans votre atelier)");
		if ( num == RT_DIALOG_YESDELPUZ    )  strcpy(text, "Supprimer");
		if ( num == RT_DIALOG_NODELPUZ     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_RESUMEINFO   )  strcpy(text, "Instructions");
		if ( num == RT_DIALOG_AUTHORINFO   )  strcpy(text, "Auteur");
		if ( num == RT_DIALOG_SIGNINFO     )  strcpy(text, "Indices");
		if ( num == RT_DIALOG_YESINFO      )  strcpy(text, "OK");
		if ( num == RT_DIALOG_YESERROR     )  strcpy(text, "OK");
		if ( num == RT_DIALOG_EXPORTGAME   )  strcpy(text, "O� voulez-vous exporter le casse-t�te ?");
		if ( num == RT_DIALOG_YESEXPORT    )  strcpy(text, "Exporter");
		if ( num == RT_DIALOG_NOEXPORT     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_IMPORTGAME   )  strcpy(text, "Casse-t�te contenus dans \"Mes documents/BlupiMania2\" :");
		if ( num == RT_DIALOG_YESIMPORT    )  strcpy(text, "Importer");
		if ( num == RT_DIALOG_NOIMPORT     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_TEST         )  strcpy(text, "Tester");
		if ( num == RT_DIALOG_DEFI         )  strcpy(text, "Relever le d�fi...");
		if ( num == RT_DIALOG_FILE         )  strcpy(text, "Fichier");
		if ( num == RT_DIALOG_RENAMEGAME   )  strcpy(text, "Changement du nom du fichier du casse-t�te.");
		if ( num == RT_DIALOG_YESRENAME    )  strcpy(text, "Renommer");
		if ( num == RT_DIALOG_NORENAME     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_RENAMEOLD    )  strcpy(text, "Nom actuel");
		if ( num == RT_DIALOG_RENAMENEW    )  strcpy(text, "Nouveau nom");

		if ( num == RT_GENERIC_DEV1        )  strcpy(text, "www.epsitec.ch");
		if ( num == RT_GENERIC_DEV2        )  strcpy(text, "D�velopp� par EPSITEC");
		if ( num == RT_GENERIC_DEMO1       )  strcpy(text, "Vous avez jou� � la version DEMO de BlupiMania II.");
		if ( num == RT_GENERIC_DEMO2       )  strcpy(text, "La version compl�te est disponible d�s aujourd'hui ...");
		if ( num == RT_GENERIC_DEMO3       )  strcpy(text, "... elle comprend 80 casse-t�te et 40 d�fis ...");
		if ( num == RT_GENERIC_DEMO4       )  strcpy(text, "... ainsi qu'un \"atelier\" pour cr�er ses propres casse-t�te.");
		if ( num == RT_GENERIC_DEMO5       )  strcpy(text, "Tous les d�tails sur www.epsitec.ch !");

		if ( num == RT_ACTION_GOTO         )  strcpy(text, "Aller");
		if ( num == RT_ACTION_PUSH1        )  strcpy(text, "Pousser une fois");
		if ( num == RT_ACTION_PUSHx        )  strcpy(text, "Pousser %d fois");
		if ( num == RT_ACTION_PUSHn        )  strcpy(text, "Pousser toujours");
		if ( num == RT_ACTION_PULL1        )  strcpy(text, "Tirer une fois");
		if ( num == RT_ACTION_ROLL         )  strcpy(text, "Rouler");
		if ( num == RT_ACTION_DOCKm        )  strcpy(text, "Actionner");
		if ( num == RT_ACTION_DOCKmx       )  strcpy(text, "Actionner %d fois");
		if ( num == RT_ACTION_DOCKg        )  strcpy(text, "Saisir/d�poser");
		if ( num == RT_ACTION_TRAXa        )  strcpy(text, "Avancer");
		if ( num == RT_ACTION_TRAXax       )  strcpy(text, "Avancer %d fois");
		if ( num == RT_ACTION_TRAXl        )  strcpy(text, "Tourner � droite");
		if ( num == RT_ACTION_TRAXlx       )  strcpy(text, "Tourner � droite %d fois");
		if ( num == RT_ACTION_TRAXr        )  strcpy(text, "Tourner � gauche");
		if ( num == RT_ACTION_TRAXrx       )  strcpy(text, "Tourner � gauche %d fois");
		if ( num == RT_ACTION_GUNa         )  strcpy(text, "Avancer puis tirer");
		if ( num == RT_ACTION_GUNax        )  strcpy(text, "Avancer puis tirer %d fois");
		if ( num == RT_ACTION_GUNl         )  strcpy(text, "Tourner � droite puis tirer");
		if ( num == RT_ACTION_GUNlx        )  strcpy(text, "Tourner � droite puis tirer %d fois");
		if ( num == RT_ACTION_GUNr         )  strcpy(text, "Tourner � gauche puis tirer");
		if ( num == RT_ACTION_GUNrx        )  strcpy(text, "Tourner � gauche puis tirer %d fois");
		if ( num == RT_ACTION_CATAPULT     )  strcpy(text, "Lancer");

		if ( num == RT_ADVISE_LITGOTO      )  strcpy(text, "Conseil:\nCliquez directement sur la destination, m�me si elle est �loign�e. Blupi est assez malin pour s'y rendre par le meilleur chemin.");
		if ( num == RT_ADVISE_GOTOPUSH     )  strcpy(text, "Conseil:\nCliquez directement sur la caisse. Blupi est assez malin pour d'abord se d�placer et ensuite pousser la caisse.");
		if ( num == RT_ADVISE_NOSIGN       )  strcpy(text, "Aucun indice disponible pour ce casse-t�te.");

		if ( num == RT_USER_HEADER         )  strcpy(text, "Fichier\tUnivers\tTest�\tInstructions");
		if ( num == RT_DEFI_HEADER         )  strcpy(text, "Fichier\tUnivers\tAuteur\tInstructions");
		if ( num == RT_IMPORT_HEADER       )  strcpy(text, "Fichier\tUnivers\tAuteur\tInstructions");

		if ( num == RT_DEMO                )  strcpy(text, "Pas disponible dans la DEMO");

		if ( num == RT_UNIVERS0            )  strcpy(text, "Castella");
		if ( num == RT_UNIVERS1            )  strcpy(text, "Paradiso");
		if ( num == RT_UNIVERS2            )  strcpy(text, "Volcano");
		if ( num == RT_UNIVERS3            )  strcpy(text, "Mecano");
		if ( num == RT_UNIVERS4            )  strcpy(text, "Sahara");
		if ( num == RT_UNIVERS5            )  strcpy(text, "Aglagla");
		if ( num == RT_UNIVERS6            )  strcpy(text, "Samba");
		if ( num == RT_UNIVERS7            )  strcpy(text, "Medieva");
		if ( num == RT_UNIVERS8            )  strcpy(text, "Vegeta");
		if ( num == RT_UNIVERS9            )  strcpy(text, "Inferno");
		if ( num == RT_UNIVERS10           )  strcpy(text, "Futura");
		if ( num == RT_UNIVERS11           )  strcpy(text, "Alibaba");
		if ( num == RT_UNIVERS12           )  strcpy(text, "Studio");
		if ( num == RT_UNIVERS13           )  strcpy(text, "Cata");
		if ( num == RT_UNIVERS14           )  strcpy(text, "Allegro");
		if ( num == RT_UNIVERS15           )  strcpy(text, "Cemento");
		if ( num == RT_UNIVERS16           )  strcpy(text, "Labo");
		if ( num == RT_UNIVERS17           )  strcpy(text, "Mystica");
		if ( num == RT_UNIVERS18           )  strcpy(text, "Fantasia");
		if ( num == RT_UNIVERS19           )  strcpy(text, "Natura");
	}

	if ( type == RES_EVENT )
	{
		if ( num == EVENT_BUTTON_OK        )  strcpy(text, "OK");
		if ( num == EVENT_BUTTON_CANCEL    )  strcpy(text, "Annuler");
		if ( num == EVENT_BUTTON_QUIT      )  strcpy(text, "Menu (\\key quit;)");

		if ( num == EVENT_DIALOG_OK        )  strcpy(text, "OK");
		if ( num == EVENT_DIALOG_CANCEL    )  strcpy(text, "Annuler");

		if ( num == EVENT_INTERFACE_PUZZLE )  strcpy(text, "Casse-t�te\\Torture progressive");
		if ( num == EVENT_INTERFACE_DEFI   )  strcpy(text, "D�fis\\Casse-t�te d'autres joueurs");
		if ( num == EVENT_INTERFACE_USER   )  strcpy(text, "Atelier\\Cr�ez vos casse-t�te");
		if ( num == EVENT_INTERFACE_PROTO  )  strcpy(text, "Proto\\Prototypes en cours d'�laboration");
		if ( num == EVENT_INTERFACE_NAME   )  strcpy(text, "Autre joueur\\Choix du nom du joueur");
		if ( num == EVENT_INTERFACE_SETUP  )  strcpy(text, "Options\\R�glages");
		if ( num == EVENT_INTERFACE_AGAIN  )  strcpy(text, "Recommencer\\Recommencer le casse-t�te au d�but");
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Retourner dans BlupiMania");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Quitter\\Quitter BlupiMania");
		if ( num == EVENT_INTERFACE_BACK   )  strcpy(text, "Annuler\\Retour au niveau pr�c�dent");
		if ( num == EVENT_INTERFACE_TERM   )  strcpy(text, "Menu\\Retour au menu");
		if ( num == EVENT_INTERFACE_PLAY   )  strcpy(text, "Jouer\\D�marrer l'action");
		if ( num == EVENT_INTERFACE_NEW    )  strcpy(text, "Nouveau\\Cr�er un nouveau casse-t�te");
		if ( num == EVENT_INTERFACE_DELETE )  strcpy(text, "Supprimer\\Supprimer le casse-t�te");
		if ( num == EVENT_INTERFACE_RENAME )  strcpy(text, "Renommer\\Renommer le casse-t�te");
		if ( num == EVENT_INTERFACE_EXPORT )  strcpy(text, "Exporter\\Exporter le casse-t�te");
		if ( num == EVENT_INTERFACE_IMPORT )  strcpy(text, "Importer\\Importer un casse-t�te");
		if ( num == EVENT_INTERFACE_EDIT   )  strcpy(text, "Modifier\\Modifier le casse-t�te");
		if ( num == EVENT_INTERFACE_OK     )  strcpy(text, "OK\\Retour au niveau pr�c�dent");

		if ( num == EVENT_INTERFACE_EXPORTdefi )  strcpy(text, "Dans les d�fis (pour tous les joueurs)");
		if ( num == EVENT_INTERFACE_EXPORTdoc  )  strcpy(text, "Dans le dossier \"Mes documents/BlupiMania2\"");

		if ( num == EVENT_INTERFACE_SETUPd )  strcpy(text, "Affichage\\Pilote et r�solution d'affichage");
		if ( num == EVENT_INTERFACE_SETUPg )  strcpy(text, "Graphique\\Options graphiques");
		if ( num == EVENT_INTERFACE_SETUPp )  strcpy(text, "Jeu\\Options de jouabilit�");
		if ( num == EVENT_INTERFACE_SETUPc )  strcpy(text, "Commandes\\Touches du clavier");
		if ( num == EVENT_INTERFACE_SETUPs )  strcpy(text, "Son\\Volumes bruitages & musiques");
		if ( num == EVENT_INTERFACE_RESOL  )  strcpy(text, "R�solution");
		if ( num == EVENT_INTERFACE_FULL   )  strcpy(text, "Plein �cran\\Plein �cran ou fen�tr�");
		if ( num == EVENT_INTERFACE_APPLY  )  strcpy(text, "Appliquer les changements\\Active les changements effectu�s");

		if ( num == EVENT_INTERFACE_SHADOW      )  strcpy(text, "Ombres\\Ombres projet�es au sol");
		if ( num == EVENT_INTERFACE_DIRTY       )  strcpy(text, "Salissures\\Salissures des objets");
		if ( num == EVENT_INTERFACE_SUNBEAM     )  strcpy(text, "Rayons du soleil\\Rayons verticaux sur l'eau");
		if ( num == EVENT_INTERFACE_LENSFLARE   )  strcpy(text, "Reflets dans l'objectif\\Reflets dans les lentilles de la cam�ra");
		if ( num == EVENT_INTERFACE_DECOR       )  strcpy(text, "Objets d�coratifs\\Arbres, plantes, animaux, tuyaux, etc.");
		if ( num == EVENT_INTERFACE_DETAIL      )  strcpy(text, "Objets d�taill�s\\Formes compliqu�es des objets");
		if ( num == EVENT_INTERFACE_METEO       )  strcpy(text, "Conditions m�t�o\\Pluie et neige");
		if ( num == EVENT_INTERFACE_AMBIANCE    )  strcpy(text, "Effets � la surface de l'eau\\Gouttes, ronds, jets de lave, etc.");
		if ( num == EVENT_INTERFACE_EXPLOVIB    )  strcpy(text, "Secousses lors d'explosions\\L'�cran vibre lors d'une explosion");
		if ( num == EVENT_INTERFACE_SPEEDSCH    )  strcpy(text, "Vitesse de d�filement horizontal\\Vitesse pour le clavier et la souris");
		if ( num == EVENT_INTERFACE_SPEEDSCV    )  strcpy(text, "Vitesse de d�filement vertical\\Vitesse pour le clavier et la souris");
		if ( num == EVENT_INTERFACE_MOUSESCROLL )  strcpy(text, "D�filement avec la souris\\Lorsque la souris touche les bords");
		if ( num == EVENT_INTERFACE_INVSCH      )  strcpy(text, "Inversion d�filement souris horizontal\\Lorsque la souris touche les bords gauche ou droite");
		if ( num == EVENT_INTERFACE_INVSCV      )  strcpy(text, "Inversion d�filement souris vertical\\Lorsque la souris touche les bords haut ou bas");
		if ( num == EVENT_INTERFACE_MOVIE       )  strcpy(text, "S�quences cin�matiques\\S�quences non interractives");
		if ( num == EVENT_INTERFACE_HELP        )  strcpy(text, "Instructions et indices\\Textes explicatifs");
		if ( num == EVENT_INTERFACE_TOOLTIPS    )  strcpy(text, "Bulles d'aide\\Bulles explicatives");
		if ( num == EVENT_INTERFACE_NICEMOUSE   )  strcpy(text, "Souris ombr�e\\Jolie souris avec une ombre");
		if ( num == EVENT_INTERFACE_ACCEL       )  strcpy(text, "Acc�l�re si long � r�soudre\\Blupi va de plus en plus vite apr�s 10 minutes de r�flexion");
		if ( num == EVENT_INTERFACE_VOLBLUPI    )  strcpy(text, "Volume des voix de Blupi\\");
		if ( num == EVENT_INTERFACE_VOLSOUND    )  strcpy(text, "Volume des bruitages\\");
		if ( num == EVENT_INTERFACE_VOLAMBIANCE )  strcpy(text, "Volume d'ambiance\\");
		if ( num == EVENT_INTERFACE_SOUND3D     )  strcpy(text, "Bruitages 3D\\Positionnement sonore dans l'espace");

		if ( num == EVENT_INTERFACE_KDEF   )  strcpy(text, "Tout r�initialiser\\Remet toutes les touches standards");
		if ( num == EVENT_INTERFACE_KLEFT  )  strcpy(text, "D�filement � gauche\\D�filement de l'�cran � gauche");
		if ( num == EVENT_INTERFACE_KRIGHT )  strcpy(text, "D�filement � droite\\D�filement de l'�cran � droite");
		if ( num == EVENT_INTERFACE_KUP    )  strcpy(text, "D�filement en haut\\D�filement de l'�cran en haut");
		if ( num == EVENT_INTERFACE_KDOWN  )  strcpy(text, "D�filement en bas\\D�filement de l'�cran en bas");
		if ( num == EVENT_INTERFACE_KROTCW )  strcpy(text, "Rotation horaire\\Rotation horaire de la cam�ra");
		if ( num == EVENT_INTERFACE_KROTCCW)  strcpy(text, "Rotation anti-horaire\\Rotation anti-horaire de la cam�ra");
		if ( num == EVENT_INTERFACE_KSTOP  )  strcpy(text, "Stoppe l'action en cours\\Stoppe l'action en cours");
		if ( num == EVENT_INTERFACE_KQUIT  )  strcpy(text, "Quitter le casse-t�te en cours\\Terminer un casse-t�te");
		if ( num == EVENT_INTERFACE_KHELP  )  strcpy(text, "Instructions sur le casse-t�te\\Marche � suivre");

		if ( num == EVENT_INTERFACE_MIN    )  strcpy(text, "Mini\\Qualit� minimale (+ rapide)");
		if ( num == EVENT_INTERFACE_NORM   )  strcpy(text, "Normal\\Qualit� standard");
		if ( num == EVENT_INTERFACE_MAX    )  strcpy(text, "Maxi\\Haute qualit� (+ lent)");

		if ( num == EVENT_INTERFACE_SILENT )  strcpy(text, "Silencieux\\Totalement silencieux");
		if ( num == EVENT_INTERFACE_NOISY  )  strcpy(text, "Normal\\Niveaux normaux");

		if ( num == EVENT_INTERFACE_NEDIT  )  strcpy(text, "\\Nom du joueur � cr�er");
		if ( num == EVENT_INTERFACE_NOK    )  strcpy(text, "S�lectionner\\Choisir le joueur de la liste");
		if ( num == EVENT_INTERFACE_NCREATE)  strcpy(text, "Nouveau\\Cr�er un nouveau joueur");
		if ( num == EVENT_INTERFACE_NDELETE)  strcpy(text, "Supprimer\\Supprimer le joueur de la liste");

		if ( num == EVENT_INTERFACE_PREV   )  strcpy(text, "\\Ecran pr�c�dent");
		if ( num == EVENT_INTERFACE_NEXT   )  strcpy(text, "\\Ecran suivant");
		if ( num == EVENT_INTERFACE_EXIT   )  strcpy(text, "\\Quitter BlupiMania");

		if ( num == EVENT_INTERFACE_UNDO   )  strcpy(text, "Annuler la derni�re action");
		if ( num == EVENT_INTERFACE_STOP   )  strcpy(text, "Stopper l'action en cours (\\key stop;)");
		if ( num == EVENT_INTERFACE_SIGN   )  strcpy(text, "Obtenir des indices");
		if ( num == EVENT_LABEL_SIGN       )  strcpy(text, "Indices");

		if ( num == EVENT_EDIT_WATER       )  strcpy(text, "Eau");
		if ( num == EVENT_EDIT_GROUND      )  strcpy(text, "Sol");
		if ( num == EVENT_EDIT_OBJECT      )  strcpy(text, "Objets");
		if ( num == EVENT_EDIT_CLEAR       )  strcpy(text, "Supprimer");
		if ( num == EVENT_EDIT_INFO        )  strcpy(text, "Informations sur le casse-t�te");

		if ( num == EVENT_CMD              )  strcpy(text, "Console de commande");
		if ( num == EVENT_SPEED            )  strcpy(text, "Vitesse du jeu");
	}

	if ( type == RES_OBJECT )
	{
		if ( num == OBJECT_BLUPI           )  strcpy(text, "Blupi");
		if ( num == OBJECT_BOX1            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX2            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX3            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX4            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX5            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX6            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX7            )  strcpy(text, "Grosse boule");
		if ( num == OBJECT_BOX8            )  strcpy(text, "Colonne lourde");
		if ( num == OBJECT_BOX9            )  strcpy(text, "Cage");
		if ( num == OBJECT_BOX10           )  strcpy(text, "Bombe");
		if ( num == OBJECT_BOX11           )  strcpy(text, "Chariot");
		if ( num == OBJECT_BOX12           )  strcpy(text, "Chariot");
		if ( num == OBJECT_BOX13           )  strcpy(text, "Caisse sur coussin d'air");
		if ( num == OBJECT_KEY1            )  strcpy(text, "Cl� orange");
		if ( num == OBJECT_KEY2            )  strcpy(text, "Cl� bleue");
		if ( num == OBJECT_DOOR1           )  strcpy(text, "Porte orange");
		if ( num == OBJECT_DOOR2           )  strcpy(text, "Porte bleue");
		if ( num == OBJECT_DOOR3           )  strcpy(text, "Porte orange");
		if ( num == OBJECT_DOOR4           )  strcpy(text, "Porte bleue");
		if ( num == OBJECT_DOCK            )  strcpy(text, "Pont-grue");
		if ( num == OBJECT_CATAPULT        )  strcpy(text, "Catapulte");
		if ( num == OBJECT_TRAX            )  strcpy(text, "Pousseur m�canique");
		if ( num == OBJECT_PERFO           )  strcpy(text, "Perforateur m�canique");
		if ( num == OBJECT_GUN             )  strcpy(text, "Destructeur de caisses");
		if ( num == OBJECT_MINE            )  strcpy(text, "Mine");
		if ( num == OBJECT_GLASS1          )  strcpy(text, "Vitre blind�e");
		if ( num == OBJECT_GLASS2          )  strcpy(text, "Vitre blind�e");
		if ( num == OBJECT_FIOLE           )  strcpy(text, "Potion de force");
		if ( num == OBJECT_GLU             )  strcpy(text, "Potion de glu");
		if ( num == OBJECT_GOAL            )  strcpy(text, "But � atteindre");
		if ( num == OBJECT_CRAZY           )  strcpy(text, "Robot fou");
		if ( num == OBJECT_BOT1            )  strcpy(text, "Bot 1");
		if ( num == OBJECT_BOT2            )  strcpy(text, "Bot 2");
		if ( num == OBJECT_BOT3            )  strcpy(text, "Bot 3");
		if ( num == OBJECT_BOT4            )  strcpy(text, "Bot 4");
		if ( num == OBJECT_BOT5            )  strcpy(text, "Bot 5");
	}

	if ( type == RES_ERR )
	{
		strcpy(text, "Erreur");
		if ( num == ERR_CMD             )  strcpy(text, "Commande inconnue");
		if ( num == ERR_INSTALL         )  strcpy(text, "BlupiMania n'est pas install�.");
		if ( num == ERR_NOCD            )  strcpy(text, "Veuillez mettre le CD de BlupiMania\net relancer le jeu.");
		if ( num == ERR_MOVE_IMPOSSIBLE )  strcpy(text, "D�placement impossible");
		if ( num == ERR_GOTO_IMPOSSIBLE )  strcpy(text, "Chemin introuvable");
		if ( num == ERR_GOTO_ITER       )  strcpy(text, "Position inaccessible");
		if ( num == ERR_GOTO_BUSY       )  strcpy(text, "Destination occup�e");
		if ( num == ERR_FIRE_VEH        )  strcpy(text, "Robot inadapt�");
		if ( num == ERR_FIRE_ENERGY     )  strcpy(text, "Pas assez d'�nergie");
		if ( num == ERR_MISSION_NOTERM  )  strcpy(text, "Le casse-t�te n'est pas termin� (appuyez sur \\key help; pour plus de d�tails)");
		if ( num == ERR_PUZZLE_ZEROBLUPI)  strcpy(text, "Il doit y avoir au moins un Blupi.");
		if ( num == ERR_PUZZLE_ZEROGOAL )  strcpy(text, "Il doit y avoir au moins un ballon.");
		if ( num == ERR_PUZZLE_LOGOAL   )  strcpy(text, "Il n'y a pas assez de ballons.");
		if ( num == ERR_PUZZLE_HIGOAL   )  strcpy(text, "Il y a trop de ballons.");
		if ( num == ERR_PUZZLE_MAXBLUPI )  strcpy(text, "4 Blupi ou robots au maximum par casse-t�te.");
		if ( num == ERR_PUZZLE_MAXGOAL  )  strcpy(text, "8 ballons au maximum par casse-t�te.");
		if ( num == ERR_PUZZLE_MAX      )  strcpy(text, "Il n'est pas possible de cr�er plus d'objets de ce type.");
		if ( num == ERR_CREATE          )  strcpy(text, "Ce fichier existe d�j�.");

		if ( num == INFO_WIN            )  strcpy(text, "<< Bravo, casse-t�te termin� >>");
		if ( num == INFO_LOST           )  strcpy(text, "<< D�sol�, casse-t�te �chou� >>");
		if ( num == INFO_LOSTq          )  strcpy(text, "<< D�sol�, casse-t�te �chou� >>");
	}

	if ( type == RES_CBOT )
	{
		strcpy(text, "Erreur");
		if ( num == TX_OPENPAR       ) strcpy(text, "Il manque une parenth�se ouvrante");
		if ( num == TX_CLOSEPAR      ) strcpy(text, "Il manque une parenth�se fermante");
		if ( num == TX_NOTBOOL       ) strcpy(text, "L'expression doit �tre un boolean");
		if ( num == TX_UNDEFVAR      ) strcpy(text, "Variable non d�clar�e");
		if ( num == TX_BADLEFT       ) strcpy(text, "Assignation impossible");
		if ( num == TX_ENDOF         ) strcpy(text, "Terminateur point-virgule non trouv�");
		if ( num == TX_OUTCASE       ) strcpy(text, "Instruction ""case"" hors d'un bloc ""switch""");
		if ( num == TX_NOTERM        ) strcpy(text, "Instructions apr�s la fin");
		if ( num == TX_CLOSEBLK      ) strcpy(text, "Il manque la fin du bloc");
		if ( num == TX_ELSEWITHOUTIF ) strcpy(text, "Instruction ""else"" sans ""if"" correspondant");
		if ( num == TX_OPENBLK       ) strcpy(text, "D�but d'un bloc attendu");
		if ( num == TX_BADTYPE       ) strcpy(text, "Mauvais type de r�sultat pour l'assignation");
		if ( num == TX_REDEFVAR      ) strcpy(text, "Red�finition d'une variable");
		if ( num == TX_BAD2TYPE      ) strcpy(text, "Les deux op�randes ne sont pas de types compatibles");
		if ( num == TX_UNDEFCALL     ) strcpy(text, "Routine inconnue");
		if ( num == TX_MISDOTS       ) strcpy(text, "S�parateur "" : "" attendu");
		if ( num == TX_WHILE         ) strcpy(text, "Manque le mot ""while""");
		if ( num == TX_BREAK         ) strcpy(text, "Instruction ""break"" en dehors d'une boucle");
		if ( num == TX_LABEL         ) strcpy(text, "Un label ne peut se placer que devant un ""for"", un ""while"", un ""do"" ou un ""switch""");
		if ( num == TX_NOLABEL       ) strcpy(text, "Cette �tiquette n'existe pas");
		if ( num == TX_NOCASE        ) strcpy(text, "Manque une instruction ""case""");
		if ( num == TX_BADNUM        ) strcpy(text, "Un nombre est attendu");
		if ( num == TX_VOID          ) strcpy(text, "Param�tre void");
		if ( num == TX_NOTYP         ) strcpy(text, "D�claration de type attendu");
		if ( num == TX_NOVAR         ) strcpy(text, "Nom d'une variable attendu");
		if ( num == TX_NOFONC        ) strcpy(text, "Nom de la fonction attendu");
		if ( num == TX_OVERPARAM     ) strcpy(text, "Trop de param�tres");
		if ( num == TX_REDEF         ) strcpy(text, "Cette fonction existe d�j�");
		if ( num == TX_LOWPARAM      ) strcpy(text, "Pas assez de param�tres");
		if ( num == TX_BADPARAM      ) strcpy(text, "Aucune fonction de ce nom n'accepte ce(s) type(s) de param�tre(s)");
		if ( num == TX_NUMPARAM      ) strcpy(text, "Aucune fonction de ce nom n'accepte ce nombre de param�tres");
		if ( num == TX_NOITEM        ) strcpy(text, "Cet �l�ment n'existe pas dans cette classe");
		if ( num == TX_DOT           ) strcpy(text, "L'objet n'est pas une instance d'une classe");
		if ( num == TX_NOCONST       ) strcpy(text, "Il n'y a pas de constructeur appropri�");
		if ( num == TX_REDEFCLASS    ) strcpy(text, "Cette classe existe d�j�");
		if ( num == TX_CLBRK         ) strcpy(text, """ ] "" attendu");
		if ( num == TX_RESERVED      ) strcpy(text, "Ce mot est r�serv�");
		if ( num == TX_BADNEW        ) strcpy(text, "Mauvais argument pour ""new""");
		if ( num == TX_OPBRK         ) strcpy(text, """ [ "" attendu");
		if ( num == TX_BADSTRING     ) strcpy(text, "Une cha�ne de caract�re est attendue");
		if ( num == TX_BADINDEX      ) strcpy(text, "Mauvais type d'index");
		if ( num == TX_PRIVATE       ) strcpy(text, "El�ment prot�g�");
		if ( num == TX_NOPUBLIC      ) strcpy(text, "Public requis");
		if ( num == TX_DIVZERO       ) strcpy(text, "Division par z�ro");
		if ( num == TX_NOTINIT       ) strcpy(text, "Variable non initialis�e");
		if ( num == TX_BADTHROW      ) strcpy(text, "Valeur n�gative refus�e pour ""throw""");
		if ( num == TX_NORETVAL      ) strcpy(text, "La fonction n'a pas retourn� de r�sultat");
		if ( num == TX_NORUN         ) strcpy(text, "Pas de fonction en ex�cution");
		if ( num == TX_NOCALL        ) strcpy(text, "Appel d'une fonction inexistante");
		if ( num == TX_NOCLASS       ) strcpy(text, "Cette classe n'existe pas");
		if ( num == TX_NULLPT        ) strcpy(text, "Objet n'existe pas");
		if ( num == TX_OPNAN         ) strcpy(text, "Op�ration sur un ""nan""");
		if ( num == TX_OUTARRAY      ) strcpy(text, "Acc�s hors du tableau");
		if ( num == TX_STACKOVER     ) strcpy(text, "D�bordement de la pile");
		if ( num == TX_DELETEDPT     ) strcpy(text, "Objet inaccessible");
		if ( num == TX_FILEOPEN      ) strcpy(text, "Ouverture du fichier impossible");
		if ( num == TX_NOTOPEN       ) strcpy(text, "Le fichier n'est pas ouvert");
		if ( num == TX_ERRREAD       ) strcpy(text, "Erreur � la lecture");
		if ( num == TX_ERRWRITE      ) strcpy(text, "Erreur � l'�criture");
	}

	if ( type == RES_KEY )
	{
		if ( num == 0                   )  strcpy(text, "< aucune >");
		if ( num == VK_LEFT             )  strcpy(text, "Fl�che Gauche");
		if ( num == VK_RIGHT            )  strcpy(text, "Fl�che Droite");
		if ( num == VK_UP               )  strcpy(text, "Fl�che Haut");
		if ( num == VK_DOWN             )  strcpy(text, "Fl�che Bas");
		if ( num == VK_CANCEL           )  strcpy(text, "Control-break");
		if ( num == VK_BACK             )  strcpy(text, "<--");
		if ( num == VK_TAB              )  strcpy(text, "Tab");
		if ( num == VK_CLEAR            )  strcpy(text, "Clear");
		if ( num == VK_RETURN           )  strcpy(text, "Entr�e");
		if ( num == VK_SHIFT            )  strcpy(text, "Shift");
		if ( num == VK_CONTROL          )  strcpy(text, "Ctrl");
		if ( num == VK_MENU             )  strcpy(text, "Alt");
		if ( num == VK_PAUSE            )  strcpy(text, "Pause");
		if ( num == VK_CAPITAL          )  strcpy(text, "Caps Lock");
		if ( num == VK_ESCAPE           )  strcpy(text, "Esc");
		if ( num == VK_SPACE            )  strcpy(text, "Espace");
		if ( num == VK_PRIOR            )  strcpy(text, "Page Up");
		if ( num == VK_NEXT             )  strcpy(text, "Page Down");
		if ( num == VK_END              )  strcpy(text, "End");
		if ( num == VK_HOME             )  strcpy(text, "Home");
		if ( num == VK_SELECT           )  strcpy(text, "Select");
		if ( num == VK_EXECUTE          )  strcpy(text, "Execute");
		if ( num == VK_SNAPSHOT         )  strcpy(text, "Print Scrn");
		if ( num == VK_INSERT           )  strcpy(text, "Insert");
		if ( num == VK_DELETE           )  strcpy(text, "Delete");
		if ( num == VK_HELP             )  strcpy(text, "Help");
		if ( num == VK_LWIN             )  strcpy(text, "Left Windows");
		if ( num == VK_RWIN             )  strcpy(text, "Right Windows");
		if ( num == VK_APPS             )  strcpy(text, "Application key");
		if ( num == VK_NUMPAD0          )  strcpy(text, "NumPad 0");
		if ( num == VK_NUMPAD1          )  strcpy(text, "NumPad 1");
		if ( num == VK_NUMPAD2          )  strcpy(text, "NumPad 2");
		if ( num == VK_NUMPAD3          )  strcpy(text, "NumPad 3");
		if ( num == VK_NUMPAD4          )  strcpy(text, "NumPad 4");
		if ( num == VK_NUMPAD5          )  strcpy(text, "NumPad 5");
		if ( num == VK_NUMPAD6          )  strcpy(text, "NumPad 6");
		if ( num == VK_NUMPAD7          )  strcpy(text, "NumPad 7");
		if ( num == VK_NUMPAD8          )  strcpy(text, "NumPad 8");
		if ( num == VK_NUMPAD9          )  strcpy(text, "NumPad 9");
		if ( num == VK_MULTIPLY         )  strcpy(text, "NumPad *");
		if ( num == VK_ADD              )  strcpy(text, "NumPad +");
		if ( num == VK_SEPARATOR        )  strcpy(text, "NumPad sep");
		if ( num == VK_SUBTRACT         )  strcpy(text, "NumPad -");
		if ( num == VK_DECIMAL          )  strcpy(text, "NumPad .");
		if ( num == VK_DIVIDE           )  strcpy(text, "NumPad /");
		if ( num == VK_F1               )  strcpy(text, "F1");
		if ( num == VK_F2               )  strcpy(text, "F2");
		if ( num == VK_F3               )  strcpy(text, "F3");
		if ( num == VK_F4               )  strcpy(text, "F4");
		if ( num == VK_F5               )  strcpy(text, "F5");
		if ( num == VK_F6               )  strcpy(text, "F6");
		if ( num == VK_F7               )  strcpy(text, "F7");
		if ( num == VK_F8               )  strcpy(text, "F8");
		if ( num == VK_F9               )  strcpy(text, "F9");
		if ( num == VK_F10              )  strcpy(text, "F10");
		if ( num == VK_F11              )  strcpy(text, "F11");
		if ( num == VK_F12              )  strcpy(text, "F12");
		if ( num == VK_F13              )  strcpy(text, "F13");
		if ( num == VK_F14              )  strcpy(text, "F14");
		if ( num == VK_F15              )  strcpy(text, "F15");
		if ( num == VK_F16              )  strcpy(text, "F16");
		if ( num == VK_F17              )  strcpy(text, "F17");
		if ( num == VK_F18              )  strcpy(text, "F18");
		if ( num == VK_F19              )  strcpy(text, "F19");
		if ( num == VK_F20              )  strcpy(text, "F20");
		if ( num == VK_NUMLOCK          )  strcpy(text, "Num Lock");
		if ( num == VK_SCROLL           )  strcpy(text, "Scroll");
		if ( num == VK_ATTN             )  strcpy(text, "Attn");
		if ( num == VK_CRSEL            )  strcpy(text, "CrSel");
		if ( num == VK_EXSEL            )  strcpy(text, "ExSel");
		if ( num == VK_EREOF            )  strcpy(text, "Erase EOF");
		if ( num == VK_PLAY             )  strcpy(text, "Play");
		if ( num == VK_ZOOM             )  strcpy(text, "Zoom");
		if ( num == VK_PA1              )  strcpy(text, "PA1");
		if ( num == VK_OEM_CLEAR        )  strcpy(text, "Clear");
		if ( num == VK_BUTTON1          )  strcpy(text, "Bouton 1");
		if ( num == VK_BUTTON2          )  strcpy(text, "Bouton 2");
		if ( num == VK_BUTTON3          )  strcpy(text, "Bouton 3");
		if ( num == VK_BUTTON4          )  strcpy(text, "Bouton 4");
		if ( num == VK_BUTTON5          )  strcpy(text, "Bouton 5");
		if ( num == VK_BUTTON6          )  strcpy(text, "Bouton 6");
		if ( num == VK_BUTTON7          )  strcpy(text, "Bouton 7");
		if ( num == VK_BUTTON8          )  strcpy(text, "Bouton 8");
		if ( num == VK_BUTTON9          )  strcpy(text, "Bouton 9");
		if ( num == VK_BUTTON10         )  strcpy(text, "Bouton 10");
		if ( num == VK_BUTTON11         )  strcpy(text, "Bouton 11");
		if ( num == VK_BUTTON12         )  strcpy(text, "Bouton 12");
		if ( num == VK_BUTTON13         )  strcpy(text, "Bouton 13");
		if ( num == VK_BUTTON14         )  strcpy(text, "Bouton 14");
		if ( num == VK_BUTTON15         )  strcpy(text, "Bouton 15");
		if ( num == VK_BUTTON16         )  strcpy(text, "Bouton 16");
		if ( num == VK_BUTTON17         )  strcpy(text, "Bouton 17");
		if ( num == VK_BUTTON18         )  strcpy(text, "Bouton 18");
		if ( num == VK_BUTTON19         )  strcpy(text, "Bouton 19");
		if ( num == VK_BUTTON20         )  strcpy(text, "Bouton 20");
		if ( num == VK_BUTTON21         )  strcpy(text, "Bouton 21");
		if ( num == VK_BUTTON22         )  strcpy(text, "Bouton 22");
		if ( num == VK_BUTTON23         )  strcpy(text, "Bouton 23");
		if ( num == VK_BUTTON24         )  strcpy(text, "Bouton 24");
		if ( num == VK_BUTTON25         )  strcpy(text, "Bouton 25");
		if ( num == VK_BUTTON26         )  strcpy(text, "Bouton 26");
		if ( num == VK_BUTTON27         )  strcpy(text, "Bouton 27");
		if ( num == VK_BUTTON28         )  strcpy(text, "Bouton 28");
		if ( num == VK_BUTTON29         )  strcpy(text, "Bouton 29");
		if ( num == VK_BUTTON30         )  strcpy(text, "Bouton 30");
		if ( num == VK_BUTTON31         )  strcpy(text, "Bouton 31");
		if ( num == VK_BUTTON32         )  strcpy(text, "Bouton 32");
		if ( num == VK_WHEELUP          )  strcpy(text, "Molette haut");
		if ( num == VK_WHEELDOWN        )  strcpy(text, "Molette bas");
	}
#endif

#if _ITALIAN
	if ( type == RES_TEXT )
	{
		#if _FULL
		#if _EDU
		if ( num == RT_VERSION_ID          )  strcpy(text, "EDU 1.3 /i");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "1.3 /i");
		#endif
		#endif
		#if _DEMO
		if ( num == RT_VERSION_ID          )  strcpy(text, "Demo 1.3 /i");
		#endif
		if ( num == RT_WINDOW_MAXIMIZED    )  strcpy(text, "Ingrandisci");
		if ( num == RT_WINDOW_MINIMIZED    )  strcpy(text, "Riduci a icona");
		if ( num == RT_WINDOW_STANDARD     )  strcpy(text, "Dimensioni normali");
		if ( num == RT_WINDOW_CLOSE        )  strcpy(text, "Chiudi");

		if ( num == RT_KEY_OR              )  strcpy(text, " o ");

		if ( num == RT_PLAY_LISTm          )  strcpy(text, " Seleziona una missione:");
		if ( num == RT_PLAY_LISTd          )  strcpy(text, " D�fis :");
		if ( num == RT_PLAY_LISTu          )  strcpy(text, " Atelier de %s :");
		if ( num == RT_PLAY_LISTp          )  strcpy(text, " Prototipi:");

		if ( num == RT_SETUP_DEVICE        )  strcpy(text, " Driver:");
		if ( num == RT_SETUP_MODE          )  strcpy(text, " Risoluzione:");
		if ( num == RT_SETUP_KEY1          )  strcpy(text, "1) Fai prima clic sul tasto da reimpostare.");
		if ( num == RT_SETUP_KEY2          )  strcpy(text, "2) Quindi premi il tasto che desideri utilizzare in alternativa.");

		if ( num == RT_PERSO_LIST          )  strcpy(text, "Elenco piloti:");

		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Uscire dal gioco?");
		if ( num == RT_DIALOG_YES          )  strcpy(text, "Interrompi\\Interrompi missione corrente");
		if ( num == RT_DIALOG_NO           )  strcpy(text, "Continua\\Continua missione corrente");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Esci\\Esci da BlupiMania");
		if ( num == RT_DIALOG_NOQUIT       )  strcpy(text, "Continua\\Continua gioco");
		if ( num == RT_DIALOG_DELGAME      )  strcpy(text, "Eliminare i giochi e le auto di %s salvati?");
		if ( num == RT_DIALOG_DELFILE      )  strcpy(text, "Eliminare il file %s esistente?");
		if ( num == RT_DIALOG_YESDEL       )  strcpy(text, "Elimina");
		if ( num == RT_DIALOG_NODEL        )  strcpy(text, "Annulla");
		if ( num == RT_DIALOG_NEWGAME      )  strcpy(text, "Nome pilota:");
		if ( num == RT_DIALOG_YESNEW       )  strcpy(text, "Crea");
		if ( num == RT_DIALOG_NONEW        )  strcpy(text, "Annulla");
		if ( num == RT_DIALOG_PLAY         )  strcpy(text, "R�soudre le puzzle...");
		if ( num == RT_DIALOG_REPLAY       )  strcpy(text, "Rejouer pour le plaisir...");
		if ( num == RT_DIALOG_QUITEDIT     )  strcpy(text, "Enregistrer les modifications ?");
		if ( num == RT_DIALOG_YESQUITEDIT  )  strcpy(text, "Oui\\Enregistrer et quitter");
		if ( num == RT_DIALOG_NOQUITEDIT   )  strcpy(text, "Non\\Quitter sans enregistrer");
		if ( num == RT_DIALOG_CANQUITEDIT  )  strcpy(text, "Annuler\\Continuer l'�dition");
		if ( num == RT_DIALOG_NEWPUZZLE    )  strcpy(text, "Voulez-vous cr�er un nouveau puzzle \"%s\" ?");
		if ( num == RT_DIALOG_YESNEWP      )  strcpy(text, "Cr�er");
		if ( num == RT_DIALOG_NONEWP       )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_DELPUZ       )  strcpy(text, "Voulez-vous supprimer le puzzle ?");
		if ( num == RT_DIALOG_DELPUZd      )  strcpy(text, "(pour tous les joueurs)");
		if ( num == RT_DIALOG_DELPUZu      )  strcpy(text, "(seulement dans votre atelier)");
		if ( num == RT_DIALOG_YESDELPUZ    )  strcpy(text, "Supprimer");
		if ( num == RT_DIALOG_NODELPUZ     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_RESUMEINFO   )  strcpy(text, "Instructions");
		if ( num == RT_DIALOG_AUTHORINFO   )  strcpy(text, "Auteur");
		if ( num == RT_DIALOG_SIGNINFO     )  strcpy(text, "Indices");
		if ( num == RT_DIALOG_YESINFO      )  strcpy(text, "OK");
		if ( num == RT_DIALOG_YESERROR     )  strcpy(text, "OK");
		if ( num == RT_DIALOG_EXPORTGAME   )  strcpy(text, "O� voulez-vous exporter le casse-t�te ?");
		if ( num == RT_DIALOG_YESEXPORT    )  strcpy(text, "Exporter");
		if ( num == RT_DIALOG_NOEXPORT     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_IMPORTGAME   )  strcpy(text, "Casse-t�te contenus dans \"Mes documents\" :");
		if ( num == RT_DIALOG_YESIMPORT    )  strcpy(text, "Importer");
		if ( num == RT_DIALOG_NOIMPORT     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_TEST         )  strcpy(text, "Tester");
		if ( num == RT_DIALOG_DEFI         )  strcpy(text, "Relever le d�fi...");
		if ( num == RT_DIALOG_FILE         )  strcpy(text, "Fichier");
		if ( num == RT_DIALOG_RENAMEGAME   )  strcpy(text, "Changement du nom du fichier du casse-t�te.");
		if ( num == RT_DIALOG_YESRENAME    )  strcpy(text, "Renommer");
		if ( num == RT_DIALOG_NORENAME     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_RENAMEOLD    )  strcpy(text, "Nom actuel");
		if ( num == RT_DIALOG_RENAMENEW    )  strcpy(text, "Nouveau nom");

		if ( num == RT_GENERIC_DEV1        )  strcpy(text, "www.epsitec.com");
		if ( num == RT_GENERIC_DEV2        )  strcpy(text, "Sviluppato da EPSITEC");
		if ( num == RT_GENERIC_DEMO1       )  strcpy(text, "Hai giocato con la versione DEMO di BlupiMania.");
		if ( num == RT_GENERIC_DEMO2       )  strcpy(text, "� ora disponibile la versione completa...");
		if ( num == RT_GENERIC_DEMO3       )  strcpy(text, "... contiene 28 avvincenti missioni e 9 gare ...");
		if ( num == RT_GENERIC_DEMO4       )  strcpy(text, "... nonch� una modalit� \"sfida\" per mettere a confronto le prestazioni.");
		if ( num == RT_GENERIC_DEMO5       )  strcpy(text, "Per ulteriori dettagli visitare il sito Web www.epsitec.com !");

		if ( num == RT_ACTION_GOTO         )  strcpy(text, "Aller");
		if ( num == RT_ACTION_PUSH1        )  strcpy(text, "Pousser une fois");
		if ( num == RT_ACTION_PUSHx        )  strcpy(text, "Pousser %d fois");
		if ( num == RT_ACTION_PUSHn        )  strcpy(text, "Pousser toujours");
		if ( num == RT_ACTION_PULL1        )  strcpy(text, "Tirer une fois");
		if ( num == RT_ACTION_ROLL         )  strcpy(text, "Rouler");
		if ( num == RT_ACTION_DOCKm        )  strcpy(text, "Actionner");
		if ( num == RT_ACTION_DOCKmx       )  strcpy(text, "Actionner %d fois");
		if ( num == RT_ACTION_DOCKg        )  strcpy(text, "Saisir/d�poser");
		if ( num == RT_ACTION_TRAXa        )  strcpy(text, "Avancer");
		if ( num == RT_ACTION_TRAXax       )  strcpy(text, "Avancer %d fois");
		if ( num == RT_ACTION_TRAXl        )  strcpy(text, "Tourner � droite");
		if ( num == RT_ACTION_TRAXlx       )  strcpy(text, "Tourner � droite %d fois");
		if ( num == RT_ACTION_TRAXr        )  strcpy(text, "Tourner � gauche");
		if ( num == RT_ACTION_TRAXrx       )  strcpy(text, "Tourner � gauche %d fois");
		if ( num == RT_ACTION_GUNa         )  strcpy(text, "Avancer puis tirer");
		if ( num == RT_ACTION_GUNax        )  strcpy(text, "Avancer puis tirer %d fois");
		if ( num == RT_ACTION_GUNl         )  strcpy(text, "Tourner � droite puis tirer");
		if ( num == RT_ACTION_GUNlx        )  strcpy(text, "Tourner � droite puis tirer %d fois");
		if ( num == RT_ACTION_GUNr         )  strcpy(text, "Tourner � gauche puis tirer");
		if ( num == RT_ACTION_GUNrx        )  strcpy(text, "Tourner � gauche puis tirer %d fois");
		if ( num == RT_ACTION_CATAPULT     )  strcpy(text, "Lancer");

		if ( num == RT_ADVISE_LITGOTO      )  strcpy(text, "...");
		if ( num == RT_ADVISE_GOTOPUSH     )  strcpy(text, "...");
		if ( num == RT_ADVISE_NOSIGN       )  strcpy(text, "Aucun indice disponible pour ce casse-t�te.");

		if ( num == RT_USER_HEADER         )  strcpy(text, "Fichier\tUnivers\tTest�\tInstructions");
		if ( num == RT_DEFI_HEADER         )  strcpy(text, "Fichier\tUnivers\tAuteur\tInstructions");
		if ( num == RT_IMPORT_HEADER       )  strcpy(text, "Fichier\tUnivers\tAuteur\tInstructions");

		if ( num == RT_DEMO                )  strcpy(text, "Pas disponible dans la DEMO");

		if ( num == RT_UNIVERS0            )  strcpy(text, "Castella");
		if ( num == RT_UNIVERS1            )  strcpy(text, "Paradiso");
		if ( num == RT_UNIVERS2            )  strcpy(text, "Volcano");
		if ( num == RT_UNIVERS3            )  strcpy(text, "Mecano");
		if ( num == RT_UNIVERS4            )  strcpy(text, "Sahara");
		if ( num == RT_UNIVERS5            )  strcpy(text, "Aglagla");
		if ( num == RT_UNIVERS6            )  strcpy(text, "Samba");
		if ( num == RT_UNIVERS7            )  strcpy(text, "Medieva");
		if ( num == RT_UNIVERS8            )  strcpy(text, "Vegeta");
		if ( num == RT_UNIVERS9            )  strcpy(text, "Inferno");
		if ( num == RT_UNIVERS10           )  strcpy(text, "Futura");
		if ( num == RT_UNIVERS11           )  strcpy(text, "Alibaba");
		if ( num == RT_UNIVERS12           )  strcpy(text, "Studio");
		if ( num == RT_UNIVERS13           )  strcpy(text, "Cata");
		if ( num == RT_UNIVERS14           )  strcpy(text, "Allegro");
		if ( num == RT_UNIVERS15           )  strcpy(text, "Cemento");
		if ( num == RT_UNIVERS16           )  strcpy(text, "Labo");
		if ( num == RT_UNIVERS17           )  strcpy(text, "Drinka");
		if ( num == RT_UNIVERS18           )  strcpy(text, "Woodo");
		if ( num == RT_UNIVERS19           )  strcpy(text, "Natura");
	}

	if ( type == RES_EVENT )
	{
		if ( num == EVENT_BUTTON_OK        )  strcpy(text, "OK");
		if ( num == EVENT_BUTTON_CANCEL    )  strcpy(text, "Annulla");
		if ( num == EVENT_BUTTON_QUIT      )  strcpy(text, "Menu (\\key quit;)");

		if ( num == EVENT_DIALOG_OK        )  strcpy(text, "OK");
		if ( num == EVENT_DIALOG_CANCEL    )  strcpy(text, "Annulla");

		if ( num == EVENT_INTERFACE_PUZZLE )  strcpy(text, "Casse-t�te\\Torture progressive");
		if ( num == EVENT_INTERFACE_DEFI   )  strcpy(text, "D�fis\\Casse-t�te d'autres joueurs");
		if ( num == EVENT_INTERFACE_USER   )  strcpy(text, "Atelier\\Cr�ez vos casse-t�te");
		if ( num == EVENT_INTERFACE_PROTO  )  strcpy(text, "Prototipi\\Prototipi in sviluppo");
		if ( num == EVENT_INTERFACE_NAME   )  strcpy(text, "Nuovo pilota\\Scegli nome pilota");
		if ( num == EVENT_INTERFACE_SETUP  )  strcpy(text, "Opzioni\\Preferenze");
		if ( num == EVENT_INTERFACE_AGAIN  )  strcpy(text, "Riavvia\\Riavvia la missione dall'inizio");
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Torna a BlupiMania");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Esci\\Esci da BlupiMania");
		if ( num == EVENT_INTERFACE_BACK   )  strcpy(text, "Annulla\\Torna alla schermata precedente");
		if ( num == EVENT_INTERFACE_TERM   )  strcpy(text, "Menu\\Torna al menu principale");
		if ( num == EVENT_INTERFACE_PLAY   )  strcpy(text, "Gioca\\Inizia la missione selezionata");
		if ( num == EVENT_INTERFACE_NEW    )  strcpy(text, "Nouveau\\Cr�er un nouveau puzzle");
		if ( num == EVENT_INTERFACE_DELETE )  strcpy(text, "Supprimer\\Supprimer le puzzle");
		if ( num == EVENT_INTERFACE_RENAME )  strcpy(text, "Renommer\\Renommer le casse-t�te");
		if ( num == EVENT_INTERFACE_EXPORT )  strcpy(text, "Exporter\\Exporter le casse-t�te");
		if ( num == EVENT_INTERFACE_IMPORT )  strcpy(text, "Importer\\Importer un casse-t�te");
		if ( num == EVENT_INTERFACE_EDIT   )  strcpy(text, "Modifier\\Modifier le puzzle");
		if ( num == EVENT_INTERFACE_OK     )  strcpy(text, "OK\\Torna alla schermata precedente");

		if ( num == EVENT_INTERFACE_EXPORTdefi )  strcpy(text, "Dans les d�fis (pour tous les joueurs)");
		if ( num == EVENT_INTERFACE_EXPORTdoc  )  strcpy(text, "Dans le dossier \"Mes documents\"");

		if ( num == EVENT_INTERFACE_SETUPd )  strcpy(text, "Dispositivo\\Impostazioni driver e risoluzione");
		if ( num == EVENT_INTERFACE_SETUPg )  strcpy(text, "Grafica\\Impostazioni grafiche");
		if ( num == EVENT_INTERFACE_SETUPp )  strcpy(text, "Gioco\\Impostazioni di gioco");
		if ( num == EVENT_INTERFACE_SETUPc )  strcpy(text, "Controlli\\Impostazioni tastiera, volante e joypad");
		if ( num == EVENT_INTERFACE_SETUPs )  strcpy(text, "Audio\\Volume di musica ed effetti sonori");
		if ( num == EVENT_INTERFACE_RESOL  )  strcpy(text, "Risoluzione");
		if ( num == EVENT_INTERFACE_FULL   )  strcpy(text, "Schermo intero\\Schermo intero o modalit� finestra");
		if ( num == EVENT_INTERFACE_APPLY  )  strcpy(text, "Applica modifiche\\Attiva le impostazioni modificate");

		if ( num == EVENT_INTERFACE_SHADOW      )  strcpy(text, "Ombres\\Ombres projet�es au sol");
		if ( num == EVENT_INTERFACE_DIRTY       )  strcpy(text, "Salissures\\Salissures des object");
		if ( num == EVENT_INTERFACE_SUNBEAM     )  strcpy(text, "Rayons du soleil\\Rayons selon l'orientation");
		if ( num == EVENT_INTERFACE_LENSFLARE   )  strcpy(text, "Reflets dans l'objectif\\Reflets selon l'orientation");
		if ( num == EVENT_INTERFACE_DECOR       )  strcpy(text, "Objets d�coratifs\\Arbres, plantes, etc.");
		if ( num == EVENT_INTERFACE_DETAIL      )  strcpy(text, "Objets d�taill�s\\Objets d�taill�s");
		if ( num == EVENT_INTERFACE_METEO       )  strcpy(text, "Conditions m�t�o\\Pluie et neige");
		if ( num == EVENT_INTERFACE_AMBIANCE    )  strcpy(text, "Effets d'ambiance\\");
		if ( num == EVENT_INTERFACE_EXPLOVIB    )  strcpy(text, "Secousses lors d'explosions\\L'�cran vibre lors d'une explosion");
		if ( num == EVENT_INTERFACE_SPEEDSCH    )  strcpy(text, "Vitesse de d�filement horizontal\\");
		if ( num == EVENT_INTERFACE_SPEEDSCV    )  strcpy(text, "Vitesse de d�filement vertical\\");
		if ( num == EVENT_INTERFACE_MOUSESCROLL )  strcpy(text, "D�filement avec la souris\\");
		if ( num == EVENT_INTERFACE_INVSCH      )  strcpy(text, "Inversion d�filement horizontal\\");
		if ( num == EVENT_INTERFACE_INVSCV      )  strcpy(text, "Inversion d�filement vertical\\");
		if ( num == EVENT_INTERFACE_MOVIE       )  strcpy(text, "S�quences cin�matiques\\S�quences d'introduction");
		if ( num == EVENT_INTERFACE_HELP        )  strcpy(text, "Instructions et indices\\Textes explicatifs");
		if ( num == EVENT_INTERFACE_TOOLTIPS    )  strcpy(text, "Bulles d'aide\\Bulles explicatives");
		if ( num == EVENT_INTERFACE_NICEMOUSE   )  strcpy(text, "Souris ombr�e\\Jolie souris avec une ombre");
		if ( num == EVENT_INTERFACE_ACCEL       )  strcpy(text, "Acc�l�re si long � r�soudre\\Blupi va plus vite apr�s 10 minutes de r�flexion");
		if ( num == EVENT_INTERFACE_VOLBLUPI    )  strcpy(text, "Volume des voix de Blupi\\");
		if ( num == EVENT_INTERFACE_VOLSOUND    )  strcpy(text, "Volume des bruitages\\");
		if ( num == EVENT_INTERFACE_VOLAMBIANCE )  strcpy(text, "Volume d'ambiance\\");
		if ( num == EVENT_INTERFACE_SOUND3D     )  strcpy(text, "Suono 3D\\Suono 3D");

		if ( num == EVENT_INTERFACE_KDEF   )  strcpy(text, "Controlli predefiniti\\Ripristina le impostazioni predefinite dei controlli");
		if ( num == EVENT_INTERFACE_KLEFT  )  strcpy(text, "Scroll left\\Scrolling to left");
		if ( num == EVENT_INTERFACE_KRIGHT )  strcpy(text, "Scroll right\\Scrolling to right");
		if ( num == EVENT_INTERFACE_KUP    )  strcpy(text, "Scroll up\\Scrolling to up");
		if ( num == EVENT_INTERFACE_KDOWN  )  strcpy(text, "Scroll down\\Scrolling to down");
		if ( num == EVENT_INTERFACE_KROTCW )  strcpy(text, "Rotate CW\\Rotate camera clockwise");
		if ( num == EVENT_INTERFACE_KROTCCW)  strcpy(text, "Rotate CCW\\Rotate camera counter clockwise");
		if ( num == EVENT_INTERFACE_KSTOP  )  strcpy(text, "Stoppe action\\Stoppe l'action en cours");
		if ( num == EVENT_INTERFACE_KQUIT  )  strcpy(text, "Esci\\Esci dalla missione o dalla gara corrente");
		if ( num == EVENT_INTERFACE_KHELP  )  strcpy(text, "Instruzioni\\Visualizza le istruzioni della missione corrente");

		if ( num == EVENT_INTERFACE_MIN    )  strcpy(text, "Minima\\Qualit� grafica minima (frequenza di quadro massima)");
		if ( num == EVENT_INTERFACE_NORM   )  strcpy(text, "Normale\\Qualit� grafica media");
		if ( num == EVENT_INTERFACE_MAX    )  strcpy(text, "Massima\\Qualit� grafica massima (frequenza di quadro minima)");

		if ( num == EVENT_INTERFACE_SILENT )  strcpy(text, "Silenzio\\Disattivazione dell'audio");
		if ( num == EVENT_INTERFACE_NOISY  )  strcpy(text, "Normale\\Volume dell'audio normale");

		if ( num == EVENT_INTERFACE_NEDIT  )  strcpy(text, "\\Nome nuovo pilota");
		if ( num == EVENT_INTERFACE_NOK    )  strcpy(text, "Seleziona\\Scegli il pilota selezionato");
		if ( num == EVENT_INTERFACE_NCREATE)  strcpy(text, "Nuovo\\Crea un nuovo pilota");
		if ( num == EVENT_INTERFACE_NDELETE)  strcpy(text, "Elimina\\Elimina il pilota dall'elenco");

		if ( num == EVENT_INTERFACE_PREV   )  strcpy(text, "\\Previous screen");
		if ( num == EVENT_INTERFACE_NEXT   )  strcpy(text, "\\Next screen");
		if ( num == EVENT_INTERFACE_EXIT   )  strcpy(text, "\\Quit BlupiMania");

		if ( num == EVENT_INTERFACE_UNDO   )  strcpy(text, "Annule la derni�re action");
		if ( num == EVENT_INTERFACE_STOP   )  strcpy(text, "Stop (\\key stop;)");
		if ( num == EVENT_INTERFACE_SIGN   )  strcpy(text, "Obtenir des indices");
		if ( num == EVENT_LABEL_SIGN       )  strcpy(text, "Indices");

		if ( num == EVENT_EDIT_WATER       )  strcpy(text, "Eau");
		if ( num == EVENT_EDIT_GROUND      )  strcpy(text, "Sol");
		if ( num == EVENT_EDIT_OBJECT      )  strcpy(text, "Objets");
		if ( num == EVENT_EDIT_INFO        )  strcpy(text, "Informations sur le puzzle");

		if ( num == EVENT_CMD              )  strcpy(text, "Riga di comando");
		if ( num == EVENT_SPEED            )  strcpy(text, "Velocit� gioco");
	}

	if ( type == RES_OBJECT )
	{
		if ( num == OBJECT_BLUPI           )  strcpy(text, "Blupi");
		if ( num == OBJECT_BOX1            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX2            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX3            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX4            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX5            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX6            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX7            )  strcpy(text, "Grosse boule");
		if ( num == OBJECT_BOX8            )  strcpy(text, "Colonne lourde");
		if ( num == OBJECT_BOX9            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX10           )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX11           )  strcpy(text, "Caisse sur roues");
		if ( num == OBJECT_BOX12           )  strcpy(text, "Caisse sur roues");
		if ( num == OBJECT_BOX13           )  strcpy(text, "Caisse sur coussin d'air");
		if ( num == OBJECT_KEY1            )  strcpy(text, "Cl� orange");
		if ( num == OBJECT_KEY2            )  strcpy(text, "Cl� bleue");
		if ( num == OBJECT_DOOR1           )  strcpy(text, "Porte orange");
		if ( num == OBJECT_DOOR2           )  strcpy(text, "Porte bleue");
		if ( num == OBJECT_DOOR3           )  strcpy(text, "Porte orange");
		if ( num == OBJECT_DOOR4           )  strcpy(text, "Porte bleue");
		if ( num == OBJECT_DOCK            )  strcpy(text, "Pont-grue");
		if ( num == OBJECT_CATAPULT        )  strcpy(text, "Catapulte");
		if ( num == OBJECT_TRAX            )  strcpy(text, "Pousseur m�canique");
		if ( num == OBJECT_PERFO           )  strcpy(text, "Perforateur m�canique");
		if ( num == OBJECT_GUN             )  strcpy(text, "Destructeur de caisses");
		if ( num == OBJECT_MINE            )  strcpy(text, "Mine");
		if ( num == OBJECT_GLASS1          )  strcpy(text, "Vitre blind�e");
		if ( num == OBJECT_GLASS2          )  strcpy(text, "Vitre blind�e");
		if ( num == OBJECT_FIOLE           )  strcpy(text, "Potion de force");
		if ( num == OBJECT_GLU             )  strcpy(text, "Potion de glu");
		if ( num == OBJECT_GOAL            )  strcpy(text, "But � atteindre");
		if ( num == OBJECT_CRAZY           )  strcpy(text, "Crazy bot");
		if ( num == OBJECT_BOT1            )  strcpy(text, "Bot 1");
		if ( num == OBJECT_BOT2            )  strcpy(text, "Bot 2");
		if ( num == OBJECT_BOT3            )  strcpy(text, "Bot 3");
		if ( num == OBJECT_BOT4            )  strcpy(text, "Bot 4");
		if ( num == OBJECT_BOT5            )  strcpy(text, "Bot 5");
	}

	if ( type == RES_ERR )
	{
		strcpy(text, "Errore");
		if ( num == ERR_CMD             )  strcpy(text, "Comando sconosciuto");
		if ( num == ERR_INSTALL         )  strcpy(text, "Wild Wheels non � stato installato correttamente.");
		if ( num == ERR_NOCD            )  strcpy(text, "Inserire il CD Wild Wheels\ne rieseguire il programma del gioco.");
		if ( num == ERR_MOVE_IMPOSSIBLE )  strcpy(text, "Errore nel trasferimento di istruzioni");
		if ( num == ERR_GOTO_IMPOSSIBLE )  strcpy(text, "Vai a: destinazione inaccessibile");
		if ( num == ERR_GOTO_ITER       )  strcpy(text, "Vai a: destinazione inaccessibile");
		if ( num == ERR_GOTO_BUSY       )  strcpy(text, "Vai a: destinazione occupata");
		if ( num == ERR_FIRE_VEH        )  strcpy(text, "Bot errato");
		if ( num == ERR_FIRE_ENERGY     )  strcpy(text, "Energia insufficiente");
		if ( num == ERR_MISSION_NOTERM  )  strcpy(text, "La missione non � stata ancora completata (premi \\key help; per ulteriori dettagli)");
		if ( num == ERR_PUZZLE_ZEROBLUPI)  strcpy(text, "Il doit y avoir au moins un Blupi !");
		if ( num == ERR_PUZZLE_ZEROGOAL )  strcpy(text, "Il doit y avoir au moins un ballon !");
		if ( num == ERR_PUZZLE_LOGOAL   )  strcpy(text, "Il n'y a pas assez de ballons !");
		if ( num == ERR_PUZZLE_HIGOAL   )  strcpy(text, "Il y a trop de ballons !");
		if ( num == ERR_PUZZLE_MAXBLUPI )  strcpy(text, "4 Blupi ou robots au maximum par puzzle !");
		if ( num == ERR_PUZZLE_MAXGOAL  )  strcpy(text, "8 ballons au maximum par puzzle !");
		if ( num == ERR_PUZZLE_MAX      )  strcpy(text, "Il n'est pas possible de cr�er plus d'objets de ce type !");
		if ( num == ERR_CREATE          )  strcpy(text, "Ce fichier existe d�j�.");

		if ( num == INFO_WIN            )  strcpy(text, "<< Congratulazioni, missione compiuta >>");
		if ( num == INFO_LOST           )  strcpy(text, "<< Spiacente, missione non riuscita >>");
		if ( num == INFO_LOSTq          )  strcpy(text, "<< Spiacente, missione non riuscita >>");
	}

	if ( type == RES_CBOT )
	{
		strcpy(text, "Errore");
		if ( num == TX_OPENPAR       ) strcpy(text, "Parentesi di apertura mancante");
		if ( num == TX_CLOSEPAR      ) strcpy(text, "Parentesi di chiusura mancante ");
		if ( num == TX_NOTBOOL       ) strcpy(text, "L'espressione deve restituire un valore boleano");
		if ( num == TX_UNDEFVAR      ) strcpy(text, "Variabile non dichiarata");
		if ( num == TX_BADLEFT       ) strcpy(text, "Assegnazione impossibile");
		if ( num == TX_ENDOF         ) strcpy(text, "Terminatore punto e virgola mancante");
		if ( num == TX_OUTCASE       ) strcpy(text, "Istruzione ""case"" fuori del blocco ""switch""");
		if ( num == TX_NOTERM        ) strcpy(text, "Istruzioni dopo la parentesi graffa di chiusura finale");
		if ( num == TX_CLOSEBLK      ) strcpy(text, "Fine del blocco mancante");
		if ( num == TX_ELSEWITHOUTIF ) strcpy(text, "Istruzione ""else"" senza corrispondente ""if"" ");
		if ( num == TX_OPENBLK       ) strcpy(text, "Parentesi graffa di apertura mancante ");//d�but d'un bloc attendu?
		if ( num == TX_BADTYPE       ) strcpy(text, "Tipo di assegnazione errato");
		if ( num == TX_REDEFVAR      ) strcpy(text, "Non � possibile dichiarare due volte una variabile");
		if ( num == TX_BAD2TYPE      ) strcpy(text, "Le tipologie dei due operandi non sono compatibili");
		if ( num == TX_UNDEFCALL     ) strcpy(text, "Funzione sconosciuta");
		if ( num == TX_MISDOTS       ) strcpy(text, "Segno "" : "" mancante");
		if ( num == TX_WHILE         ) strcpy(text, "Parola chiave ""while"" mancante");
		if ( num == TX_BREAK         ) strcpy(text, "Istruzione ""break"" fuori del cliclo");
		if ( num == TX_LABEL         ) strcpy(text, "Un'etichetta deve esser seguita da ""for"", ""while"", ""do"" o ""switch""");
		if ( num == TX_NOLABEL       ) strcpy(text, "Etichetta inesistente");// Cette �tiquette n'existe pas
		if ( num == TX_NOCASE        ) strcpy(text, "Istruzione ""case"" mancante");
		if ( num == TX_BADNUM        ) strcpy(text, "Numero mancante");
		if ( num == TX_VOID          ) strcpy(text, "Parametro nullo");
		if ( num == TX_NOTYP         ) strcpy(text, "Dichiarazione Type mancante");
		if ( num == TX_NOVAR         ) strcpy(text, "Nome variabile mancante");
		if ( num == TX_NOFONC        ) strcpy(text, "Nome funzione mancante");
		if ( num == TX_OVERPARAM     ) strcpy(text, "Parametri in eccesso");
		if ( num == TX_REDEF         ) strcpy(text, "La funzione esiste gi�");
		if ( num == TX_LOWPARAM      ) strcpy(text, "Parametri mancanti ");
		if ( num == TX_BADPARAM      ) strcpy(text, "Nessuna funzione con questo nome accetta questo tipo di parametro");
		if ( num == TX_NUMPARAM      ) strcpy(text, "Nessuna funzione con questo nome accetta questo numero di parametri");
		if ( num == TX_NOITEM        ) strcpy(text, "Non appartiene a questa classe");
		if ( num == TX_DOT           ) strcpy(text, "Questo oggetto non appartiene ad alcuna classe");
		if ( num == TX_NOCONST       ) strcpy(text, "Costruttore appropriato mancante");
		if ( num == TX_REDEFCLASS    ) strcpy(text, "Questa classe esiste gi�");
		if ( num == TX_CLBRK         ) strcpy(text, """ ] "" mancante");
		if ( num == TX_RESERVED      ) strcpy(text, "Parola chiave riservata del linguaggio CBOT");
		if ( num == TX_BADNEW        ) strcpy(text, "Argomento errato per ""new""");
		if ( num == TX_OPBRK         ) strcpy(text, """ [ "" previsto");
		if ( num == TX_BADSTRING     ) strcpy(text, "Stringa mancante");
		if ( num == TX_BADINDEX      ) strcpy(text, "Tipo di indice errato");
		if ( num == TX_PRIVATE       ) strcpy(text, "Elemento privato");
		if ( num == TX_NOPUBLIC      ) strcpy(text, "Richiesto pubblico");
		if ( num == TX_DIVZERO       ) strcpy(text, "Divisione per zero");
		if ( num == TX_NOTINIT       ) strcpy(text, "Variabile non inizializzata");
		if ( num == TX_BADTHROW      ) strcpy(text, "Valore negativo respinto da ""throw""");//C'est quoi, �a?
		if ( num == TX_NORETVAL      ) strcpy(text, "La funzione non ha restituito alcun valore ");
		if ( num == TX_NORUN         ) strcpy(text, "Nessuna funzione in esecuzione");
		if ( num == TX_NOCALL        ) strcpy(text, "Richiamo di una funzione sconosciuta");
		if ( num == TX_NOCLASS       ) strcpy(text, "Classe inesistente");
		if ( num == TX_NULLPT        ) strcpy(text, "Oggetto sconosciuto");
		if ( num == TX_OPNAN         ) strcpy(text, "Operazione impossibile con il valore ""nan""");
		if ( num == TX_OUTARRAY      ) strcpy(text, "Accesso oltre il limite della matrice");
		if ( num == TX_STACKOVER     ) strcpy(text, "Overflow dello stack");
		if ( num == TX_DELETEDPT     ) strcpy(text, "Oggetto non valido");
		if ( num == TX_FILEOPEN      ) strcpy(text, "Impossibile aprire il file");
		if ( num == TX_NOTOPEN       ) strcpy(text, "Il file non � aperto");
		if ( num == TX_ERRREAD       ) strcpy(text, "Errore di lettura");
		if ( num == TX_ERRWRITE      ) strcpy(text, "Errore di scrittura");
	}

	if ( type == RES_KEY )
	{
		if ( num == 0                   )  strcpy(text, "< nessuno >");
		if ( num == VK_LEFT             )  strcpy(text, "Freccia a sinistra");
		if ( num == VK_RIGHT            )  strcpy(text, "Freccia a destra");
		if ( num == VK_UP               )  strcpy(text, "Freccia su");
		if ( num == VK_DOWN             )  strcpy(text, "Freccia gi�");
		if ( num == VK_CANCEL           )  strcpy(text, "CTRL+INTER");
		if ( num == VK_BACK             )  strcpy(text, "<--");
		if ( num == VK_TAB              )  strcpy(text, "Tabulazione");
		if ( num == VK_CLEAR            )  strcpy(text, "Cancella");
		if ( num == VK_RETURN           )  strcpy(text, "Invio");
		if ( num == VK_SHIFT            )  strcpy(text, "Maiusc");
		if ( num == VK_CONTROL          )  strcpy(text, "CTRL");
		if ( num == VK_MENU             )  strcpy(text, "Alt");
		if ( num == VK_PAUSE            )  strcpy(text, "Pausa");
		if ( num == VK_CAPITAL          )  strcpy(text, "Bloc Maiusc");
		if ( num == VK_ESCAPE           )  strcpy(text, "Esc");
		if ( num == VK_SPACE            )  strcpy(text, "Barra spaziatrice");
		if ( num == VK_PRIOR            )  strcpy(text, "PGSU");
		if ( num == VK_NEXT             )  strcpy(text, "PGGI�");
		if ( num == VK_END              )  strcpy(text, "Fine");
		if ( num == VK_HOME             )  strcpy(text, "Home");
		if ( num == VK_SELECT           )  strcpy(text, "Seleziona");
		if ( num == VK_EXECUTE          )  strcpy(text, "Esegui");
		if ( num == VK_SNAPSHOT         )  strcpy(text, "Stamp");
		if ( num == VK_INSERT           )  strcpy(text, "Ins");
		if ( num == VK_DELETE           )  strcpy(text, "Canc");
		if ( num == VK_HELP             )  strcpy(text, "?");
		if ( num == VK_LWIN             )  strcpy(text, "Finestre di sinistra");
		if ( num == VK_RWIN             )  strcpy(text, "Finestre di destra");
		if ( num == VK_APPS             )  strcpy(text, "Tasto Applicazione");
		if ( num == VK_NUMPAD0          )  strcpy(text, "Tast. num 0");
		if ( num == VK_NUMPAD1          )  strcpy(text, "Tast. num 1");
		if ( num == VK_NUMPAD2          )  strcpy(text, "Tast. num 2");
		if ( num == VK_NUMPAD3          )  strcpy(text, "Tast. num 3");
		if ( num == VK_NUMPAD4          )  strcpy(text, "Tast. num 4");
		if ( num == VK_NUMPAD5          )  strcpy(text, "Tast. num 5");
		if ( num == VK_NUMPAD6          )  strcpy(text, "Tast. num 6");
		if ( num == VK_NUMPAD7          )  strcpy(text, "Tast. num 7");
		if ( num == VK_NUMPAD8          )  strcpy(text, "Tast. num 8");
		if ( num == VK_NUMPAD9          )  strcpy(text, "Tast. num 9");
		if ( num == VK_MULTIPLY         )  strcpy(text, "Tast. num *");
		if ( num == VK_ADD              )  strcpy(text, "Tast. num +");
		if ( num == VK_SEPARATOR        )  strcpy(text, "Separatore tast. num");
		if ( num == VK_SUBTRACT         )  strcpy(text, "Tast. num -");
		if ( num == VK_DECIMAL          )  strcpy(text, "Tast. num .");
		if ( num == VK_DIVIDE           )  strcpy(text, "Tast. num /");
		if ( num == VK_F1               )  strcpy(text, "F1");
		if ( num == VK_F2               )  strcpy(text, "F2");
		if ( num == VK_F3               )  strcpy(text, "F3");
		if ( num == VK_F4               )  strcpy(text, "F4");
		if ( num == VK_F5               )  strcpy(text, "F5");
		if ( num == VK_F6               )  strcpy(text, "F6");
		if ( num == VK_F7               )  strcpy(text, "F7");
		if ( num == VK_F8               )  strcpy(text, "F8");
		if ( num == VK_F9               )  strcpy(text, "F9");
		if ( num == VK_F10              )  strcpy(text, "F10");
		if ( num == VK_F11              )  strcpy(text, "F11");
		if ( num == VK_F12              )  strcpy(text, "F12");
		if ( num == VK_F13              )  strcpy(text, "F13");
		if ( num == VK_F14              )  strcpy(text, "F14");
		if ( num == VK_F15              )  strcpy(text, "F15");
		if ( num == VK_F16              )  strcpy(text, "F16");
		if ( num == VK_F17              )  strcpy(text, "F17");
		if ( num == VK_F18              )  strcpy(text, "F18");
		if ( num == VK_F19              )  strcpy(text, "F19");
		if ( num == VK_F20              )  strcpy(text, "F20");
		if ( num == VK_NUMLOCK          )  strcpy(text, "Bloc Num");
		if ( num == VK_SCROLL           )  strcpy(text, "Bloc Scorr");
		if ( num == VK_ATTN             )  strcpy(text, "Attn");
		if ( num == VK_CRSEL            )  strcpy(text, "CrSel");
		if ( num == VK_EXSEL            )  strcpy(text, "ExSel");
		if ( num == VK_EREOF            )  strcpy(text, "Elimina EOF");
		if ( num == VK_PLAY             )  strcpy(text, "Gioca");
		if ( num == VK_ZOOM             )  strcpy(text, "Zoom");
		if ( num == VK_PA1              )  strcpy(text, "PA1");
		if ( num == VK_OEM_CLEAR        )  strcpy(text, "Elimina");
		if ( num == VK_BUTTON1          )  strcpy(text, "Pulsante 1");
		if ( num == VK_BUTTON2          )  strcpy(text, "Pulsante 2");
		if ( num == VK_BUTTON3          )  strcpy(text, "Pulsante 3");
		if ( num == VK_BUTTON4          )  strcpy(text, "Pulsante 4");
		if ( num == VK_BUTTON5          )  strcpy(text, "Pulsante 5");
		if ( num == VK_BUTTON6          )  strcpy(text, "Pulsante 6");
		if ( num == VK_BUTTON7          )  strcpy(text, "Pulsante 7");
		if ( num == VK_BUTTON8          )  strcpy(text, "Pulsante 8");
		if ( num == VK_BUTTON9          )  strcpy(text, "Pulsante 9");
		if ( num == VK_BUTTON10         )  strcpy(text, "Pulsante 10");
		if ( num == VK_BUTTON11         )  strcpy(text, "Pulsante 11");
		if ( num == VK_BUTTON12         )  strcpy(text, "Pulsante 12");
		if ( num == VK_BUTTON13         )  strcpy(text, "Pulsante 13");
		if ( num == VK_BUTTON14         )  strcpy(text, "Pulsante 14");
		if ( num == VK_BUTTON15         )  strcpy(text, "Pulsante 15");
		if ( num == VK_BUTTON16         )  strcpy(text, "Pulsante 16");
		if ( num == VK_BUTTON17         )  strcpy(text, "Pulsante 17");
		if ( num == VK_BUTTON18         )  strcpy(text, "Pulsante 18");
		if ( num == VK_BUTTON19         )  strcpy(text, "Pulsante 19");
		if ( num == VK_BUTTON20         )  strcpy(text, "Pulsante 20");
		if ( num == VK_BUTTON21         )  strcpy(text, "Pulsante 21");
		if ( num == VK_BUTTON22         )  strcpy(text, "Pulsante 22");
		if ( num == VK_BUTTON23         )  strcpy(text, "Pulsante 23");
		if ( num == VK_BUTTON24         )  strcpy(text, "Pulsante 24");
		if ( num == VK_BUTTON25         )  strcpy(text, "Pulsante 25");
		if ( num == VK_BUTTON26         )  strcpy(text, "Pulsante 26");
		if ( num == VK_BUTTON27         )  strcpy(text, "Pulsante 27");
		if ( num == VK_BUTTON28         )  strcpy(text, "Pulsante 28");
		if ( num == VK_BUTTON29         )  strcpy(text, "Pulsante 29");
		if ( num == VK_BUTTON30         )  strcpy(text, "Pulsante 30");
		if ( num == VK_BUTTON31         )  strcpy(text, "Pulsante 31");
		if ( num == VK_BUTTON32         )  strcpy(text, "Pulsante 32");
		if ( num == VK_WHEELUP          )  strcpy(text, "Volante su");
		if ( num == VK_WHEELDOWN        )  strcpy(text, "Volante gi�");
	}
#endif

#if _SPANISH
	if ( type == RES_TEXT )
	{
		#if _FULL
		#if _EDU
		if ( num == RT_VERSION_ID          )  strcpy(text, "EDU 1.3 /s");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "1.3 /s");
		#endif
		#endif
		#if _DEMO
		if ( num == RT_VERSION_ID          )  strcpy(text, "Demo 1.3 /s");
		#endif
		if ( num == RT_WINDOW_MAXIMIZED    )  strcpy(text, "Maximizar");
		if ( num == RT_WINDOW_MINIMIZED    )  strcpy(text, "Minimizar");
		if ( num == RT_WINDOW_STANDARD     )  strcpy(text, "Tama�o normal");
		if ( num == RT_WINDOW_CLOSE        )  strcpy(text, "Cerrar");

		if ( num == RT_KEY_OR              )  strcpy(text, " o ");

		if ( num == RT_PLAY_LISTm          )  strcpy(text, " Selecciona una misi�n:");
		if ( num == RT_PLAY_LISTd          )  strcpy(text, " D�fis :");
		if ( num == RT_PLAY_LISTu          )  strcpy(text, " Atelier de %s :");
		if ( num == RT_PLAY_LISTp          )  strcpy(text, " Prototipos:");

		if ( num == RT_SETUP_DEVICE        )  strcpy(text, " Drivers:");
		if ( num == RT_SETUP_MODE          )  strcpy(text, " Resoluci�n:");
		if ( num == RT_SETUP_KEY1          )  strcpy(text, "1) Haz clic primero en la tecla que quieres redefinir.");
		if ( num == RT_SETUP_KEY2          )  strcpy(text, "2) Pulsa la tecla que quieres usar en su lugar.");

		if ( num == RT_PERSO_LIST          )  strcpy(text, "Lista de conductores:");

		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "�Quieres salir de BlupiMania?");
		if ( num == RT_DIALOG_YES          )  strcpy(text, "Anular\\Anular la misi�n actual");
		if ( num == RT_DIALOG_NO           )  strcpy(text, "Continuar\\Continuar la misi�n actual");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Salir\\Salir de BlupiMania");
		if ( num == RT_DIALOG_NOQUIT       )  strcpy(text, "Continuar\\Continuar el juego");
		if ( num == RT_DIALOG_DELGAME      )  strcpy(text, "�Quieres eliminar juegos y coches guardados de %s?");
		if ( num == RT_DIALOG_DELFILE      )  strcpy(text, "�Eliminar el archivo %s existente?");
		if ( num == RT_DIALOG_YESDEL       )  strcpy(text, "Eliminar");
		if ( num == RT_DIALOG_NODEL        )  strcpy(text, "Cancelar");
		if ( num == RT_DIALOG_NEWGAME      )  strcpy(text, "Nombre del conductor:");
		if ( num == RT_DIALOG_YESNEW       )  strcpy(text, "Crear");
		if ( num == RT_DIALOG_NONEW        )  strcpy(text, "Cancelar");
		if ( num == RT_DIALOG_PLAY         )  strcpy(text, "R�soudre le puzzle...");
		if ( num == RT_DIALOG_REPLAY       )  strcpy(text, "Rejouer pour le plaisir...");
		if ( num == RT_DIALOG_QUITEDIT     )  strcpy(text, "Enregistrer les modifications ?");
		if ( num == RT_DIALOG_YESQUITEDIT  )  strcpy(text, "Oui\\Enregistrer et quitter");
		if ( num == RT_DIALOG_NOQUITEDIT   )  strcpy(text, "Non\\Quitter sans enregistrer");
		if ( num == RT_DIALOG_CANQUITEDIT  )  strcpy(text, "Annuler\\Continuer l'�dition");
		if ( num == RT_DIALOG_NEWPUZZLE    )  strcpy(text, "Voulez-vous cr�er un nouveau puzzle \"%s\" ?");
		if ( num == RT_DIALOG_YESNEWP      )  strcpy(text, "Cr�er");
		if ( num == RT_DIALOG_NONEWP       )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_DELPUZ       )  strcpy(text, "Voulez-vous supprimer le puzzle ?");
		if ( num == RT_DIALOG_DELPUZd      )  strcpy(text, "(pour tous les joueurs)");
		if ( num == RT_DIALOG_DELPUZu      )  strcpy(text, "(seulement dans votre atelier)");
		if ( num == RT_DIALOG_YESDELPUZ    )  strcpy(text, "Supprimer");
		if ( num == RT_DIALOG_NODELPUZ     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_RESUMEINFO   )  strcpy(text, "Instructions");
		if ( num == RT_DIALOG_AUTHORINFO   )  strcpy(text, "Auteur");
		if ( num == RT_DIALOG_SIGNINFO     )  strcpy(text, "Indices");
		if ( num == RT_DIALOG_YESINFO      )  strcpy(text, "OK");
		if ( num == RT_DIALOG_YESERROR     )  strcpy(text, "OK");
		if ( num == RT_DIALOG_EXPORTGAME   )  strcpy(text, "O� voulez-vous exporter le casse-t�te ?");
		if ( num == RT_DIALOG_YESEXPORT    )  strcpy(text, "Exporter");
		if ( num == RT_DIALOG_NOEXPORT     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_IMPORTGAME   )  strcpy(text, "Casse-t�te contenus dans \"Mes documents\" :");
		if ( num == RT_DIALOG_YESIMPORT    )  strcpy(text, "Importer");
		if ( num == RT_DIALOG_NOIMPORT     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_TEST         )  strcpy(text, "Tester");
		if ( num == RT_DIALOG_DEFI         )  strcpy(text, "Relever le d�fi...");
		if ( num == RT_DIALOG_FILE         )  strcpy(text, "Fichier");
		if ( num == RT_DIALOG_RENAMEGAME   )  strcpy(text, "Changement du nom du fichier du casse-t�te.");
		if ( num == RT_DIALOG_YESRENAME    )  strcpy(text, "Renommer");
		if ( num == RT_DIALOG_NORENAME     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_RENAMEOLD    )  strcpy(text, "Nom actuel");
		if ( num == RT_DIALOG_RENAMENEW    )  strcpy(text, "Nouveau nom");

		if ( num == RT_GENERIC_DEV1        )  strcpy(text, "www.epsitec.com");
		if ( num == RT_GENERIC_DEV2        )  strcpy(text, "Desarrollado por EPSITEC");
		if ( num == RT_GENERIC_DEMO1       )  strcpy(text, "Has jugado con la versi�n de DEMOSTRACI�N de BlupiMania.");
		if ( num == RT_GENERIC_DEMO2       )  strcpy(text, "La versi�n completa ya est� disponible...");
		if ( num == RT_GENERIC_DEMO3       )  strcpy(text, "... consta de 28 misiones desafiantes y 9 carreras...");
		if ( num == RT_GENERIC_DEMO4       )  strcpy(text, "... as� como de un modo de \"duelo\" para comparar actuaciones.");
		if ( num == RT_GENERIC_DEMO5       )  strcpy(text, "M�s informaci�n en www.epsitec.com");

		if ( num == RT_ACTION_GOTO         )  strcpy(text, "Aller");
		if ( num == RT_ACTION_PUSH1        )  strcpy(text, "Pousser une fois");
		if ( num == RT_ACTION_PUSHx        )  strcpy(text, "Pousser %d fois");
		if ( num == RT_ACTION_PUSHn        )  strcpy(text, "Pousser toujours");
		if ( num == RT_ACTION_PULL1        )  strcpy(text, "Tirer une fois");
		if ( num == RT_ACTION_ROLL         )  strcpy(text, "Rouler");
		if ( num == RT_ACTION_DOCKm        )  strcpy(text, "Actionner");
		if ( num == RT_ACTION_DOCKmx       )  strcpy(text, "Actionner %d fois");
		if ( num == RT_ACTION_DOCKg        )  strcpy(text, "Saisir/d�poser");
		if ( num == RT_ACTION_TRAXa        )  strcpy(text, "Avancer");
		if ( num == RT_ACTION_TRAXax       )  strcpy(text, "Avancer %d fois");
		if ( num == RT_ACTION_TRAXl        )  strcpy(text, "Tourner � droite");
		if ( num == RT_ACTION_TRAXlx       )  strcpy(text, "Tourner � droite %d fois");
		if ( num == RT_ACTION_TRAXr        )  strcpy(text, "Tourner � gauche");
		if ( num == RT_ACTION_TRAXrx       )  strcpy(text, "Tourner � gauche %d fois");
		if ( num == RT_ACTION_GUNa         )  strcpy(text, "Avancer puis tirer");
		if ( num == RT_ACTION_GUNax        )  strcpy(text, "Avancer puis tirer %d fois");
		if ( num == RT_ACTION_GUNl         )  strcpy(text, "Tourner � droite puis tirer");
		if ( num == RT_ACTION_GUNlx        )  strcpy(text, "Tourner � droite puis tirer %d fois");
		if ( num == RT_ACTION_GUNr         )  strcpy(text, "Tourner � gauche puis tirer");
		if ( num == RT_ACTION_GUNrx        )  strcpy(text, "Tourner � gauche puis tirer %d fois");
		if ( num == RT_ACTION_CATAPULT     )  strcpy(text, "Lancer");

		if ( num == RT_ADVISE_LITGOTO      )  strcpy(text, "...");
		if ( num == RT_ADVISE_GOTOPUSH     )  strcpy(text, "...");
		if ( num == RT_ADVISE_NOSIGN       )  strcpy(text, "Aucun indice disponible pour ce casse-t�te.");

		if ( num == RT_USER_HEADER         )  strcpy(text, "Fichier\tUnivers\tTest�\tInstructions");
		if ( num == RT_DEFI_HEADER         )  strcpy(text, "Fichier\tUnivers\tAuteur\tInstructions");
		if ( num == RT_IMPORT_HEADER       )  strcpy(text, "Fichier\tUnivers\tAuteur\tInstructions");

		if ( num == RT_DEMO                )  strcpy(text, "Pas disponible dans la DEMO");

		if ( num == RT_UNIVERS0            )  strcpy(text, "Castella");
		if ( num == RT_UNIVERS1            )  strcpy(text, "Paradiso");
		if ( num == RT_UNIVERS2            )  strcpy(text, "Volcano");
		if ( num == RT_UNIVERS3            )  strcpy(text, "Mecano");
		if ( num == RT_UNIVERS4            )  strcpy(text, "Sahara");
		if ( num == RT_UNIVERS5            )  strcpy(text, "Aglagla");
		if ( num == RT_UNIVERS6            )  strcpy(text, "Samba");
		if ( num == RT_UNIVERS7            )  strcpy(text, "Medieva");
		if ( num == RT_UNIVERS8            )  strcpy(text, "Vegeta");
		if ( num == RT_UNIVERS9            )  strcpy(text, "Inferno");
		if ( num == RT_UNIVERS10           )  strcpy(text, "Futura");
		if ( num == RT_UNIVERS11           )  strcpy(text, "Alibaba");
		if ( num == RT_UNIVERS12           )  strcpy(text, "Studio");
		if ( num == RT_UNIVERS13           )  strcpy(text, "Cata");
		if ( num == RT_UNIVERS14           )  strcpy(text, "Allegro");
		if ( num == RT_UNIVERS15           )  strcpy(text, "Cemento");
		if ( num == RT_UNIVERS16           )  strcpy(text, "Labo");
		if ( num == RT_UNIVERS17           )  strcpy(text, "Drinka");
		if ( num == RT_UNIVERS18           )  strcpy(text, "Woodo");
		if ( num == RT_UNIVERS19           )  strcpy(text, "Natura");
	}

	if ( type == RES_EVENT )
	{
		if ( num == EVENT_BUTTON_OK        )  strcpy(text, "Aceptar");
		if ( num == EVENT_BUTTON_CANCEL    )  strcpy(text, "Cancelar");
		if ( num == EVENT_BUTTON_QUIT      )  strcpy(text, "Men� (\\key quit;)");

		if ( num == EVENT_DIALOG_OK        )  strcpy(text, "Aceptar");
		if ( num == EVENT_DIALOG_CANCEL    )  strcpy(text, "Cancelar");

		if ( num == EVENT_INTERFACE_PUZZLE )  strcpy(text, "Casse-t�te\\Torture progressive");
		if ( num == EVENT_INTERFACE_DEFI   )  strcpy(text, "D�fis\\Casse-t�te d'autres joueurs");
		if ( num == EVENT_INTERFACE_USER   )  strcpy(text, "Atelier\\Cr�ez vos casse-t�te");
		if ( num == EVENT_INTERFACE_PROTO  )  strcpy(text, "Prototipos\\Prototipos en construcci�n");
		if ( num == EVENT_INTERFACE_NAME   )  strcpy(text, "Conductor nuevo\\Elegir nombre de conductor");
		if ( num == EVENT_INTERFACE_SETUP  )  strcpy(text, "Opciones\\Preferencias");
		if ( num == EVENT_INTERFACE_AGAIN  )  strcpy(text, "Reiniciar\\Reiniciar la misi�n desde el principio");
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Volver a BlupiMania");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Salir\\Salir de BlupiMania");
		if ( num == EVENT_INTERFACE_BACK   )  strcpy(text, "Cancelar\\Volver a la pantalla anterior");
		if ( num == EVENT_INTERFACE_TERM   )  strcpy(text, "Men�\\Volver al men� principal");
		if ( num == EVENT_INTERFACE_PLAY   )  strcpy(text, "Jugar\\Comenzar misi�n seleccionada");
		if ( num == EVENT_INTERFACE_NEW    )  strcpy(text, "Nouveau\\Cr�er un nouveau puzzle");
		if ( num == EVENT_INTERFACE_DELETE )  strcpy(text, "Supprimer\\Supprimer le puzzle");
		if ( num == EVENT_INTERFACE_RENAME )  strcpy(text, "Renommer\\Renommer le casse-t�te");
		if ( num == EVENT_INTERFACE_EXPORT )  strcpy(text, "Exporter\\Exporter le casse-t�te");
		if ( num == EVENT_INTERFACE_IMPORT )  strcpy(text, "Importer\\Importer un casse-t�te");
		if ( num == EVENT_INTERFACE_EDIT   )  strcpy(text, "Modifier\\Modifier le puzzle");
		if ( num == EVENT_INTERFACE_OK     )  strcpy(text, "Aceptar\\Volver a la pantalla anterior");

		if ( num == EVENT_INTERFACE_EXPORTdefi )  strcpy(text, "Dans les d�fis (pour tous les joueurs)");
		if ( num == EVENT_INTERFACE_EXPORTdoc  )  strcpy(text, "Dans le dossier \"Mes documents\"");

		if ( num == EVENT_INTERFACE_SETUPd )  strcpy(text, "Dispositivo\\Configuraci�n de controlador y resoluci�n");
		if ( num == EVENT_INTERFACE_SETUPg )  strcpy(text, "Gr�ficos\\Configuraci�n de gr�ficos");
		if ( num == EVENT_INTERFACE_SETUPp )  strcpy(text, "Juego\\Configuraci�n de juego");
		if ( num == EVENT_INTERFACE_SETUPc )  strcpy(text, "Controles\\Configuraci�n de teclado, volante y gamepad");
		if ( num == EVENT_INTERFACE_SETUPs )  strcpy(text, "Sonido\\Volumen de m�sica y efectos de sonido");
		if ( num == EVENT_INTERFACE_RESOL  )  strcpy(text, "Resoluci�n");
		if ( num == EVENT_INTERFACE_FULL   )  strcpy(text, "Pantalla completa\\Modo de pantalla completa o ventana");
		if ( num == EVENT_INTERFACE_APPLY  )  strcpy(text, "Aplicar cambios\\Activa la configuraci�n modificada");

		if ( num == EVENT_INTERFACE_SHADOW      )  strcpy(text, "Ombres\\Ombres projet�es au sol");
		if ( num == EVENT_INTERFACE_DIRTY       )  strcpy(text, "Salissures\\Salissures des object");
		if ( num == EVENT_INTERFACE_SUNBEAM     )  strcpy(text, "Rayons du soleil\\Rayons selon l'orientation");
		if ( num == EVENT_INTERFACE_LENSFLARE   )  strcpy(text, "Reflets dans l'objectif\\Reflets selon l'orientation");
		if ( num == EVENT_INTERFACE_DECOR       )  strcpy(text, "Objets d�coratifs\\Arbres, plantes, etc.");
		if ( num == EVENT_INTERFACE_DETAIL      )  strcpy(text, "Objets d�taill�s\\Objets d�taill�s");
		if ( num == EVENT_INTERFACE_METEO       )  strcpy(text, "Conditions m�t�o\\Pluie et neige");
		if ( num == EVENT_INTERFACE_AMBIANCE    )  strcpy(text, "Effets d'ambiance\\");
		if ( num == EVENT_INTERFACE_EXPLOVIB    )  strcpy(text, "Secousses lors d'explosions\\L'�cran vibre lors d'une explosion");
		if ( num == EVENT_INTERFACE_SPEEDSCH    )  strcpy(text, "Vitesse de d�filement horizontal\\");
		if ( num == EVENT_INTERFACE_SPEEDSCV    )  strcpy(text, "Vitesse de d�filement vertical\\");
		if ( num == EVENT_INTERFACE_MOUSESCROLL )  strcpy(text, "D�filement avec la souris\\");
		if ( num == EVENT_INTERFACE_INVSCH      )  strcpy(text, "Inversion d�filement horizontal\\");
		if ( num == EVENT_INTERFACE_INVSCV      )  strcpy(text, "Inversion d�filement vertical\\");
		if ( num == EVENT_INTERFACE_MOVIE       )  strcpy(text, "S�quences cin�matiques\\S�quences d'introduction");
		if ( num == EVENT_INTERFACE_HELP        )  strcpy(text, "Instructions et indices\\Textes explicatifs");
		if ( num == EVENT_INTERFACE_TOOLTIPS    )  strcpy(text, "Bulles d'aide\\Bulles explicatives");
		if ( num == EVENT_INTERFACE_NICEMOUSE   )  strcpy(text, "Souris ombr�e\\Jolie souris avec une ombre");
		if ( num == EVENT_INTERFACE_ACCEL       )  strcpy(text, "Acc�l�re si long � r�soudre\\Blupi va plus vite apr�s 10 minutes de r�flexion");
		if ( num == EVENT_INTERFACE_VOLBLUPI    )  strcpy(text, "Volume des voix de Blupi\\");
		if ( num == EVENT_INTERFACE_VOLSOUND    )  strcpy(text, "Volume des bruitages\\");
		if ( num == EVENT_INTERFACE_VOLAMBIANCE )  strcpy(text, "Volume d'ambiance\\");
		if ( num == EVENT_INTERFACE_SOUND3D     )  strcpy(text, "Sonido 3D\\Sonido 3D");

		if ( num == EVENT_INTERFACE_KDEF   )  strcpy(text, "Controles predeterminados\\Restablece todos los controles a los valores predeterminados");
		if ( num == EVENT_INTERFACE_KLEFT  )  strcpy(text, "Scroll left\\Scrolling to left");
		if ( num == EVENT_INTERFACE_KRIGHT )  strcpy(text, "Scroll right\\Scrolling to right");
		if ( num == EVENT_INTERFACE_KUP    )  strcpy(text, "Scroll up\\Scrolling to up");
		if ( num == EVENT_INTERFACE_KDOWN  )  strcpy(text, "Scroll down\\Scrolling to down");
		if ( num == EVENT_INTERFACE_KROTCW )  strcpy(text, "Rotate CW\\Rotate camera clockwise");
		if ( num == EVENT_INTERFACE_KROTCCW)  strcpy(text, "Rotate CCW\\Rotate camera counter clockwise");
		if ( num == EVENT_INTERFACE_KSTOP  )  strcpy(text, "Stoppe action\\Stoppe l'action en cours");
		if ( num == EVENT_INTERFACE_KQUIT  )  strcpy(text, "Salir\\Salir de la misi�n o carrera actual");
		if ( num == EVENT_INTERFACE_KHELP  )  strcpy(text, "Instrucciones\\Mostrar instrucciones para la misi�n actual");

		if ( num == EVENT_INTERFACE_MIN    )  strcpy(text, "M�s baja\\Calidad de gr�ficos m�nima (frecuencia de cuadro m�s alta)");
		if ( num == EVENT_INTERFACE_NORM   )  strcpy(text, "Normal\\Calidad de gr�ficos normal");
		if ( num == EVENT_INTERFACE_MAX    )  strcpy(text, "M�s alta\\Calidad de gr�ficos m�s alta (frecuencia de cuadro m�s baja)");

		if ( num == EVENT_INTERFACE_SILENT )  strcpy(text, "Silencio\\Sin sonido");
		if ( num == EVENT_INTERFACE_NOISY  )  strcpy(text, "Normal\\Volumen de sonido normal");

		if ( num == EVENT_INTERFACE_NEDIT  )  strcpy(text, "\\Nombre de nuevo conductor");
		if ( num == EVENT_INTERFACE_NOK    )  strcpy(text, "Seleccionar\\Elegir el conductor seleccionado");
		if ( num == EVENT_INTERFACE_NCREATE)  strcpy(text, "Nuevo\\Crear un conductor nuevo");
		if ( num == EVENT_INTERFACE_NDELETE)  strcpy(text, "Eliminar\\Eliminar al conductor de la lista");

		if ( num == EVENT_INTERFACE_PREV   )  strcpy(text, "\\Previous screen");
		if ( num == EVENT_INTERFACE_NEXT   )  strcpy(text, "\\Next screen");
		if ( num == EVENT_INTERFACE_EXIT   )  strcpy(text, "\\Quit BlupiMania");

		if ( num == EVENT_INTERFACE_UNDO   )  strcpy(text, "Annule la derni�re action");
		if ( num == EVENT_INTERFACE_STOP   )  strcpy(text, "Stop (\\key stop;)");
		if ( num == EVENT_INTERFACE_SIGN   )  strcpy(text, "Obtenir des indices");
		if ( num == EVENT_LABEL_SIGN       )  strcpy(text, "Indices");

		if ( num == EVENT_EDIT_WATER       )  strcpy(text, "Eau");
		if ( num == EVENT_EDIT_GROUND      )  strcpy(text, "Sol");
		if ( num == EVENT_EDIT_OBJECT      )  strcpy(text, "Objets");
		if ( num == EVENT_EDIT_INFO        )  strcpy(text, "Informations sur le puzzle");

		if ( num == EVENT_CMD              )  strcpy(text, "L�nea de comandos");
		if ( num == EVENT_SPEED            )  strcpy(text, "Velocidad de juego");
	}

	if ( type == RES_OBJECT )
	{
		if ( num == OBJECT_BLUPI           )  strcpy(text, "Blupi");
		if ( num == OBJECT_BOX1            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX2            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX3            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX4            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX5            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX6            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX7            )  strcpy(text, "Grosse boule");
		if ( num == OBJECT_BOX8            )  strcpy(text, "Colonne lourde");
		if ( num == OBJECT_BOX9            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX10           )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX11           )  strcpy(text, "Caisse sur roues");
		if ( num == OBJECT_BOX12           )  strcpy(text, "Caisse sur roues");
		if ( num == OBJECT_BOX13           )  strcpy(text, "Caisse sur coussin d'air");
		if ( num == OBJECT_KEY1            )  strcpy(text, "Cl� orange");
		if ( num == OBJECT_KEY2            )  strcpy(text, "Cl� bleue");
		if ( num == OBJECT_DOOR1           )  strcpy(text, "Porte orange");
		if ( num == OBJECT_DOOR2           )  strcpy(text, "Porte bleue");
		if ( num == OBJECT_DOOR3           )  strcpy(text, "Porte orange");
		if ( num == OBJECT_DOOR4           )  strcpy(text, "Porte bleue");
		if ( num == OBJECT_DOCK            )  strcpy(text, "Pont-grue");
		if ( num == OBJECT_CATAPULT        )  strcpy(text, "Catapulte");
		if ( num == OBJECT_TRAX            )  strcpy(text, "Pousseur m�canique");
		if ( num == OBJECT_PERFO           )  strcpy(text, "Perforateur m�canique");
		if ( num == OBJECT_GUN             )  strcpy(text, "Destructeur de caisses");
		if ( num == OBJECT_MINE            )  strcpy(text, "Mine");
		if ( num == OBJECT_GLASS1          )  strcpy(text, "Vitre blind�e");
		if ( num == OBJECT_GLASS2          )  strcpy(text, "Vitre blind�e");
		if ( num == OBJECT_FIOLE           )  strcpy(text, "Potion de force");
		if ( num == OBJECT_GLU             )  strcpy(text, "Potion de glu");
		if ( num == OBJECT_GOAL            )  strcpy(text, "But � atteindre");
		if ( num == OBJECT_CRAZY           )  strcpy(text, "Crazy bot");
		if ( num == OBJECT_BOT1            )  strcpy(text, "Bot 1");
		if ( num == OBJECT_BOT2            )  strcpy(text, "Bot 2");
		if ( num == OBJECT_BOT3            )  strcpy(text, "Bot 3");
		if ( num == OBJECT_BOT4            )  strcpy(text, "Bot 4");
		if ( num == OBJECT_BOT5            )  strcpy(text, "Bot 5");
	}

	if ( type == RES_ERR )
	{
		strcpy(text, "Error");
		if ( num == ERR_CMD             )  strcpy(text, "Comando desconocido");
		if ( num == ERR_INSTALL         )  strcpy(text, "Wild Wheels no se ha instalado correctamente.");
		if ( num == ERR_NOCD            )  strcpy(text, "Introduce el CD de Wild Wheels\ny ejecuta el juego.");
		if ( num == ERR_MOVE_IMPOSSIBLE )  strcpy(text, "Error al realizar la instrucci�n");
		if ( num == ERR_GOTO_IMPOSSIBLE )  strcpy(text, "Ir a: destino no accesible");
		if ( num == ERR_GOTO_ITER       )  strcpy(text, "Ir a: destino no accesible");
		if ( num == ERR_GOTO_BUSY       )  strcpy(text, "Ir a: destino ocupado");
		if ( num == ERR_FIRE_VEH        )  strcpy(text, "BOT inapropiado");
		if ( num == ERR_FIRE_ENERGY     )  strcpy(text, "No hay suficiente energ�a");
		if ( num == ERR_MISSION_NOTERM  )  strcpy(text, "A�n no se ha realizado la misi�n (pulsa la \\key help; para m�s informaci�n)");
		if ( num == ERR_PUZZLE_ZEROBLUPI)  strcpy(text, "Il doit y avoir au moins un Blupi !");
		if ( num == ERR_PUZZLE_ZEROGOAL )  strcpy(text, "Il doit y avoir au moins un ballon !");
		if ( num == ERR_PUZZLE_LOGOAL   )  strcpy(text, "Il n'y a pas assez de ballons !");
		if ( num == ERR_PUZZLE_HIGOAL   )  strcpy(text, "Il y a trop de ballons !");
		if ( num == ERR_PUZZLE_MAXBLUPI )  strcpy(text, "4 Blupi ou robots au maximum par puzzle !");
		if ( num == ERR_PUZZLE_MAXGOAL  )  strcpy(text, "8 ballons au maximum par puzzle !");
		if ( num == ERR_PUZZLE_MAX      )  strcpy(text, "Il n'est pas possible de cr�er plus d'objets de ce type !");
		if ( num == ERR_CREATE          )  strcpy(text, "Ce fichier existe d�j�.");

		if ( num == INFO_WIN            )  strcpy(text, "<< Enhorabuena, misi�n cumplida >>");
		if ( num == INFO_LOST           )  strcpy(text, "<< Lo sentimos, misi�n no lograda >>");
		if ( num == INFO_LOSTq          )  strcpy(text, "<< Lo sentimos, misi�n no lograda >>");
	}

	if ( type == RES_CBOT )
	{
		strcpy(text, "Error");
		if ( num == TX_OPENPAR       ) strcpy(text, "Falta par�ntesis de apertura");
		if ( num == TX_CLOSEPAR      ) strcpy(text, "Falta par�ntesis de cierre ");
		if ( num == TX_NOTBOOL       ) strcpy(text, "La expresi�n debe devolver un valor booleano");
		if ( num == TX_UNDEFVAR      ) strcpy(text, "No se ha declarado la variable");
		if ( num == TX_BADLEFT       ) strcpy(text, "Asignaci�n imposible");
		if ( num == TX_ENDOF         ) strcpy(text, "Falta terminador de punto y coma");
		if ( num == TX_OUTCASE       ) strcpy(text, "Instrucci�n ""case"" fuera de un bloque ""switch""");
		if ( num == TX_NOTERM        ) strcpy(text, "Instrucciones despu�s de la llave de cierre final");
		if ( num == TX_CLOSEBLK      ) strcpy(text, "Falta el final del bloque");
		if ( num == TX_ELSEWITHOUTIF ) strcpy(text, "Instrucci�n ""else"" sin su correspondiente ""if"" ");
		if ( num == TX_OPENBLK       ) strcpy(text, "Abriendo la llave que falta ");//d�but d'un bloc attendu?
		if ( num == TX_BADTYPE       ) strcpy(text, "Tipo incorrecto para la asignaci�n");
		if ( num == TX_REDEFVAR      ) strcpy(text, "Una variable no puede declararse dos veces");
		if ( num == TX_BAD2TYPE      ) strcpy(text, "Los tipos de los dos operadores son incompatibles ");
		if ( num == TX_UNDEFCALL     ) strcpy(text, "Funci�n desconocida");
		if ( num == TX_MISDOTS       ) strcpy(text, "Falta el signo "" : """);
		if ( num == TX_WHILE         ) strcpy(text, "Falta la palabra clave ""while""");
		if ( num == TX_BREAK         ) strcpy(text, "Instrucci�n ""break"" fuera de bucle");
		if ( num == TX_LABEL         ) strcpy(text, "Una etiqueta debe ir seguida de ""for"", ""while"", ""do"" o ""switch""");
		if ( num == TX_NOLABEL       ) strcpy(text, "Esta etiqueta no existe");// Cette �tiquette n'existe pas
		if ( num == TX_NOCASE        ) strcpy(text, "Falta la instrucci�n ""case""");
		if ( num == TX_BADNUM        ) strcpy(text, "Falta el n�mero");
		if ( num == TX_VOID          ) strcpy(text, "Par�metro nulo");
		if ( num == TX_NOTYP         ) strcpy(text, "Falta la declaraci�n de tipo");
		if ( num == TX_NOVAR         ) strcpy(text, "Falta el nombre de la variable");
		if ( num == TX_NOFONC        ) strcpy(text, "Falta el nombre de la funci�n");
		if ( num == TX_OVERPARAM     ) strcpy(text, "Demasiados par�metros");
		if ( num == TX_REDEF         ) strcpy(text, "La funci�n ya existe");
		if ( num == TX_LOWPARAM      ) strcpy(text, "Faltan par�metros ");
		if ( num == TX_BADPARAM      ) strcpy(text, "Ninguna funci�n de este nombre acepta este tipo de par�metro");
		if ( num == TX_NUMPARAM      ) strcpy(text, "Ninguna funci�n de este nombre acepta este n�mero de par�metros");
		if ( num == TX_NOITEM        ) strcpy(text, "No es un miembro de esta clase");
		if ( num == TX_DOT           ) strcpy(text, "Este objeto no es un miembro de una clase");
		if ( num == TX_NOCONST       ) strcpy(text, "Falta el constructor adecuado");
		if ( num == TX_REDEFCLASS    ) strcpy(text, "Esta clase ya existe");
		if ( num == TX_CLBRK         ) strcpy(text, """ Falta ] """);
		if ( num == TX_RESERVED      ) strcpy(text, "Palabra clave reservada del idioma CBOT");
		if ( num == TX_BADNEW        ) strcpy(text, "Argumento incorrecto para ""new""");
		if ( num == TX_OPBRK         ) strcpy(text, """ Se esperaba [ """);
		if ( num == TX_BADSTRING     ) strcpy(text, "Falta cadena");
		if ( num == TX_BADINDEX      ) strcpy(text, "Tipo de �ndice incorrecto");
		if ( num == TX_PRIVATE       ) strcpy(text, "Elemento privado");
		if ( num == TX_NOPUBLIC      ) strcpy(text, "Se requiere p�blico");
		if ( num == TX_DIVZERO       ) strcpy(text, "Dividiendo por cero");
		if ( num == TX_NOTINIT       ) strcpy(text, "Variable no inicializada");
		if ( num == TX_BADTHROW      ) strcpy(text, """throw"" rechaza el valor negativo");//C'est quoi, �a?
		if ( num == TX_NORETVAL      ) strcpy(text, "La funci�n no devolvi� ning�n valor ");
		if ( num == TX_NORUN         ) strcpy(text, "No se ejecuta ninguna funci�n");
		if ( num == TX_NOCALL        ) strcpy(text, "Llamando a una funci�n desconocida");
		if ( num == TX_NOCLASS       ) strcpy(text, "Esta clase no existe");
		if ( num == TX_NULLPT        ) strcpy(text, "Objeto desconocido");
		if ( num == TX_OPNAN         ) strcpy(text, "Operaci�n imposible con el valor ""nan""");
		if ( num == TX_OUTARRAY      ) strcpy(text, "Acceso fuera del l�mite de la matriz");
		if ( num == TX_STACKOVER     ) strcpy(text, "Pila desbordada");
		if ( num == TX_DELETEDPT     ) strcpy(text, "Objeto ilegal");
		if ( num == TX_FILEOPEN      ) strcpy(text, "No puede abrirse el archivo");
		if ( num == TX_NOTOPEN       ) strcpy(text, "Archivo sin abrir");
		if ( num == TX_ERRREAD       ) strcpy(text, "Error de lectura");
		if ( num == TX_ERRWRITE      ) strcpy(text, "Error de escritura");
	}

	if ( type == RES_KEY )
	{
		if ( num == 0                   )  strcpy(text, "< ninguno >");
		if ( num == VK_LEFT             )  strcpy(text, "Flecha Izquierda");
		if ( num == VK_RIGHT            )  strcpy(text, "Flecha Derecha");
		if ( num == VK_UP               )  strcpy(text, "Flecha Arriba");
		if ( num == VK_DOWN             )  strcpy(text, "Flecha Abajo");
		if ( num == VK_CANCEL           )  strcpy(text, "Control-interrupci�n");
		if ( num == VK_BACK             )  strcpy(text, "<--");
		if ( num == VK_TAB              )  strcpy(text, "Tab");
		if ( num == VK_CLEAR            )  strcpy(text, "Borrar");
		if ( num == VK_RETURN           )  strcpy(text, "Entrar");
		if ( num == VK_SHIFT            )  strcpy(text, "May�s");
		if ( num == VK_CONTROL          )  strcpy(text, "Ctrl");
		if ( num == VK_MENU             )  strcpy(text, "Alt");
		if ( num == VK_PAUSE            )  strcpy(text, "Pausa");
		if ( num == VK_CAPITAL          )  strcpy(text, "Bloq May�s");
		if ( num == VK_ESCAPE           )  strcpy(text, "Esc");
		if ( num == VK_SPACE            )  strcpy(text, "Barra espaciadora");
		if ( num == VK_PRIOR            )  strcpy(text, "Re P�g");
		if ( num == VK_NEXT             )  strcpy(text, "Av P�g");
		if ( num == VK_END              )  strcpy(text, "Fin");
		if ( num == VK_HOME             )  strcpy(text, "Inicio");
		if ( num == VK_SELECT           )  strcpy(text, "Seleccionar");
		if ( num == VK_EXECUTE          )  strcpy(text, "Ejecutar");
		if ( num == VK_SNAPSHOT         )  strcpy(text, "Impr Pant");
		if ( num == VK_INSERT           )  strcpy(text, "Insertar");
		if ( num == VK_DELETE           )  strcpy(text, "Eliminar");
		if ( num == VK_HELP             )  strcpy(text, "Ayuda");
		if ( num == VK_LWIN             )  strcpy(text, "Ventanas izquierdas");
		if ( num == VK_RWIN             )  strcpy(text, "Ventanas derechas");
		if ( num == VK_APPS             )  strcpy(text, "Tecla de aplicaci�n");
		if ( num == VK_NUMPAD0          )  strcpy(text, "Teclado num�rico 0");
		if ( num == VK_NUMPAD1          )  strcpy(text, "Teclado num�rico 1");
		if ( num == VK_NUMPAD2          )  strcpy(text, "Teclado num�rico 2");
		if ( num == VK_NUMPAD3          )  strcpy(text, "Teclado num�rico 3");
		if ( num == VK_NUMPAD4          )  strcpy(text, "Teclado num�rico 4");
		if ( num == VK_NUMPAD5          )  strcpy(text, "Teclado num�rico 5");
		if ( num == VK_NUMPAD6          )  strcpy(text, "Teclado num�rico 6");
		if ( num == VK_NUMPAD7          )  strcpy(text, "Teclado num�rico 7");
		if ( num == VK_NUMPAD8          )  strcpy(text, "Teclado num�rico 8");
		if ( num == VK_NUMPAD9          )  strcpy(text, "Teclado num�rico 9");
		if ( num == VK_MULTIPLY         )  strcpy(text, "Teclado num�rico *");
		if ( num == VK_ADD              )  strcpy(text, "Teclado num�rico +");
		if ( num == VK_SEPARATOR        )  strcpy(text, "Teclado num�rico separador");
		if ( num == VK_SUBTRACT         )  strcpy(text, "Teclado num�rico -");
		if ( num == VK_DECIMAL          )  strcpy(text, "Teclado num�rico .");
		if ( num == VK_DIVIDE           )  strcpy(text, "Teclado num�rico /");
		if ( num == VK_F1               )  strcpy(text, "F1");
		if ( num == VK_F2               )  strcpy(text, "F2");
		if ( num == VK_F3               )  strcpy(text, "F3");
		if ( num == VK_F4               )  strcpy(text, "F4");
		if ( num == VK_F5               )  strcpy(text, "F5");
		if ( num == VK_F6               )  strcpy(text, "F6");
		if ( num == VK_F7               )  strcpy(text, "F7");
		if ( num == VK_F8               )  strcpy(text, "F8");
		if ( num == VK_F9               )  strcpy(text, "F9");
		if ( num == VK_F10              )  strcpy(text, "F10");
		if ( num == VK_F11              )  strcpy(text, "F11");
		if ( num == VK_F12              )  strcpy(text, "F12");
		if ( num == VK_F13              )  strcpy(text, "F13");
		if ( num == VK_F14              )  strcpy(text, "F14");
		if ( num == VK_F15              )  strcpy(text, "F15");
		if ( num == VK_F16              )  strcpy(text, "F16");
		if ( num == VK_F17              )  strcpy(text, "F17");
		if ( num == VK_F18              )  strcpy(text, "F18");
		if ( num == VK_F19              )  strcpy(text, "F19");
		if ( num == VK_F20              )  strcpy(text, "F20");
		if ( num == VK_NUMLOCK          )  strcpy(text, "Bloq Num");
		if ( num == VK_SCROLL           )  strcpy(text, "Desplazar");
		if ( num == VK_ATTN             )  strcpy(text, "Atenci�n");
		if ( num == VK_CRSEL            )  strcpy(text, "CrSel");
		if ( num == VK_EXSEL            )  strcpy(text, "ExSel");
		if ( num == VK_EREOF            )  strcpy(text, "Borrar EOF");
		if ( num == VK_PLAY             )  strcpy(text, "Jugar");
		if ( num == VK_ZOOM             )  strcpy(text, "Zoom");
		if ( num == VK_PA1              )  strcpy(text, "PA1");
		if ( num == VK_OEM_CLEAR        )  strcpy(text, "Borrar");
		if ( num == VK_BUTTON1          )  strcpy(text, "Bot�n 1");
		if ( num == VK_BUTTON2          )  strcpy(text, "Bot�n 2");
		if ( num == VK_BUTTON3          )  strcpy(text, "Bot�n 3");
		if ( num == VK_BUTTON4          )  strcpy(text, "Bot�n 4");
		if ( num == VK_BUTTON5          )  strcpy(text, "Bot�n 5");
		if ( num == VK_BUTTON6          )  strcpy(text, "Bot�n 6");
		if ( num == VK_BUTTON7          )  strcpy(text, "Bot�n 7");
		if ( num == VK_BUTTON8          )  strcpy(text, "Bot�n 8");
		if ( num == VK_BUTTON9          )  strcpy(text, "Bot�n 9");
		if ( num == VK_BUTTON10         )  strcpy(text, "Bot�n 10");
		if ( num == VK_BUTTON11         )  strcpy(text, "Bot�n 11");
		if ( num == VK_BUTTON12         )  strcpy(text, "Bot�n 12");
		if ( num == VK_BUTTON13         )  strcpy(text, "Bot�n 13");
		if ( num == VK_BUTTON14         )  strcpy(text, "Bot�n 14");
		if ( num == VK_BUTTON15         )  strcpy(text, "Bot�n 15");
		if ( num == VK_BUTTON16         )  strcpy(text, "Bot�n 16");
		if ( num == VK_BUTTON17         )  strcpy(text, "Bot�n 17");
		if ( num == VK_BUTTON18         )  strcpy(text, "Bot�n 18");
		if ( num == VK_BUTTON19         )  strcpy(text, "Bot�n 19");
		if ( num == VK_BUTTON20         )  strcpy(text, "Bot�n 20");
		if ( num == VK_BUTTON21         )  strcpy(text, "Bot�n 21");
		if ( num == VK_BUTTON22         )  strcpy(text, "Bot�n 22");
		if ( num == VK_BUTTON23         )  strcpy(text, "Bot�n 23");
		if ( num == VK_BUTTON24         )  strcpy(text, "Bot�n 24");
		if ( num == VK_BUTTON25         )  strcpy(text, "Bot�n 25");
		if ( num == VK_BUTTON26         )  strcpy(text, "Bot�n 26");
		if ( num == VK_BUTTON27         )  strcpy(text, "Bot�n 27");
		if ( num == VK_BUTTON28         )  strcpy(text, "Bot�n 28");
		if ( num == VK_BUTTON29         )  strcpy(text, "Bot�n 29");
		if ( num == VK_BUTTON30         )  strcpy(text, "Bot�n 30");
		if ( num == VK_BUTTON31         )  strcpy(text, "Bot�n 31");
		if ( num == VK_BUTTON32         )  strcpy(text, "Bot�n 32");
		if ( num == VK_WHEELUP          )  strcpy(text, "Subir volante");
		if ( num == VK_WHEELDOWN        )  strcpy(text, "Bajar volante");
	}
#endif

#if _PORTUGUESE
	if ( type == RES_TEXT )
	{
		#if _FULL
		#if _EDU
		if ( num == RT_VERSION_ID          )  strcpy(text, "EDU 1.3 /p");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "1.3 /p");
		#endif
		#endif
		#if _DEMO
		if ( num == RT_VERSION_ID          )  strcpy(text, "Demo 1.3 /p");
		#endif
		if ( num == RT_WINDOW_MAXIMIZED    )  strcpy(text, "Maximizar");
		if ( num == RT_WINDOW_MINIMIZED    )  strcpy(text, "Minimizar");
		if ( num == RT_WINDOW_STANDARD     )  strcpy(text, "Tamanho normal");
		if ( num == RT_WINDOW_CLOSE        )  strcpy(text, "Fechar");

		if ( num == RT_KEY_OR              )  strcpy(text, " ou ");

		if ( num == RT_PLAY_LISTm          )  strcpy(text, " Selecione uma miss�o:");
		if ( num == RT_PLAY_LISTd          )  strcpy(text, " D�fis :");
		if ( num == RT_PLAY_LISTu          )  strcpy(text, " Atelier de %s :");
		if ( num == RT_PLAY_LISTp          )  strcpy(text, " Prot�tipos:");

		if ( num == RT_SETUP_DEVICE        )  strcpy(text, " Drivers:");
		if ( num == RT_SETUP_MODE          )  strcpy(text, " Resolu��o:");
		if ( num == RT_SETUP_KEY1          )  strcpy(text, "1) Primeiro clique na tecla que deseja redefinir.");
		if ( num == RT_SETUP_KEY2          )  strcpy(text, "2) Depois pressione a tecla que deseja usar.");

		if ( num == RT_PERSO_LIST          )  strcpy(text, "Lista de pilotos:");

		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Deseja sair de BlupiMania?");
		if ( num == RT_DIALOG_YES          )  strcpy(text, "Anular\\Anular a miss�o atual");
		if ( num == RT_DIALOG_NO           )  strcpy(text, "Continuar\\Continuar a miss�o atual");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Sair\\Sair de BlupiMania");
		if ( num == RT_DIALOG_NOQUIT       )  strcpy(text, "Continuar\\Continuar o jogo");
		if ( num == RT_DIALOG_DELGAME      )  strcpy(text, "Deseja excluir os jogos e os carros salvos de %s?");
		if ( num == RT_DIALOG_DELFILE      )  strcpy(text, "Excluir o arquivo existente %s?");
		if ( num == RT_DIALOG_YESDEL       )  strcpy(text, "Excluir");
		if ( num == RT_DIALOG_NODEL        )  strcpy(text, "Cancelar");
		if ( num == RT_DIALOG_NEWGAME      )  strcpy(text, "Nome do piloto:");
		if ( num == RT_DIALOG_YESNEW       )  strcpy(text, "Criar");
		if ( num == RT_DIALOG_NONEW        )  strcpy(text, "Cancelar");
		if ( num == RT_DIALOG_PLAY         )  strcpy(text, "R�soudre le puzzle...");
		if ( num == RT_DIALOG_REPLAY       )  strcpy(text, "Rejouer pour le plaisir...");
		if ( num == RT_DIALOG_QUITEDIT     )  strcpy(text, "Enregistrer les modifications ?");
		if ( num == RT_DIALOG_YESQUITEDIT  )  strcpy(text, "Oui\\Enregistrer et quitter");
		if ( num == RT_DIALOG_NOQUITEDIT   )  strcpy(text, "Non\\Quitter sans enregistrer");
		if ( num == RT_DIALOG_CANQUITEDIT  )  strcpy(text, "Annuler\\Continuer l'�dition");
		if ( num == RT_DIALOG_NEWPUZZLE    )  strcpy(text, "Voulez-vous cr�er un nouveau puzzle \"%s\" ?");
		if ( num == RT_DIALOG_YESNEWP      )  strcpy(text, "Cr�er");
		if ( num == RT_DIALOG_NONEWP       )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_DELPUZ       )  strcpy(text, "Voulez-vous supprimer le puzzle ?");
		if ( num == RT_DIALOG_DELPUZd      )  strcpy(text, "(pour tous les joueurs)");
		if ( num == RT_DIALOG_DELPUZu      )  strcpy(text, "(seulement dans votre atelier)");
		if ( num == RT_DIALOG_YESDELPUZ    )  strcpy(text, "Supprimer");
		if ( num == RT_DIALOG_NODELPUZ     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_RESUMEINFO   )  strcpy(text, "Instructions");
		if ( num == RT_DIALOG_AUTHORINFO   )  strcpy(text, "Auteur");
		if ( num == RT_DIALOG_SIGNINFO     )  strcpy(text, "Indices");
		if ( num == RT_DIALOG_YESINFO      )  strcpy(text, "OK");
		if ( num == RT_DIALOG_YESERROR     )  strcpy(text, "OK");
		if ( num == RT_DIALOG_EXPORTGAME   )  strcpy(text, "O� voulez-vous exporter le casse-t�te ?");
		if ( num == RT_DIALOG_YESEXPORT    )  strcpy(text, "Exporter");
		if ( num == RT_DIALOG_NOEXPORT     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_IMPORTGAME   )  strcpy(text, "Casse-t�te contenus dans \"Mes documents\" :");
		if ( num == RT_DIALOG_YESIMPORT    )  strcpy(text, "Importer");
		if ( num == RT_DIALOG_NOIMPORT     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_TEST         )  strcpy(text, "Tester");
		if ( num == RT_DIALOG_DEFI         )  strcpy(text, "Relever le d�fi...");
		if ( num == RT_DIALOG_FILE         )  strcpy(text, "Fichier");
		if ( num == RT_DIALOG_RENAMEGAME   )  strcpy(text, "Changement du nom du fichier du casse-t�te.");
		if ( num == RT_DIALOG_YESRENAME    )  strcpy(text, "Renommer");
		if ( num == RT_DIALOG_NORENAME     )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_RENAMEOLD    )  strcpy(text, "Nom actuel");
		if ( num == RT_DIALOG_RENAMENEW    )  strcpy(text, "Nouveau nom");

		if ( num == RT_GENERIC_DEV1        )  strcpy(text, "www.epsitec.com");
		if ( num == RT_GENERIC_DEV2        )  strcpy(text, "Desenvolvido por EPSITEC");
		if ( num == RT_GENERIC_DEMO1       )  strcpy(text, "Voc� jogou com a vers�o DEMO de BlupiMania.");
		if ( num == RT_GENERIC_DEMO2       )  strcpy(text, "A vers�o completa est� agora dispon�vel...");
		if ( num == RT_GENERIC_DEMO3       )  strcpy(text, "... cont�m 28 miss�es cheias de desafios e 9 corridas...");
		if ( num == RT_GENERIC_DEMO4       )  strcpy(text, "... bem como um modo \"duelo\" para comparar desempenhos.");
		if ( num == RT_GENERIC_DEMO5       )  strcpy(text, "Mais informa��es em www.epsitec.com!");

		if ( num == RT_ACTION_GOTO         )  strcpy(text, "Aller");
		if ( num == RT_ACTION_PUSH1        )  strcpy(text, "Pousser une fois");
		if ( num == RT_ACTION_PUSHx        )  strcpy(text, "Pousser %d fois");
		if ( num == RT_ACTION_PUSHn        )  strcpy(text, "Pousser toujours");
		if ( num == RT_ACTION_PULL1        )  strcpy(text, "Tirer une fois");
		if ( num == RT_ACTION_ROLL         )  strcpy(text, "Rouler");
		if ( num == RT_ACTION_DOCKm        )  strcpy(text, "Actionner");
		if ( num == RT_ACTION_DOCKmx       )  strcpy(text, "Actionner %d fois");
		if ( num == RT_ACTION_DOCKg        )  strcpy(text, "Saisir/d�poser");
		if ( num == RT_ACTION_TRAXa        )  strcpy(text, "Avancer");
		if ( num == RT_ACTION_TRAXax       )  strcpy(text, "Avancer %d fois");
		if ( num == RT_ACTION_TRAXl        )  strcpy(text, "Tourner � droite");
		if ( num == RT_ACTION_TRAXlx       )  strcpy(text, "Tourner � droite %d fois");
		if ( num == RT_ACTION_TRAXr        )  strcpy(text, "Tourner � gauche");
		if ( num == RT_ACTION_TRAXrx       )  strcpy(text, "Tourner � gauche %d fois");
		if ( num == RT_ACTION_GUNa         )  strcpy(text, "Avancer puis tirer");
		if ( num == RT_ACTION_GUNax        )  strcpy(text, "Avancer puis tirer %d fois");
		if ( num == RT_ACTION_GUNl         )  strcpy(text, "Tourner � droite puis tirer");
		if ( num == RT_ACTION_GUNlx        )  strcpy(text, "Tourner � droite puis tirer %d fois");
		if ( num == RT_ACTION_GUNr         )  strcpy(text, "Tourner � gauche puis tirer");
		if ( num == RT_ACTION_GUNrx        )  strcpy(text, "Tourner � gauche puis tirer %d fois");
		if ( num == RT_ACTION_CATAPULT     )  strcpy(text, "Lancer");

		if ( num == RT_ADVISE_LITGOTO      )  strcpy(text, "...");
		if ( num == RT_ADVISE_GOTOPUSH     )  strcpy(text, "...");
		if ( num == RT_ADVISE_NOSIGN       )  strcpy(text, "Aucun indice disponible pour ce casse-t�te.");

		if ( num == RT_USER_HEADER         )  strcpy(text, "Fichier\tUnivers\tTest�\tInstructions");
		if ( num == RT_DEFI_HEADER         )  strcpy(text, "Fichier\tUnivers\tAuteur\tInstructions");
		if ( num == RT_IMPORT_HEADER       )  strcpy(text, "Fichier\tUnivers\tAuteur\tInstructions");

		if ( num == RT_DEMO                )  strcpy(text, "Pas disponible dans la DEMO");

		if ( num == RT_UNIVERS0            )  strcpy(text, "Castella");
		if ( num == RT_UNIVERS1            )  strcpy(text, "Paradiso");
		if ( num == RT_UNIVERS2            )  strcpy(text, "Volcano");
		if ( num == RT_UNIVERS3            )  strcpy(text, "Mecano");
		if ( num == RT_UNIVERS4            )  strcpy(text, "Sahara");
		if ( num == RT_UNIVERS5            )  strcpy(text, "Aglagla");
		if ( num == RT_UNIVERS6            )  strcpy(text, "Samba");
		if ( num == RT_UNIVERS7            )  strcpy(text, "Medieva");
		if ( num == RT_UNIVERS8            )  strcpy(text, "Vegeta");
		if ( num == RT_UNIVERS9            )  strcpy(text, "Inferno");
		if ( num == RT_UNIVERS10           )  strcpy(text, "Futura");
		if ( num == RT_UNIVERS11           )  strcpy(text, "Alibaba");
		if ( num == RT_UNIVERS12           )  strcpy(text, "Studio");
		if ( num == RT_UNIVERS13           )  strcpy(text, "Cata");
		if ( num == RT_UNIVERS14           )  strcpy(text, "Allegro");
		if ( num == RT_UNIVERS15           )  strcpy(text, "Cemento");
		if ( num == RT_UNIVERS16           )  strcpy(text, "Labo");
		if ( num == RT_UNIVERS17           )  strcpy(text, "Drinka");
		if ( num == RT_UNIVERS18           )  strcpy(text, "Woodo");
		if ( num == RT_UNIVERS19           )  strcpy(text, "Natura");
	}

	if ( type == RES_EVENT )
	{
		if ( num == EVENT_BUTTON_OK        )  strcpy(text, "OK");
		if ( num == EVENT_BUTTON_CANCEL    )  strcpy(text, "Cancelar");
		if ( num == EVENT_BUTTON_QUIT      )  strcpy(text, "Menu (\\key quit;)");

		if ( num == EVENT_DIALOG_OK        )  strcpy(text, "OK");
		if ( num == EVENT_DIALOG_CANCEL    )  strcpy(text, "Cancelar");

		if ( num == EVENT_INTERFACE_PUZZLE )  strcpy(text, "Casse-t�te\\Torture progressive");
		if ( num == EVENT_INTERFACE_DEFI   )  strcpy(text, "D�fis\\Casse-t�te d'autres joueurs");
		if ( num == EVENT_INTERFACE_USER   )  strcpy(text, "Atelier\\Cr�ez vos casse-t�te");
		if ( num == EVENT_INTERFACE_PROTO  )  strcpy(text, "Proto\\Prot�tipos em desenvolvimento");
		if ( num == EVENT_INTERFACE_NAME   )  strcpy(text, "Novo piloto\\Escolha o nome do piloto");
		if ( num == EVENT_INTERFACE_SETUP  )  strcpy(text, "Op��es\\Prefer�ncias");
		if ( num == EVENT_INTERFACE_AGAIN  )  strcpy(text, "Reiniciar\\Reinicia a miss�o a partir do come�o");
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Volta ao BlupiMania");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Sair\\Sai de BlupiMania");
		if ( num == EVENT_INTERFACE_BACK   )  strcpy(text, "Cancelar\\Volta � tela anterior");
		if ( num == EVENT_INTERFACE_TERM   )  strcpy(text, "Menu\\Volta ao menu principal");
		if ( num == EVENT_INTERFACE_PLAY   )  strcpy(text, "Reproduzir\\Inicia miss�o selecionada!");
		if ( num == EVENT_INTERFACE_NEW    )  strcpy(text, "Nouveau\\Cr�er un nouveau puzzle");
		if ( num == EVENT_INTERFACE_DELETE )  strcpy(text, "Supprimer\\Supprimer le puzzle");
		if ( num == EVENT_INTERFACE_RENAME )  strcpy(text, "Renommer\\Renommer le casse-t�te");
		if ( num == EVENT_INTERFACE_EXPORT )  strcpy(text, "Exporter\\Exporter le casse-t�te");
		if ( num == EVENT_INTERFACE_IMPORT )  strcpy(text, "Importer\\Importer un casse-t�te");
		if ( num == EVENT_INTERFACE_EDIT   )  strcpy(text, "Modifier\\Modifier le puzzle");
		if ( num == EVENT_INTERFACE_OK     )  strcpy(text, "OK\\Volta � tela anterior");

		if ( num == EVENT_INTERFACE_EXPORTdefi )  strcpy(text, "Dans les d�fis (pour tous les joueurs)");
		if ( num == EVENT_INTERFACE_EXPORTdoc  )  strcpy(text, "Dans le dossier \"Mes documents\"");

		if ( num == EVENT_INTERFACE_SETUPd )  strcpy(text, "Dispositivo\\Configura��es de driver e resolu��o");
		if ( num == EVENT_INTERFACE_SETUPg )  strcpy(text, "Gr�ficos\\Configura��es de gr�ficos");
		if ( num == EVENT_INTERFACE_SETUPp )  strcpy(text, "Jogo\\Configura��es do jogo");
		if ( num == EVENT_INTERFACE_SETUPc )  strcpy(text, "Controles\\Configura��es do teclado, volante e gamepad");
		if ( num == EVENT_INTERFACE_SETUPs )  strcpy(text, "Som\\Volume de m�sica e efeitos sonoros");
		if ( num == EVENT_INTERFACE_RESOL  )  strcpy(text, "Resolu��o");
		if ( num == EVENT_INTERFACE_FULL   )  strcpy(text, "Tela cheia\\Tela cheia ou modo de janela");
		if ( num == EVENT_INTERFACE_APPLY  )  strcpy(text, "Aplicar altera��es\\Ativa as configura��es alteradas");

		if ( num == EVENT_INTERFACE_SHADOW      )  strcpy(text, "Ombres\\Ombres projet�es au sol");
		if ( num == EVENT_INTERFACE_DIRTY       )  strcpy(text, "Salissures\\Salissures des object");
		if ( num == EVENT_INTERFACE_SUNBEAM     )  strcpy(text, "Rayons du soleil\\Rayons selon l'orientation");
		if ( num == EVENT_INTERFACE_LENSFLARE   )  strcpy(text, "Reflets dans l'objectif\\Reflets selon l'orientation");
		if ( num == EVENT_INTERFACE_DECOR       )  strcpy(text, "Objets d�coratifs\\Arbres, plantes, etc.");
		if ( num == EVENT_INTERFACE_DETAIL      )  strcpy(text, "Objets d�taill�s\\Objets d�taill�s");
		if ( num == EVENT_INTERFACE_METEO       )  strcpy(text, "Conditions m�t�o\\Pluie et neige");
		if ( num == EVENT_INTERFACE_AMBIANCE    )  strcpy(text, "Effets d'ambiance\\");
		if ( num == EVENT_INTERFACE_EXPLOVIB    )  strcpy(text, "Secousses lors d'explosions\\L'�cran vibre lors d'une explosion");
		if ( num == EVENT_INTERFACE_SPEEDSCH    )  strcpy(text, "Vitesse de d�filement horizontal\\");
		if ( num == EVENT_INTERFACE_SPEEDSCV    )  strcpy(text, "Vitesse de d�filement vertical\\");
		if ( num == EVENT_INTERFACE_MOUSESCROLL )  strcpy(text, "D�filement avec la souris\\");
		if ( num == EVENT_INTERFACE_INVSCH      )  strcpy(text, "Inversion d�filement horizontal\\");
		if ( num == EVENT_INTERFACE_INVSCV      )  strcpy(text, "Inversion d�filement vertical\\");
		if ( num == EVENT_INTERFACE_MOVIE       )  strcpy(text, "S�quences cin�matiques\\S�quences d'introduction");
		if ( num == EVENT_INTERFACE_HELP        )  strcpy(text, "Instructions et indices\\Textes explicatifs");
		if ( num == EVENT_INTERFACE_TOOLTIPS    )  strcpy(text, "Bulles d'aide\\Bulles explicatives");
		if ( num == EVENT_INTERFACE_NICEMOUSE   )  strcpy(text, "Souris ombr�e\\Jolie souris avec une ombre");
		if ( num == EVENT_INTERFACE_ACCEL       )  strcpy(text, "Acc�l�re si long � r�soudre\\Blupi va plus vite apr�s 10 minutes de r�flexion");
		if ( num == EVENT_INTERFACE_VOLBLUPI    )  strcpy(text, "Volume des voix de Blupi\\");
		if ( num == EVENT_INTERFACE_VOLSOUND    )  strcpy(text, "Volume des bruitages\\");
		if ( num == EVENT_INTERFACE_VOLAMBIANCE )  strcpy(text, "Volume d'ambiance\\");
		if ( num == EVENT_INTERFACE_SOUND3D     )  strcpy(text, "Som 3D\\Som 3D");

		if ( num == EVENT_INTERFACE_KDEF   )  strcpy(text, "Controles padr�o\\Redefine todos os controles para os valores padr�o");
		if ( num == EVENT_INTERFACE_KLEFT  )  strcpy(text, "Scroll left\\Scrolling to left");
		if ( num == EVENT_INTERFACE_KRIGHT )  strcpy(text, "Scroll right\\Scrolling to right");
		if ( num == EVENT_INTERFACE_KUP    )  strcpy(text, "Scroll up\\Scrolling to up");
		if ( num == EVENT_INTERFACE_KDOWN  )  strcpy(text, "Scroll down\\Scrolling to down");
		if ( num == EVENT_INTERFACE_KROTCW )  strcpy(text, "Rotate CW\\Rotate camera clockwise");
		if ( num == EVENT_INTERFACE_KROTCCW)  strcpy(text, "Rotate CCW\\Rotate camera counter clockwise");
		if ( num == EVENT_INTERFACE_KSTOP  )  strcpy(text, "Stoppe action\\Stoppe l'action en cours");
		if ( num == EVENT_INTERFACE_KQUIT  )  strcpy(text, "Sair\\Sai da miss�o ou da corrida atual");
		if ( num == EVENT_INTERFACE_KHELP  )  strcpy(text, "Instru��es\\Mostra as instru��es para a miss�o atual");

		if ( num == EVENT_INTERFACE_MIN    )  strcpy(text, "M�nima\\Qualidade gr�fica m�nima (a mais alta taxa de quadros)");
		if ( num == EVENT_INTERFACE_NORM   )  strcpy(text, "Normal\\Qualidade gr�fica normal");
		if ( num == EVENT_INTERFACE_MAX    )  strcpy(text, "M�xima\\Qualidade gr�fica mais alta (a mais baixa taxa de quadros)");

		if ( num == EVENT_INTERFACE_SILENT )  strcpy(text, "Sil�ncio\\Sem som");
		if ( num == EVENT_INTERFACE_NOISY  )  strcpy(text, "Normal\\Volume de som normal");

		if ( num == EVENT_INTERFACE_NEDIT  )  strcpy(text, "\\Novo nome de piloto");
		if ( num == EVENT_INTERFACE_NOK    )  strcpy(text, "Selecionar\\Escolha o piloto selecionado");
		if ( num == EVENT_INTERFACE_NCREATE)  strcpy(text, "Novo\\Cria um novo piloto");
		if ( num == EVENT_INTERFACE_NDELETE)  strcpy(text, "Excluir\\Exclui o piloto da lista");

		if ( num == EVENT_INTERFACE_PREV   )  strcpy(text, "\\Previous screen");
		if ( num == EVENT_INTERFACE_NEXT   )  strcpy(text, "\\Next screen");
		if ( num == EVENT_INTERFACE_EXIT   )  strcpy(text, "\\Quit BlupiMania");

		if ( num == EVENT_INTERFACE_UNDO   )  strcpy(text, "Annule la derni�re action");
		if ( num == EVENT_INTERFACE_STOP   )  strcpy(text, "Stop (\\key stop;)");
		if ( num == EVENT_INTERFACE_SIGN   )  strcpy(text, "Obtenir des indices");
		if ( num == EVENT_LABEL_SIGN       )  strcpy(text, "Indices");

		if ( num == EVENT_EDIT_WATER       )  strcpy(text, "Eau");
		if ( num == EVENT_EDIT_GROUND      )  strcpy(text, "Sol");
		if ( num == EVENT_EDIT_OBJECT      )  strcpy(text, "Objets");
		if ( num == EVENT_EDIT_INFO        )  strcpy(text, "Informations sur le puzzle");

		if ( num == EVENT_CMD              )  strcpy(text, "Linha de comando");
		if ( num == EVENT_SPEED            )  strcpy(text, "Velocidade do jogo");
	}

	if ( type == RES_OBJECT )
	{
		if ( num == OBJECT_BLUPI           )  strcpy(text, "Blupi");
		if ( num == OBJECT_BOX1            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX2            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX3            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX4            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX5            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX6            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX7            )  strcpy(text, "Grosse boule");
		if ( num == OBJECT_BOX8            )  strcpy(text, "Colonne lourde");
		if ( num == OBJECT_BOX9            )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX10           )  strcpy(text, "Caisse");
		if ( num == OBJECT_BOX11           )  strcpy(text, "Caisse sur roues");
		if ( num == OBJECT_BOX12           )  strcpy(text, "Caisse sur roues");
		if ( num == OBJECT_BOX13           )  strcpy(text, "Caisse sur coussin d'air");
		if ( num == OBJECT_KEY1            )  strcpy(text, "Cl� orange");
		if ( num == OBJECT_KEY2            )  strcpy(text, "Cl� bleue");
		if ( num == OBJECT_DOOR1           )  strcpy(text, "Porte orange");
		if ( num == OBJECT_DOOR2           )  strcpy(text, "Porte bleue");
		if ( num == OBJECT_DOOR3           )  strcpy(text, "Porte orange");
		if ( num == OBJECT_DOOR4           )  strcpy(text, "Porte bleue");
		if ( num == OBJECT_DOCK            )  strcpy(text, "Pont-grue");
		if ( num == OBJECT_CATAPULT        )  strcpy(text, "Catapulte");
		if ( num == OBJECT_TRAX            )  strcpy(text, "Pousseur m�canique");
		if ( num == OBJECT_PERFO           )  strcpy(text, "Perforateur m�canique");
		if ( num == OBJECT_GUN             )  strcpy(text, "Destructeur de caisses");
		if ( num == OBJECT_MINE            )  strcpy(text, "Mine");
		if ( num == OBJECT_GLASS1          )  strcpy(text, "Vitre blind�e");
		if ( num == OBJECT_GLASS2          )  strcpy(text, "Vitre blind�e");
		if ( num == OBJECT_FIOLE           )  strcpy(text, "Potion de force");
		if ( num == OBJECT_GLU             )  strcpy(text, "Potion de glu");
		if ( num == OBJECT_GOAL            )  strcpy(text, "But � atteindre");
		if ( num == OBJECT_CRAZY           )  strcpy(text, "Crazy bot");
		if ( num == OBJECT_BOT1            )  strcpy(text, "Bot 1");
		if ( num == OBJECT_BOT2            )  strcpy(text, "Bot 2");
		if ( num == OBJECT_BOT3            )  strcpy(text, "Bot 3");
		if ( num == OBJECT_BOT4            )  strcpy(text, "Bot 4");
		if ( num == OBJECT_BOT5            )  strcpy(text, "Bot 5");
	}

	if ( type == RES_ERR )
	{
		strcpy(text, "Erro");
		if ( num == ERR_CMD             )  strcpy(text, "Comando desconhecido");
		if ( num == ERR_INSTALL         )  strcpy(text, "Wild Wheels n�o foi instalado corretamente.");
		if ( num == ERR_NOCD            )  strcpy(text, "Insira o CD de Wild Wheels\ne reexecute o jogo.");
		if ( num == ERR_MOVE_IMPOSSIBLE )  strcpy(text, "Erro em movimento de instru��o");
		if ( num == ERR_GOTO_IMPOSSIBLE )  strcpy(text, "Ir a: destino inacess�vel");
		if ( num == ERR_GOTO_ITER       )  strcpy(text, "Ir a: destino inacess�vel");
		if ( num == ERR_GOTO_BUSY       )  strcpy(text, "Ir a: destino ocupado");
		if ( num == ERR_FIRE_VEH        )  strcpy(text, "Bot inapropriado");
		if ( num == ERR_FIRE_ENERGY     )  strcpy(text, "Energia insuficiente");
		if ( num == ERR_MISSION_NOTERM  )  strcpy(text, "A miss�o ainda n�o foi conclu�da (pressione \\key help; para mais detalhes)");
		if ( num == ERR_PUZZLE_ZEROBLUPI)  strcpy(text, "Il doit y avoir au moins un Blupi !");
		if ( num == ERR_PUZZLE_ZEROGOAL )  strcpy(text, "Il doit y avoir au moins un ballon !");
		if ( num == ERR_PUZZLE_LOGOAL   )  strcpy(text, "Il n'y a pas assez de ballons !");
		if ( num == ERR_PUZZLE_HIGOAL   )  strcpy(text, "Il y a trop de ballons !");
		if ( num == ERR_PUZZLE_MAXBLUPI )  strcpy(text, "4 Blupi ou robots au maximum par puzzle !");
		if ( num == ERR_PUZZLE_MAXGOAL  )  strcpy(text, "8 ballons au maximum par puzzle !");
		if ( num == ERR_PUZZLE_MAX      )  strcpy(text, "Il n'est pas possible de cr�er plus d'objets de ce type !");
		if ( num == ERR_CREATE          )  strcpy(text, "Ce fichier existe d�j�.");

		if ( num == INFO_WIN            )  strcpy(text, "<< Parab�ns, miss�o conclu�da >>");
		if ( num == INFO_LOST           )  strcpy(text, "<< Desculpe, miss�o fracassada >>");
		if ( num == INFO_LOSTq          )  strcpy(text, "<< Desculpe, miss�o fracassada >>");
	}

	if ( type == RES_CBOT )
	{
		strcpy(text, "Erro");
		if ( num == TX_OPENPAR       ) strcpy(text, "Par�ntese de abertura faltando");
		if ( num == TX_CLOSEPAR      ) strcpy(text, "Par�ntese de fechamento faltando ");
		if ( num == TX_NOTBOOL       ) strcpy(text, "A express�o deve retornar um valor booleano");
		if ( num == TX_UNDEFVAR      ) strcpy(text, "Vari�vel n�o declarada");
		if ( num == TX_BADLEFT       ) strcpy(text, "Atribui��o imposs�vel");
		if ( num == TX_ENDOF         ) strcpy(text, "Finalizador de ponto e v�rgula faltando");
		if ( num == TX_OUTCASE       ) strcpy(text, "Instru��o ""case"" fora de um bloco ""switch""");
		if ( num == TX_NOTERM        ) strcpy(text, "Instru��es depois do colchete de fechamento final");
		if ( num == TX_CLOSEBLK      ) strcpy(text, "Fim de bloco faltando");
		if ( num == TX_ELSEWITHOUTIF ) strcpy(text, "Instru��o ""else"" sem ""if"" correspondente ");
		if ( num == TX_OPENBLK       ) strcpy(text, "Colchete de abertura faltando ");//d�but d'un bloc attendu?
		if ( num == TX_BADTYPE       ) strcpy(text, "Tipo errado para a atribui��o");
		if ( num == TX_REDEFVAR      ) strcpy(text, "Uma vari�vel n�o pode ser declarada duas vezes");
		if ( num == TX_BAD2TYPE      ) strcpy(text, "Os tipos dos dois operandos s�o incompat�veis ");
		if ( num == TX_UNDEFCALL     ) strcpy(text, "Fun��o desconhecida");
		if ( num == TX_MISDOTS       ) strcpy(text, "Sinal "" : "" faltando");
		if ( num == TX_WHILE         ) strcpy(text, "Palavra-chave ""while"" faltando");
		if ( num == TX_BREAK         ) strcpy(text, "Instru��o ""break"" fora de um loop");
		if ( num == TX_LABEL         ) strcpy(text, "Uma etiqueta deve ser seguida de ""for"", ""while"", ""do"" ou ""switch""");
		if ( num == TX_NOLABEL       ) strcpy(text, "Esta etiqueta n�o existe");// Cette �tiquette n'existe pas
		if ( num == TX_NOCASE        ) strcpy(text, "Instru��o ""case"" faltando");
		if ( num == TX_BADNUM        ) strcpy(text, "N�mero faltando");
		if ( num == TX_VOID          ) strcpy(text, "Par�metro nulo");
		if ( num == TX_NOTYP         ) strcpy(text, "Declara��o de tipo faltando");
		if ( num == TX_NOVAR         ) strcpy(text, "Nome de vari�vel faltando");
		if ( num == TX_NOFONC        ) strcpy(text, "Nome de fun��o faltando");
		if ( num == TX_OVERPARAM     ) strcpy(text, "Muitos par�metros");
		if ( num == TX_REDEF         ) strcpy(text, "A fun��o j� existe");
		if ( num == TX_LOWPARAM      ) strcpy(text, "Par�metros faltando ");
		if ( num == TX_BADPARAM      ) strcpy(text, "Nenhuma fun��o deste nome aceita este tipo de par�metro");
		if ( num == TX_NUMPARAM      ) strcpy(text, "Nenhuma fun��o deste nome aceita este n�mero de par�metros");
		if ( num == TX_NOITEM        ) strcpy(text, "Este n�o � um membro desta classe");
		if ( num == TX_DOT           ) strcpy(text, "Este objeto n�o � um membro de uma classe");
		if ( num == TX_NOCONST       ) strcpy(text, "Construtor apropriado faltando");
		if ( num == TX_REDEFCLASS    ) strcpy(text, "Esta classe j� existe");
		if ( num == TX_CLBRK         ) strcpy(text, """ ] "" faltando");
		if ( num == TX_RESERVED      ) strcpy(text, "Palavra-chave reservada de linguagem CBOT");
		if ( num == TX_BADNEW        ) strcpy(text, "Argumento incorreto para ""new""");
		if ( num == TX_OPBRK         ) strcpy(text, """ [ "" esperado");
		if ( num == TX_BADSTRING     ) strcpy(text, "Seq��ncia faltando");
		if ( num == TX_BADINDEX      ) strcpy(text, "Tipo de �ndice incorreto");
		if ( num == TX_PRIVATE       ) strcpy(text, "Elementro privado");
		if ( num == TX_NOPUBLIC      ) strcpy(text, "P�blico requerido");
		if ( num == TX_DIVZERO       ) strcpy(text, "Dividindo por zero");
		if ( num == TX_NOTINIT       ) strcpy(text, "Vari�vel n�o inicializada");
		if ( num == TX_BADTHROW      ) strcpy(text, "Valor negativo rejeitado por ""throw""");//C'est quoi, �a?
		if ( num == TX_NORETVAL      ) strcpy(text, "A fun��o n�o retornou um valor ");
		if ( num == TX_NORUN         ) strcpy(text, "Nenhuma fun��o em execu��o");
		if ( num == TX_NOCALL        ) strcpy(text, "Chamando uma fun��o desconhecida");
		if ( num == TX_NOCLASS       ) strcpy(text, "Esta classe n�o existe");
		if ( num == TX_NULLPT        ) strcpy(text, "Objeto desconhecido");
		if ( num == TX_OPNAN         ) strcpy(text, "Opera��o imposs�vel com valor ""nan""");
		if ( num == TX_OUTARRAY      ) strcpy(text, "Acesso al�m do limite de matriz");
		if ( num == TX_STACKOVER     ) strcpy(text, "Estouro de pilha");
		if ( num == TX_DELETEDPT     ) strcpy(text, "Objeto ilegal");
		if ( num == TX_FILEOPEN      ) strcpy(text, "N�o � poss�vel abrir arquivo");
		if ( num == TX_NOTOPEN       ) strcpy(text, "O arquivo n�o est� aberto");
		if ( num == TX_ERRREAD       ) strcpy(text, "Erro de leitura");
		if ( num == TX_ERRWRITE      ) strcpy(text, "Erro de grava��o");
	}

	if ( type == RES_KEY )
	{
		if ( num == 0                   )  strcpy(text, "< nenhum >");
		if ( num == VK_LEFT             )  strcpy(text, "Seta esquerda");
		if ( num == VK_RIGHT            )  strcpy(text, "Seta direita");
		if ( num == VK_UP               )  strcpy(text, "Seta acima");
		if ( num == VK_DOWN             )  strcpy(text, "Seta abaixo");
		if ( num == VK_CANCEL           )  strcpy(text, "Control-break");
		if ( num == VK_BACK             )  strcpy(text, "<--");
		if ( num == VK_TAB              )  strcpy(text, "Guia");
		if ( num == VK_CLEAR            )  strcpy(text, "Limpar");
		if ( num == VK_RETURN           )  strcpy(text, "Enter");
		if ( num == VK_SHIFT            )  strcpy(text, "Shift");
		if ( num == VK_CONTROL          )  strcpy(text, "Ctrl");
		if ( num == VK_MENU             )  strcpy(text, "Alt");
		if ( num == VK_PAUSE            )  strcpy(text, "Pause");
		if ( num == VK_CAPITAL          )  strcpy(text, "Caps Lock");
		if ( num == VK_ESCAPE           )  strcpy(text, "Esc");
		if ( num == VK_SPACE            )  strcpy(text, "Barra de espa�amento");
		if ( num == VK_PRIOR            )  strcpy(text, "Page Up");
		if ( num == VK_NEXT             )  strcpy(text, "Page Down");
		if ( num == VK_END              )  strcpy(text, "End");
		if ( num == VK_HOME             )  strcpy(text, "Home");
		if ( num == VK_SELECT           )  strcpy(text, "Selecionar");
		if ( num == VK_EXECUTE          )  strcpy(text, "Executar");
		if ( num == VK_SNAPSHOT         )  strcpy(text, "Print Scrn");
		if ( num == VK_INSERT           )  strcpy(text, "Insert");
		if ( num == VK_DELETE           )  strcpy(text, "Delete");
		if ( num == VK_HELP             )  strcpy(text, "Ajuda");
		if ( num == VK_LWIN             )  strcpy(text, "Janelas � esquerda");
		if ( num == VK_RWIN             )  strcpy(text, "Janelas � direita");
		if ( num == VK_APPS             )  strcpy(text, "Tecla do aplicativo");
		if ( num == VK_NUMPAD0          )  strcpy(text, "NumPad 0");
		if ( num == VK_NUMPAD1          )  strcpy(text, "NumPad 1");
		if ( num == VK_NUMPAD2          )  strcpy(text, "NumPad 2");
		if ( num == VK_NUMPAD3          )  strcpy(text, "NumPad 3");
		if ( num == VK_NUMPAD4          )  strcpy(text, "NumPad 4");
		if ( num == VK_NUMPAD5          )  strcpy(text, "NumPad 5");
		if ( num == VK_NUMPAD6          )  strcpy(text, "NumPad 6");
		if ( num == VK_NUMPAD7          )  strcpy(text, "NumPad 7");
		if ( num == VK_NUMPAD8          )  strcpy(text, "NumPad 8");
		if ( num == VK_NUMPAD9          )  strcpy(text, "NumPad 9");
		if ( num == VK_MULTIPLY         )  strcpy(text, "NumPad *");
		if ( num == VK_ADD              )  strcpy(text, "NumPad +");
		if ( num == VK_SEPARATOR        )  strcpy(text, "NumPad sep");
		if ( num == VK_SUBTRACT         )  strcpy(text, "NumPad -");
		if ( num == VK_DECIMAL          )  strcpy(text, "NumPad .");
		if ( num == VK_DIVIDE           )  strcpy(text, "NumPad /");
		if ( num == VK_F1               )  strcpy(text, "F1");
		if ( num == VK_F2               )  strcpy(text, "F2");
		if ( num == VK_F3               )  strcpy(text, "F3");
		if ( num == VK_F4               )  strcpy(text, "F4");
		if ( num == VK_F5               )  strcpy(text, "F5");
		if ( num == VK_F6               )  strcpy(text, "F6");
		if ( num == VK_F7               )  strcpy(text, "F7");
		if ( num == VK_F8               )  strcpy(text, "F8");
		if ( num == VK_F9               )  strcpy(text, "F9");
		if ( num == VK_F10              )  strcpy(text, "F10");
		if ( num == VK_F11              )  strcpy(text, "F11");
		if ( num == VK_F12              )  strcpy(text, "F12");
		if ( num == VK_F13              )  strcpy(text, "F13");
		if ( num == VK_F14              )  strcpy(text, "F14");
		if ( num == VK_F15              )  strcpy(text, "F15");
		if ( num == VK_F16              )  strcpy(text, "F16");
		if ( num == VK_F17              )  strcpy(text, "F17");
		if ( num == VK_F18              )  strcpy(text, "F18");
		if ( num == VK_F19              )  strcpy(text, "F19");
		if ( num == VK_F20              )  strcpy(text, "F20");
		if ( num == VK_NUMLOCK          )  strcpy(text, "Num Lock");
		if ( num == VK_SCROLL           )  strcpy(text, "Scroll");
		if ( num == VK_ATTN             )  strcpy(text, "Attn");
		if ( num == VK_CRSEL            )  strcpy(text, "CrSel");
		if ( num == VK_EXSEL            )  strcpy(text, "ExSel");
		if ( num == VK_EREOF            )  strcpy(text, "Apagar EOF");
		if ( num == VK_PLAY             )  strcpy(text, "Reproduzir");
		if ( num == VK_ZOOM             )  strcpy(text, "Zoom");
		if ( num == VK_PA1              )  strcpy(text, "PA1");
		if ( num == VK_OEM_CLEAR        )  strcpy(text, "Limpar");
		if ( num == VK_BUTTON1          )  strcpy(text, "Bot�o 1");
		if ( num == VK_BUTTON2          )  strcpy(text, "Bot�o 2");
		if ( num == VK_BUTTON3          )  strcpy(text, "Bot�o 3");
		if ( num == VK_BUTTON4          )  strcpy(text, "Bot�o 4");
		if ( num == VK_BUTTON5          )  strcpy(text, "Bot�o 5");
		if ( num == VK_BUTTON6          )  strcpy(text, "Bot�o 6");
		if ( num == VK_BUTTON7          )  strcpy(text, "Bot�o 7");
		if ( num == VK_BUTTON8          )  strcpy(text, "Bot�o 8");
		if ( num == VK_BUTTON9          )  strcpy(text, "Bot�o 9");
		if ( num == VK_BUTTON10         )  strcpy(text, "Bot�o 10");
		if ( num == VK_BUTTON11         )  strcpy(text, "Bot�o 11");
		if ( num == VK_BUTTON12         )  strcpy(text, "Bot�o 12");
		if ( num == VK_BUTTON13         )  strcpy(text, "Bot�o 13");
		if ( num == VK_BUTTON14         )  strcpy(text, "Bot�o 14");
		if ( num == VK_BUTTON15         )  strcpy(text, "Bot�o 15");
		if ( num == VK_BUTTON16         )  strcpy(text, "Bot�o 16");
		if ( num == VK_BUTTON17         )  strcpy(text, "Bot�o 17");
		if ( num == VK_BUTTON18         )  strcpy(text, "Bot�o 18");
		if ( num == VK_BUTTON19         )  strcpy(text, "Bot�o 19");
		if ( num == VK_BUTTON20         )  strcpy(text, "Bot�o 20");
		if ( num == VK_BUTTON21         )  strcpy(text, "Bot�o 21");
		if ( num == VK_BUTTON22         )  strcpy(text, "Bot�o 22");
		if ( num == VK_BUTTON23         )  strcpy(text, "Bot�o 23");
		if ( num == VK_BUTTON24         )  strcpy(text, "Bot�o 24");
		if ( num == VK_BUTTON25         )  strcpy(text, "Bot�o 25");
		if ( num == VK_BUTTON26         )  strcpy(text, "Bot�o 26");
		if ( num == VK_BUTTON27         )  strcpy(text, "Bot�o 27");
		if ( num == VK_BUTTON28         )  strcpy(text, "Bot�o 28");
		if ( num == VK_BUTTON29         )  strcpy(text, "Bot�o 29");
		if ( num == VK_BUTTON30         )  strcpy(text, "Bot�o 30");
		if ( num == VK_BUTTON31         )  strcpy(text, "Bot�o 31");
		if ( num == VK_BUTTON32         )  strcpy(text, "Bot�o 32");
		if ( num == VK_WHEELUP          )  strcpy(text, "Roda acima");
		if ( num == VK_WHEELDOWN        )  strcpy(text, "Roda abaixo");
	}
#endif

	return ( text[0] != 0 );
}


