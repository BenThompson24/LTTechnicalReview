/*******************************************************************************
filename photos.c
	author Ben Thompson
   email benjitech0@gmail.com

Brief Description:
	This program serves as a test of techincal skills for Lean Techniques to
	evaluate my programming skills for an internship. The program takes a user
	specified album id and prints out the photo id and title of all photos
	associated with that album.
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ALBUM_SIZE 3
#define MAX_PHOTO_ID_SIZE 4
#define MAX_PHOTO_DATA_LINE_LENGTH 200
#define MAX_TITLE_LENGTH 200
#define LAST_ALBUM 100
#define COMMA 44
#define QUOTE 34
#define SPACE 32

/*******************************************************************************
	 Function: GetAlbum

Description: Asks user for specific album

		 Inputs: None

		Outputs: The user specified album
*******************************************************************************/
int GetAlbum(void)
{
	/* User specified album */
	int album = 0;
	
	/* Ask user for an album */
	printf("Please enter the album you would like to retrieve: \n");
	scanf("%d", &album);
	
	/* Clear the buffer in case album is not valid */
	while ((getchar()) != '\n');
	
	/* Check validity of album */
	if(album == 0 || album > LAST_ALBUM)
		printf("Invalid Input\n");

	/* Return the album */
	return album;
}

/*******************************************************************************
	 Function: PrintId

Description: Reads in a photo id and prints it to the command line

		 Inputs: fp - a pointer to a file pointer which points to the file to be 
									read.

		Outputs: Void Function
*******************************************************************************/
void PrintId(FILE **fp)
{
	/* Buffer to hold photo data for photo id */
	char buff[MAX_PHOTO_DATA_LINE_LENGTH] = {0};
	
	/* Buffer to hold the specific photo id */
	char idArray[MAX_PHOTO_ID_SIZE + 1] = {0};
	
	/* Pointer to navigate the information in the photo data buffer */
	char *linePos = buff;
	
	/* Index of idArray */
	int i = 0;
	
	/* The photo id */
	int photoId = 0;
	
	/* If we are able to read in a line from the file */
	if(fgets(buff, MAX_PHOTO_DATA_LINE_LENGTH, *fp))
	{
		/* Move 10 bytes within the buffer to reach the start of the photo id */
		linePos += 10;
	
		/* While there are more numbers in the photo id */
		while(*linePos != COMMA && i < MAX_PHOTO_ID_SIZE + 1)
		{
			/* Set the current index of idArray to the current character in the buffer */
			idArray[i] = *linePos;
			
			/* Move the pointer to the next character in the buffer */
			linePos++;
			
			/* Increase the index of idArray */
			i++;
		}
		
		/* Convert the string stored in idArray to an integer */
		photoId = atoi(idArray);
	
		/* Print the photo Id */
		printf("[%d] ", photoId);
	}
}

/*******************************************************************************
	 Function: PrintTitle

Description: Reads in a photo title and prints it to the command line

		 Inputs: fp - a pointer to a file pointer which points to the file to be 
									read.

		Outputs: Void Function
*******************************************************************************/
void PrintTitle(FILE **fp)
{
	/* Buffer to hold the line from photodata that contains the title */
	char buff[MAX_PHOTO_DATA_LINE_LENGTH] = {0};
	
	/* Pointer to navigate the buffer in search for the title */
	char *linePos = buff;
	
	/* Attempt to read the line with the title into the buffer */
	if(fgets(buff, MAX_PHOTO_DATA_LINE_LENGTH, *fp))
	{
		/* Array to hold the specific title */
		char title[MAX_TITLE_LENGTH + 1] = {0};
		
		/* Index of title array */
		int i = 0;
		
		/* Move 14 bytes within the buffer to reach the start of the title */
		linePos += 14;
	
		/* While their are more characters in the title */
		while(*linePos != QUOTE && i < MAX_TITLE_LENGTH + 1)
		{
			/* Set the current index of the title array to the value in the buffer */
			title[i] = *linePos;
			
			/* Move to the next character in the buffer */
			linePos++;
			
			/* Increase the index of the title array */
			i++;
		}
		
		/* Print the title after the photo id */
		printf("%s\n", title);
	}
}

/*******************************************************************************
	 Function: FindAlbum

Description: Takes a user specified album number and searches photodata for
						 the specific album data, and then prints all photo's photo id and
						 title if the album exists.

		 Inputs: selectedAlbum - the user specified album number to look for

		Outputs: Void Function
*******************************************************************************/
void FindAlbum(int selectedAlbum)
{
	/* File pointer */
	FILE * fp = fopen("photodata.txt", "rt");
	
	/* Check if the file is opened */
	if(fp)
	{
		/* The current albumId being looked at in the file */
		int iAlbumId = 0;
		
		/* 
			While there are still more photos in the file, and the current id being 
			 evaluated isn't greater than the one being looked for 
		*/
		while(!feof(fp) && iAlbumId <= selectedAlbum)
		{
			/* Buffer to hold the line with the photo id read in from the file */
			char buff[MAX_PHOTO_DATA_LINE_LENGTH];
			
			/* Pointer to navigate the string read into the buffer */
			char * linePos = buff;
			
			/* Read a line into the buffer */
			if(fgets(buff, MAX_PHOTO_DATA_LINE_LENGTH, fp))
			{
				/* Walk the line until hitting a non-space character */
				while(*linePos == SPACE)
					linePos++;
				
				/* If the non-space character is a " then, procede and check if it a quote */
				if(*linePos == QUOTE)
				{
					/* Move to the next character to get to the tag of the current data */
					linePos++;
					
					/* Verify that we are looking at the albumId tag */
					if(memcmp(linePos, "albumId", 7) == 0)
					{
						/* Character Array to store the specific albumId */
						char albumId[MAX_ALBUM_SIZE + 1] = {0};
						
						/* Index of the albumId array */
						int i = 0;
						
						/* Move 10 bytes to the first character of the albumId in the buffer */
						linePos += 10;
						
						/* While their are more characters in the albumId */
						while(*linePos != COMMA && i < MAX_ALBUM_SIZE + 1)
						{
							/* Place character in array */
							albumId[i] = *linePos;
							
							/* Move to next index */
							i++;
							
							/* Move to next character */
							linePos++;
						}
						
						/* Convert the string to an integer */
						iAlbumId = atoi(albumId);
						
						/* Check if the id matches the user inputed id */
						if(iAlbumId == selectedAlbum)
						{	
							/* Print the photo id of the current item */
							PrintId(&fp);
							
							/* Print the title of the current item */
							PrintTitle(&fp);
						}
						
					}	
				}
			}
		}
		
		/* Close the opened file */
		fclose(fp);
	}
	else
		/* If unable to open file, print an error message */
		printf("Could not open photodata.txt");
}

/*******************************************************************************
	 Function: main

Description: Starts the program, and gets a user specified album number, then
						 prints any photo id and title of photos within that album

		 Inputs: None

		Outputs: Returns an integer to the operating system to notify that it was
						 successful
*******************************************************************************/
int main(void)
{
	/* Return value of GetAlbum, which is the album to be selected */
	int selectedAlbum = 0;
	
	/* If the selected album is not valid, ask for another album */
	while(selectedAlbum == 0 || selectedAlbum > LAST_ALBUM)
		selectedAlbum = GetAlbum();
	
	/* Print the information from the selected album */
	FindAlbum(selectedAlbum);

	/* Return to the operating system */
	return 0;
}