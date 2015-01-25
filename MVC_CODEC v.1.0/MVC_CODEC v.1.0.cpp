// Multiview Video CODEC for encoding and decoding views (For 2 views, left and right)

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
using namespace std;

/* Defining COnstants */
#define GOP 5
#define WIDTH 480
#define HEIGHT 272
#define BLOCK_W 8
#define BLOCK_H 8
#define BLOCK_SIZE 8
#define D 8
#define SEARCH_WIN (2*D+1)*(2*D+1)
int FRAMES;
#include "frame_prediction.h"
#include "DCT_IDCT.h"

/*
	File Descripters
*/
FILE *fr;
FILE *fw;
FILE *fn;
int fileSize;

/*
	This structure is used for storing the Symbols Data
	This will contain all the symbols with their codes along with their length
*/
struct CodeSet{
	unsigned char symbol;
	int CodeLength;
	bool *code;
};

/*
	symbols is a pointer to CodeSet (structure) taht will contain all the symbols and their info
*/
struct CodeSet *symbolsInfo;
bool *myCodeSet;
int countBits;
int noOfSymbols;

/*
	This structure is used for creating nodes each for unique symbol.
	Also for storing the amoount of the occurence in the video file
	Also this structure is used for making tree
*/
struct Node{
	int symbol;
	int occurAmount;
	float probability;
	struct Node* downNode;
	struct Node* leftNode;
	struct Node* rightNode;
	struct Node* nextNode;
};

/*
	Some variables used for managing data structres like linkList and tree
*/
struct Node *headNode;
struct Node *currentNode;
struct Node *lastNode;
struct Node *startingNode;

/*
	This function initializes the LinkList in which i will
	store the usefull information needed to make a tree
*/
int initLinkList(){
	headNode = (struct Node*)malloc(sizeof(struct Node));
	headNode->symbol = 0;
	headNode->occurAmount = 0;
	headNode->nextNode = NULL;
	headNode->probability = 1;
	countBits = 0;
	return 0;
}

/*
	This function adds a new Node to the existing LinkList
*/
int addNode(int symbol){
	
	for(currentNode=headNode; currentNode->nextNode != NULL; currentNode = currentNode->nextNode){
		
	}
	struct Node *newNode;
	newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->symbol = symbol;
	newNode->occurAmount = 1;
	currentNode->nextNode = newNode;
	newNode->nextNode = NULL;
	newNode->downNode = NULL;
	newNode->leftNode = NULL;
	newNode->rightNode = NULL;
	return 0;
}


/*
	Function need to delete a Node
*/
int deleteNode(int symbol){

	return 0;
}

/*
	This function searches the current symbol
	if it is present in the existing LinkList
	than it just increment the accurence of that symbol
	else it will add a new Node by calling the function addNode()
*/
int searchNode(unsigned char symbol){
	for(currentNode=headNode->nextNode; ; currentNode = currentNode->nextNode){
		
		if (currentNode == NULL){
			addNode(symbol);
			return 0;
		}

		if(currentNode->symbol == symbol){
			currentNode->occurAmount++;
			return 1;
		}

		if(currentNode->nextNode == NULL){
			addNode(symbol);
			return 0;
		}
	}

	return 0;
}

/*
	This function traverses through the LinkList
	and displays all the Data present in the LinkList
*/
int displayData(){
	int i = 1;
	float total = 0;
	for(currentNode=headNode->nextNode; ; currentNode = currentNode->nextNode){
		cout<<i<<": "<<(int)currentNode->symbol<<" "<<currentNode->occurAmount<<"        Probability: "<<(float)(currentNode->occurAmount/(1.00*fileSize))<<endl;
		currentNode->probability = (float)(currentNode->occurAmount/(1.00*fileSize));
		total = total + (float)(currentNode->occurAmount/(1.00*fileSize));
		if(currentNode->nextNode == NULL)
			break;
		i++;
	}
	cout<<endl<<"Total: "<<total<<endl;
	return 0;
}

