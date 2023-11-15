#ifndef __FAT_H__
#define __FAT_H__

/*Include Library*/
#include "HAL.h"

/**********************************************************	FUNCTION PROTOTYPE	**********************************************************/

/*
@brief				This function to open img file
@param[in]			imgFileName: Name of img file
@param[out]			void
@return function	void
*/
void FAT_openFileImg(const uint8_t *imgFileName);

/*
@brief				This function to close img file
@param[in]			void
@param[out]			void
@return function	void
*/
void FAT_closeFileImg();

/*
@brief				This function to read data of the Boot sector Area
@param[in]			Boot: A pointer to the bootsectorFAT12 structure to store the read data.
@param[out]			sizeBoot: Size of Boot Sector FAT12 area
@return function	int32_t
*/
int32_t FAT_readBoot(bootsectorFAT12 *Boot);

/*
@brief				This funtion to get a Entry
@param[in] 			Offset: The starting address to read the Entry from
@param[out] 		Entry:  To save the entry just read
@return function	entryType
*/
entryType FAT_getAEntry(uint32_t Offset);

/*
@brief				This function to read the content of multiple sectors from the disk
@param[in] 			sectorStart: The starting sector to read from
					totalSector: The total number of sectors to read
					saveArr: A pointer to the array to store the read data
					Boot: A pointer to the bootsectorFAT12 structure containing boot sector information
@param[out] 		numberOfByte: Number of bytes read
@return function	int32_t
*/
int32_t FAT_readMultiSector(uint32_t sectorStart, uint32_t totalSector, uint8_t *saveArr, bootsectorFAT12 *Boot);

/*
@brief				This function to find remaining clusters of the file
@param[in] 			currentCluster: Current cluster of the file
@param[out] 		nextCluster: Next cluster of the file
@return function	uint16_t
*/
uint16_t FAT_getRemainCluster(uint16_t currentCluster);
#endif	/*FAT.h*/
