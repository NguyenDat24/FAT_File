#ifndef __APP_H__
#define __APP_H__

/*Include Library*/
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include <string.h>

/* @brief Struct to save address of entry */
struct Node
{
	uint32_t Data;
	struct Node *Next;
};
typedef struct Node nodeType;

/* @brief Enum to save status of entry */
enum Status
{
	None,
	Folder,
	File
};

/* @brief struct to save status of entry, address, numMainEntry */
typedef struct {
    enum Status Flag;
    uint32_t Address;
    uint8_t numMainEntry;
} statusType;

/**********************************************************	FUNCTION PROTOTYPE	**********************************************************/

/*
@brief				This funtion to add one node to linked list
@param[in] 			Option: Option of user
					curAddress: Address of the entry to add to the node of linked list
@param[out] 		void  
@return function	void
*/
void APP_addNode(uint32_t Option, uint32_t curAddress);

/*
@brief				This funtion to delete one node in the linked list
@param[in] 			void
@param[out] 		void  
@return function	void
*/
void APP_deleteNode();

/*
@brief				This funtion to run program
@param[in] 			void
@param[out] 		void  
@return function	void
*/
void APP_runProgram();

/*
@brief				This funtion to print content of File or Folder
@param[in] 			void
@param[out] 		void  
@return function	void
*/
void APP_printData();

/*
@brief				This funtion to show list of file and folder
@param[in] 			startAddress: The starting address to read the entry's data
@param[out] 		void  
@return function	void
*/
void APP_printList(uint32_t startAddress);

/*
@brief				This funtion to print content of File
@param[in] 			Address: The starting address of the entry to get the starting cluster
@param[out] 		void  
@return function	void
*/
void APP_printFile(uint32_t Address);

/*
@brief				This funtion to take the user's choice
@param[in] 			void
@param[out] 		Result: Save user's choice  
@return function	uint8_t
*/
uint8_t APP_getOptionUser();

#endif	/*APP.h*/