/*
	This function counts the total number
	of symbols that are present in the LinkList
*/
int countSymbols(){
	int numOfSymbols = 0;
	for(currentNode=headNode->nextNode; ; currentNode = currentNode->nextNode){
		numOfSymbols++;
		if(currentNode->nextNode == NULL)
			break;
	}
	return numOfSymbols;
}

/*
	This function sort the LinkList
	based on the probability of each symbol
*/
void sort(){
	
	struct Node *swap;
	swap = (struct Node*)malloc(sizeof(struct Node));
	startingNode = headNode->nextNode;
	currentNode = headNode->nextNode;
	while(1){
		if (startingNode->nextNode == NULL) {
			break;
		}
		currentNode = headNode->nextNode;
		while(1){

			if(currentNode->nextNode == NULL) {
				break;
			}

			if(currentNode->occurAmount > (currentNode->nextNode)->occurAmount){
				swap->occurAmount = currentNode->occurAmount;
				swap->probability = currentNode->probability;
				swap->symbol = currentNode->symbol;
				currentNode->occurAmount = (currentNode->nextNode)->occurAmount;
				currentNode->probability = (currentNode->nextNode)->probability;
				currentNode->symbol = (currentNode->nextNode)->symbol;
				(currentNode->nextNode)->occurAmount = swap->occurAmount;
				(currentNode->nextNode)->probability = swap->probability;
				(currentNode->nextNode)->symbol = swap->symbol;
			}
			currentNode = currentNode->nextNode;
		}
		startingNode = startingNode->nextNode;
	}

}

/*
	This function is used in builing tree
	and the purpose of this function is to
	find the symbol Node with minimum 
	probability
*/
struct Node * findMinProbabilityNew(struct Node *skipThatNode){
	struct Node *minProbabilityNode;
	float minProbability = 1;
	minProbabilityNode = headNode;
	currentNode = headNode->nextNode;
	struct Node *dummyNode;
	while(1) {
		
		if (currentNode == NULL) {
			break;
		}
		dummyNode = currentNode;
		while (1) {
			
			if (dummyNode->downNode == NULL) {
				if (minProbability > dummyNode->probability) {
					if (dummyNode == skipThatNode){
						break;
					}
					minProbability = dummyNode->probability;
					minProbabilityNode = dummyNode;
					break;
				}
				break;
			}
			dummyNode = dummyNode->downNode;
		}
		currentNode = currentNode->nextNode;
	}
	if (minProbabilityNode == headNode){
		minProbabilityNode = NULL;
	}
	return minProbabilityNode;
}

/*
	This function will make a tree according to
	the Huffman Coding Algorithm
*/
int makeTree(){

	struct Node *firstMinNode;
	struct Node *secondMinNode;
	struct Node *newNode;
	while(1){

		firstMinNode = findMinProbabilityNew(NULL);
		secondMinNode = findMinProbabilityNew(firstMinNode);

		if (secondMinNode == NULL){
			lastNode = firstMinNode;
			cout<<endl<<endl<<"Tree Constructed!!!"<<endl;
			break;
		}

		newNode = (struct Node*)malloc(sizeof(struct Node));
		newNode->leftNode = firstMinNode;
		newNode->rightNode = secondMinNode;
		newNode->probability = firstMinNode->probability + secondMinNode->probability;
		cout<<endl<<"Sum: "<<newNode->probability;
		firstMinNode->downNode = newNode;
		secondMinNode->downNode = newNode;
		newNode->downNode = NULL;
	}
		
	return 0;
}

