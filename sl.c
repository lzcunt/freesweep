#include <stdio.h>
#include <stdlib.h>
#include "sweep.h"

void SaveGame(GameStats* Game, char *fname)
{
	FILE *fo = NULL;

	fo = fopen(fname, "w");
	if (fo == NULL)
	{
		SweepError("Could not open save game file!");

		/* XXX fix up the exit to be nice */
		exit(EXIT_FAILURE);
	}
	
	/* dump the stats out */
	fprintf(fo, "%d\n%d\n%d\n%u\n%u\n%u\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n"
			"%u\n%u\n%u\n",
			Game->Height, Game->Width, Game->Percent, Game->NumMines, 
			Game->MarkedMines, Game->BadMarkedMines, Game->Color, 
			Game->Fast, Game->Alert, 
			Game->LineDraw, Game->CursorX, Game->CursorY,
			Game->LargeBoardX, Game->LargeBoardY, Game->Status,
			Game->FocusX, Game->FocusY, Game->Time);
	
	/* dump the field out */
	fwrite(Game->Field, 
		(Game->Height*((Game->Width % 2 ? (Game->Width) +1 : Game->Width ))/2)
		* sizeof(unsigned char), 1, fo);

	fclose(fo);
}

GameStats* LoadGame(char *fname)
{
	FILE *fi = NULL;
	GameStats *Game = NULL;
	int VViewable = 0, HViewable = 0;

	VViewable=(LINES-6);
	HViewable=((COLS-INFO_W-2)/3);

	/* get space for the new board */
	if ((Game=malloc(sizeof(GameStats)))==NULL)
	{
		SweepError("Out of Memory.");
		/* XXX clean this up */
		exit(EXIT_FAILURE);
	}

	/* open the file for reading */
	fi = fopen(fname, "r");
	if (fi == NULL)
	{
		SweepError("Could not find file to load");
		/* XXX clean this up */
		exit(EXIT_FAILURE);
	}

	/* Load the Game Stats */
	fscanf(fi, "%d\n%d\n%d\n%u\n%u\n%u\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n"
			"%u\n%u\n%u\n",
			&Game->Height, &Game->Width, &Game->Percent, &Game->NumMines, 
			&Game->MarkedMines, &Game->BadMarkedMines, &Game->Color, 
			&Game->Fast, &Game->Alert, 
			&Game->LineDraw, &Game->CursorX, &Game->CursorY,
			&Game->LargeBoardX, &Game->LargeBoardY, &Game->Status,
			&Game->FocusX, &Game->FocusY, &Game->Time);

	/* make the field I need to write into */
	if ((Game->Field=calloc((Game->Height*(
		( Game->Width % 2 ? (Game->Width) +1 : Game->Width )))/2,
		sizeof(char)))==NULL)
	{
		SweepError("Out of Memory.");
		/* XXX clean this up */
		exit(EXIT_FAILURE);
	}

	/* load the board */
	fread(Game->Field, 
		(Game->Height*((Game->Width % 2 ? (Game->Width) +1 : Game->Width ))/2)
		* sizeof(unsigned char), 1, fi);

	/* make the new window setup for it */
	if (Game->LargeBoardX && Game->LargeBoardY)
	{
		Game->Border=newwin((LINES-4),(COLS-INFO_W),0,0);
/*		Game->Board=newwin(VViewable,(3*HViewable),1,1);*/
		Game->Board=derwin(Game->Border,VViewable,(3*HViewable),1,1);
	}
	else if (Game->LargeBoardX)
	{
		Game->Border=newwin((Game->Height+2),(COLS-INFO_W),0,0);
/*		Game->Board=newwin(Game->Height,(3*HViewable),1,1);*/
		Game->Board=derwin(Game->Border,Game->Height,(3*HViewable),1,1);
	}
	else if (Game->LargeBoardY)
	{
		Game->Border=newwin((LINES-4),((3*Game->Width)+2),0,0);
/*		Game->Board=newwin(VViewable,(3*Game->Width),1,1);*/
		Game->Board=derwin(Game->Border,VViewable,(3*Game->Width),1,1);
	}
	else
	{
		Game->Border=newwin((Game->Height+2),((3*Game->Width)+2),0,0);
/*		Game->Board=newwin(Game->Height,(3*Game->Width),1,1);*/
		Game->Board=derwin(Game->Border,Game->Height,(3*Game->Width),1,1);
	}

	if ((Game->Border==NULL)||(Game->Board==NULL))
	{
		perror("ReadyGame::AllocWin");
		exit(EXIT_FAILURE);
	}

	return Game;
}




