// Inline.cpp
//
// This file contains routines where functionality is inlined in assembly language
//
#include <windows.h>
#include <stdio.h>


char *gWordList[] = {

	"absorbed",
	"abstracted",
	"advertisement",
	"ants",
	"apparatus",
	"appear",
	"arm",
	"bird",
	"blue",
	"boundless",
	"broad",
	"cause",
	"ceaseless",
	"complete",
	"confuse",
	"cooperative",
	"cover",
	"credit",
	"devilish",
	"dirty",
	"discreet",		// #20, 20 * 4 = 80 == (0x50), 21st word in list, #20
	"ear",
	"eatable",
	"experience",
	"fix",
	"flower",
	"friend",
	"furtive",
	"harm",
	"harmony",
	"heady",
	"heap",
	"ignore",
	"infamous",
	"jittery",
	"language",
	"learn",
	"line",
	"live",
	"maid",
	"melted",
	"memory",
	"nasty",
	"neck",
	"noise",
	"orange",
	"peaceful",
	"pine",
	"piquant",
	"pollution",
	"precede",
	"profit",
	"quiver",
	"quizzical",
	"request",
	"rustic",
	"satisfying",
	"scatter",
	"science",
	"second-hand",
	"shade",
	"sharp",
	"shivering",
	"show",
	"solid",
	"sore",
	"squealing",
	"start",
	"system",
	"terrible",
	"test",
	"throne",
	"tooth",
	"womanly",
	"xylophone",
	"zebra"
};

// searches the gWordList[] for the word passed in
// do a binary search and count the number of checks
// return -1 if the word is not found
int inlineBinarySearch(char *searchWord, int *numSteps)
{
	int elementNumber = -1;

	*numSteps = 0;

	__asm {

		//mov elementNumber, 4	// puts a 4 in the return value
		//mov edi,numSteps		// how to access numSteps
		//mov [edi],25

		//xor eax,eax
		//lea edi,gWordList		// this gets the base address of array of character pointers
		//mov esi,[edi]			// address of first word in esi
		//mov al,[esi]			// first letter of first word
		//mov elementNumber,eax	// return 97 which is character 'a'

		//add edi,0x50			// get to address of 21st word = "discreet"  (word #20 in list, 20 * 4 = 80)
		//mov esi,[edi]			// get address where 21st word is stored in memory
		//mov al,[esi+1]			// put 2nd character from "discreet" which is an 'i' = 0x69 (105)
		//mov elementNumber,eax
		//mov edx, [edi + ecx * 4]	 //edx == first
		//mov ebx, last	//ebx == last
		xor ecx, ecx
		lea edi, gWordList					//edi == gWordList
		mov edx, [edi]			//edx == first word in gWordList

		mov ecx, 74							//ecx == array length -1
		mov ebx, [edi + ecx * 4]			//ebx == last word in gWordList
		xor ecx, ecx
		mov eax, 74
		shl eax, 2
	BeginL :
			cmp edx, ebx	//cmp edx and ebx
			ja EndBad

			//mov eax, ebx	//eax == first
			//add eax, ebx	//eax == first + last
			//shr eax, 1		// eax == (first + last) >> 1
			shr eax, 1

			mov esi, [edi + eax]		//esi == sortedArray[mid]
			//cmp searchWord, esi				//cmp key and esi

			jmp DopeCompare						//compare our two current strings

		Return :
			cmp eax, 0
			popad

			je Equal						//_if(key == edx) jmpto Equal
			jl LessThan						//if(key < edx) jmpto LessThan

			mov edx, esi					//first = mid
			add edx, 4						//first = mid + 1
			jmp BeginL						//loop

		LessThan :
			mov ebx, esi					//last = mid
			sub ebx, 4						//last = mid - 1
			jmp BeginL						//loop

		EndBad :
			mov elementNumber, -1					//value not found
			jmp EndL

		DopeCompare :
			pushad
			xor ecx, ecx

		dankLOOP :
			mov eax, 0x0
			mov dl, byte ptr [esi + ecx]
			mov bl, byte ptr [searchWord + ecx]
			inc ecx 
			
			cmp dl, bl
			mov eax, -1
			jl Return
			mov eax, 1
			jg Return
			mov eax, 0x0
			test dl, dl
			je Return


			jmp dankLOOP

			
			

			
			
			
			

		Equal :
			mov elementNumber, eax					//value found, retval = eax, end
		EndL :
	}

	return elementNumber;
} // inlineBinarySearch


void printBytes(char *data, int length)
{
	int x;

	for(x = 0; x < length; x++)
	{
		if( (x & 0xF) == 0) printf("\n");
		printf("%02X ", (unsigned char) data[x]);
	}
	printf("\n\n");

	return;
} // printBytes


void callInLineFunctions()
{
	int x, tmpi;
	char word[64];

//*	Start Binary Search Test Cases

	// get the length of the word list
	int gListLength = sizeof(gWordList) / sizeof( char *);		// get size of word list

// Test Before/After the list
	strcpy(word, "aaaaaa");
	tmpi = inlineBinarySearch(word, &x);
	if(tmpi == -1)
		printf("The word \"%s\" not found! Steps taken=%d\n\n", word, x);
	else
		printf("Element #%3d  Steps: %2d - The word \"%s\" was found.\n\n", tmpi, x, word);

	strcpy(word, "zzzzzz");
	tmpi = inlineBinarySearch(word, &x);
	if(tmpi == -1)
		printf("The word \"%s\" not found! Steps taken=%d\n\n", word, x);
	else
		printf("Element #%3d  Steps: %2d - The word \"%s\" was found.\n\n", tmpi, x, word);

	strcpy(word, "bluebird");
	tmpi = inlineBinarySearch(word, &x);
	if(tmpi == -1)
		printf("The word \"%s\" not found! Steps taken=%d\n\n", word, x);
	else
		printf("Element #%3d  Steps: %2d - The word \"%s\" was found.\n\n", tmpi, x, word);

	strcpy(word, "project");
	tmpi = inlineBinarySearch(word, &x);
	if(tmpi == -1)
		printf("The word \"%s\" not found! Steps taken=%d\n\n", word, x);
	else
		printf("Element #%3d  Steps: %2d - The word \"%s\" was found.\n\n", tmpi, x, word);

	strcpy(word, "black");
	tmpi = inlineBinarySearch(word, &x);
	if(tmpi == -1)
		printf("The word \"%s\" not found! Steps taken=%d\n\n", word, x);
	else
		printf("Element #%3d  Steps: %2d - The word \"%s\" was found.\n\n", tmpi, x, word);

// Check for words not on the list, but would be in the middle

// Check the entire list to make sure we can find any word
	for(int z = 0; z < gListLength; z++)
	{
		strcpy(word, gWordList[z]);
		tmpi = inlineBinarySearch(word, &x);
		if(tmpi == -1)
			printf("The word \"%s\" not found! Steps taken=%d\n\n", word, x);
		else
			printf("Element #%3d  Steps: %2d - The word \"%s\" was found.\n\n", tmpi, x, word);
	}

//*/	End Binary Search

	exit(0);
} // callInLineFunctions

void main(int argc, char *argv[]){
	callInLineFunctions();
}