/*
	This function traverses through the huffman tree
	And makes the variable length Codes each for the
	unique symbol
*/
void makeCodes(){
	int count = 0;
	int temporaryIndex = 0;
	int tempInt = 0;
	bool tempArrayInt[50];
	struct Node *dummyNode;
	currentNode = headNode->nextNode;
	while(1){
		
		if (currentNode == NULL){
			break;
		}
		startingNode = currentNode;
		
		while(1){
			
			if(startingNode->downNode == NULL){
				break;
			}
			dummyNode = startingNode;
			startingNode = startingNode->downNode;
			
			if (startingNode->leftNode == dummyNode){
				tempArrayInt[count] = 0;
				count++;
			}
			if (startingNode->rightNode == dummyNode){
				tempArrayInt[count] = 1;
				count++;
			}
		}

		symbolsInfo[tempInt].symbol = (unsigned char)currentNode->symbol;
		symbolsInfo[tempInt].CodeLength = count;
		symbolsInfo[tempInt].code = (bool *)malloc(sizeof(bool)*count);
		temporaryIndex = 0;
		
		for(int i = (count-1); i >= 0; i--){
			symbolsInfo[tempInt].code[temporaryIndex] = tempArrayInt[i];
			temporaryIndex++;
		}

		countBits = countBits + (count*currentNode->occurAmount);
		count = 0;
		currentNode = currentNode->nextNode;
		tempInt++;
	}
}

/*
	this function is used for displaying all the 
	data present in the array named as symbolsInfo
*/
void displaySymbolsWithCode(){
	cout<<endl<<"Symbols With Codes"<<endl;
	for (int i = 0; i < countSymbols(); i++) {
		cout<<"Symbol: "<<(int)symbolsInfo[i].symbol<<" Code: ";
		for (int j = 0; j < symbolsInfo[i].CodeLength; j++) {
			cout<<symbolsInfo[i].code[j];
		}
		cout<<endl;
	}
}

/*
	This function is used to search for the index of the given symbol
*/
int findIndexOfThatSymbol(unsigned char symbol){
	for (int i = 0; i < countSymbols(); i++) {
		if (symbol == symbolsInfo[i].symbol){
			return i;
		}
	}
	return 0;
}

/*
	This function is used in decoding side
	the purpose of the method is to find the
	symbol of the given code
*/
int findSymbol(bool *code, int lengthOfCode){
	bool isEqual = 0;
	for(int i=noOfSymbols; i>=0; i--){
		if (symbolsInfo[i].CodeLength == lengthOfCode){
			//Here find the perfect Match for the bit pattern
			isEqual = true;
			for (int j=0; j<lengthOfCode; j++) {
				if(symbolsInfo[i].code[j] != code[j]){
					isEqual = false;
				}
			}
			if (isEqual) {
				return (int)symbolsInfo[i].symbol;
			} 
		}
	}
	return 11111; // No Match found, it'll return 11111
}

/*
	Making hashTable for boosting up the speed of the program
	and make the search faster
*/
int hashTable[256];
int makeHashTable(){
	int ti = 0;
	for (int i = 0; i < countSymbols(); i++) {
		ti = symbolsInfo[i].symbol;
		hashTable[ti] = i;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	#pragma warning (disable : 4996);
	FRAMES = atoi(argv[4]);

	if (!strcmp("encode", argv[1])){
		finding_motion_vectors(argv[2], argv[3]);
		apply_DCT();
		
		FILE *v = fopen("raw_2.yuv", "rb");
		fseek (v, 0, SEEK_END);   // getting file Size
		int totalLength = ftell (v);
		fclose (v);
		fileSize = totalLength;
		int index = 0;
		char nameOfVideo[] = "raw_2.yuv";

		fr = fopen(nameOfVideo,"rb");
		if(fr==NULL){
			printf("Failed to open");
			getch();
			return 0;
		}
		
		fw = fopen("EncodedVideo.yuv","wb");
		if(fw==NULL){
			printf("Failed to open");
			getch();
			return 0;
		}
	
		unsigned char *cur_frame = (unsigned char*)malloc(totalLength*sizeof(unsigned char));
		fread(cur_frame,totalLength,sizeof(unsigned char),fr);
		initLinkList();
		
		for (int i=0; i<totalLength; i++){
			searchNode((int)cur_frame[i]);
		}

		sort();
		displayData();

		symbolsInfo = (struct CodeSet*)malloc(sizeof(struct CodeSet)*countSymbols());
		noOfSymbols = countSymbols();
		displayData();
		makeTree();
		makeCodes();
		displaySymbolsWithCode();
		cout<<"Passing 1"<<endl;
		makeHashTable();
		cout<<endl<<"Starting Encodeing---->";
	
		bool *encoded_frame = (bool *) malloc(sizeof(bool)*countBits);
		bool *en_b = (bool *) malloc(sizeof(bool)*countBits);
		cout<<"Passing 1"<<endl;
		int symbolCounter = 0;
		bool *dummyBitArray = (bool *) malloc(sizeof(bool)*50);
		int symbolIndex = 0;
		int encodedIndex = 0;

		while(symbolCounter < totalLength){
			symbolIndex = hashTable[cur_frame[symbolCounter]];
			for (int i=0; i<symbolsInfo[symbolIndex].CodeLength; i++){
				encoded_frame[encodedIndex] = symbolsInfo[symbolIndex].code[i];
				encodedIndex++;
			}
			symbolCounter++;
		}
		

		cout<<endl<<"Saving File";
		int en_p = 0;
		int en_index = 0;
		unsigned char *en = (unsigned char*)malloc((countBits/8)*sizeof(unsigned char));
		int justforcheckingnum = 0; // make sure this is only for checking the code, Plz Work
		
		for(int i=0; i<(countBits/8); i++){
			en[i] = 0;
			for(int j=7; j>=0; j--){
				en_p = pow(2, j);
				if (encoded_frame[en_index] == 1){
					en[i] = en[i] | en_p;
				}
				en_index++;
			}
			justforcheckingnum = i;
		}

		FILE *info = fopen("info.yuv", "wb");
		unsigned char numberOfSymbols = countSymbols();
		unsigned char *temp = (unsigned char *)malloc(sizeof(unsigned char));
		temp[0] = numberOfSymbols;
		fwrite(temp,1,sizeof(unsigned char),info);
		
		for(int i=0; i<numberOfSymbols; i++){
			temp[0] = symbolsInfo[i].symbol;
			cout<<(int)temp[0]<<" ";
			fwrite(temp,1,sizeof(unsigned char), info);
			temp[0] = symbolsInfo[i].CodeLength;
			cout<<(int)temp[0]<<" ";
			fwrite(temp,1,sizeof(unsigned char),info);
			for (int j=0; j<symbolsInfo[i].CodeLength; j++){
				if (symbolsInfo[i].code[j] == true){
					temp[0] = 1;
					cout<<(int)temp[0]<<" ";
					fwrite(temp,1,sizeof(unsigned char),info);
				} else {
					temp[0] = 0;
					cout<<(int)temp[0]<<" ";
					fwrite(temp,1,sizeof(unsigned char),info);
				}
			}
			cout<<endl;
		}
		int *temp1 = (int *)malloc(sizeof(int));
		temp1[0] = countBits;
		fwrite(temp1, 1, sizeof(int), fw);
		temp1[0] = totalLength;
		fwrite(temp1, 1, sizeof(int), fw);
		fwrite(en, ceil(countBits/8),sizeof(unsigned char),fw);
		fclose(fw);
		FILE *just = fopen("EncodedVideo.yuv", "rb");
		fseek (just, 0, SEEK_END);   // non-portable
		int fileSize=ftell (just);
		fclose (just);
		just = fopen("EncodedVideo.yuv", "rb");
		fread(temp1, 1, sizeof(int), just);
		cout<<endl<<"Encoding Complete:"<<endl<<"File Name - EncodedPic.yuv"<<endl<<"File Size: "<<((countBits/8)/1024)/1024<<" MB"<<endl;

		fclose(info);
		fclose(fr);
		free(en);
		free(en_b);
		free(cur_frame);

	} else if (!strcmp("decode", argv[1])){
		
		fr = fopen("info.yuv", "rb");
		cout<<endl<<"Starting Decoding Pic";
		int en_p = 0;
		int en_index = 0;
		bool *dummyBitArray = (bool *) malloc(sizeof(bool)*50);
		unsigned char *numberOfSymbols = (unsigned char*)malloc(sizeof(unsigned char));
		unsigned char *temp = (unsigned char*)malloc(sizeof(unsigned char));
		fread(numberOfSymbols,1,sizeof(unsigned char),fr);
		symbolsInfo = (struct CodeSet*)malloc(numberOfSymbols[0]*sizeof(struct CodeSet));
		noOfSymbols = numberOfSymbols[0];
	
		for(int i=0; i<numberOfSymbols[0]; i++){
			fread(temp,1,sizeof(unsigned char),fr);
			symbolsInfo[i].symbol = temp[0];
			cout<<(int)temp[0]<<" ";
			fread(temp,1,sizeof(unsigned char),fr);
			symbolsInfo[i].CodeLength = temp[0];
			cout<<(int)temp[0]<<" ";
			symbolsInfo[i].code = (bool *)malloc(symbolsInfo[i].CodeLength*sizeof(bool));
			for (int j=0; j<symbolsInfo[i].CodeLength; j++){
				fread(temp,1,sizeof(unsigned char),fr);
				cout<<(int)temp[0]<<" ";
				if (temp[0] == 1){
					symbolsInfo[i].code[j] = 1;
				} else {
					symbolsInfo[i].code[j] = 0;
				}
			}
			cout<<endl;
		}
		fclose(fr);
		fw = fopen("EncodedVideo.yuv", "rb");

		fseek (fw, 0, SEEK_END);   // getting file Size
		int fileSize=ftell (fw);
		fclose (fw);
		fr = fopen("EncodedVideo.yuv", "rb");
		
		if (fr == NULL){
			cout<<"File Not Found that is EncodedVideo.yuv"<<endl;
		}
		
		int *temp1 = (int *)malloc(sizeof(int));
		fread(temp1, 1, sizeof(int), fr);
		countBits = temp1[0];
		fread(temp1, 1, sizeof(int), fr);
		int totalLength = temp1[0];
		cout<<"This is the count from the file"<<(int)temp1[0]<<endl;
		bool *en_b = (bool *) malloc(sizeof(bool)*countBits);
		unsigned char *en = (unsigned char*)malloc((countBits/8)*sizeof(unsigned char));
		fread(en, sizeof(unsigned char), countBits/8, fr);
		
		for(int i=0; i<(countBits/8); i++){
			for(int j=7; j>=0; j--){
				en_p = pow(2, j);
				en_b[en_index] = (en[i] & en_p)? true : false;
				en_index++;
			}
		}

		cout<<"Count Bit: "<<countBits<<endl;
		unsigned char *decoded_frame = (unsigned char *) malloc(sizeof(unsigned char)*totalLength);
		int raw_counter = 0;
		int bitCounter = 0;
		int check = 0;
		int decoded_counter = 0;
		while(raw_counter < countBits){
			dummyBitArray[bitCounter] = en_b[raw_counter];
			bitCounter++;
			check = findSymbol(dummyBitArray, bitCounter);
			if (check != 11111) {
				decoded_frame[decoded_counter] = (unsigned char)check;
				decoded_counter++;
				bitCounter = 0;
			}
			raw_counter++;
		}
		cout<<"Check Post"<<endl;
		FILE *fTes = fopen("raw_2.yuv","wb");
		if(fTes==NULL){
			printf("Failed to open");
			getch();
			return 0;
		}
	
		cout<<"Check Post"<<endl;
		fwrite(decoded_frame,totalLength,sizeof(unsigned char),fTes);
		cout<<"Check Post"<<endl;
		free(decoded_frame);
		fclose(fTes);
		fclose(fr);
		free(en);
		free(en_b);
		cout<<"Check Post Before IDCT"<<endl;
		apply_IDCT();
		reconstruct_frames();
		cout<<"Check Post After IDCT"<<endl;
	}
	getch();
	return 0;
}


