#ifndef __HAL_H__
#define __HAL_H__

/*Include Library*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*Preprocessor directive: No Padding*/
#pragma pack(1)

/* @brief: Function like macro to get remain cluster of File */
#define GET_EVEN_CLUSTER(x)		((uint16_t)(x) & 0xfffU)
#define GET_ODD_CLUSTER(x)		(((uint16_t)(x) & 0xfff0U) >> 4)

/* @brief: Structure for the FAT12 Boot Sector Area */
typedef struct
{
	uint8_t jmp[3];
    uint8_t oem[8];
    uint16_t bytespPerSector;
    uint8_t sectorspPerCluster;
    uint16_t reservedSectors;
    uint8_t numberOfFats;
    uint16_t numberEntryRoot;
    uint16_t totalSectorsShort;
    uint8_t mediaDescriptor;
    uint16_t fatSizeSectors;
    uint16_t sectorsPerTrack;
    uint16_t numberOfHeads;
    uint32_t hiddenSectors;
    uint32_t totalSectorsLong;
    uint8_t driveNumber;
    uint8_t Preventive;
    uint8_t bootSignature;
    uint32_t volumeId;
    uint8_t volumeLabel[11];
    uint8_t fatType[8];
    uint8_t bootCode[448];
    uint16_t bootSectorSignature;
} bootsectorFAT12;

/* @brief Structure for the Main Entry */
typedef struct{
    uint8_t Name[11];
    uint8_t Attribute;
    uint8_t Reserved[10];
    uint16_t Time;
    uint16_t Date;
    uint16_t firstCluster;
    uint32_t Size;
} entryType;

/**********************************************************	FUNCTION PROTOTYPE	**********************************************************/

/*
@brief				This funtion to open file
@param[in]			diskName: Name of the Disk to open
@param[out]			void
@return function	void
*/
void HAL_openFile(const uint8_t *fileName);

/*
@brief				This funtion to close file
@param[in]			void
@param[out]			void
@return function	void
*/
void HAL_closeFile();

/*
@brief				This function to read data of the Boot sector Area
@param[in]			Boot: A pointer to the bootsectorFAT12 structure to store the read data.
@param[out]			sizeBoot: Size of Boot Sector FAT12 area
@return function	int32_t
*/
int32_t HAL_readBoot(bootsectorFAT12 *Boot);

/*
@brief				This funtion to get a Entry
@param[in] 			Offset: The starting address to read the Entry from
@param[out] 		Entry:  To save the entry just read
@return function	entryType
*/
entryType HAL_getAEntry(uint32_t Offset);

/*
@brief				This function to read the content of multiple sectors from the disk
@param[in] 			sectorStart: The starting sector to read from
					totalSector: The total number of sectors to read
					saveArr: A pointer to the array to store the read data
					Boot: A pointer to the bootsectorFAT12 structure containing boot sector information
@param[out] 		numberOfByte: Number of bytes read
@return function	int32_t
*/
int32_t HAL_readMultiSector(uint32_t sectorStart, uint32_t totalSector, uint8_t *saveArr, bootsectorFAT12 *Boot);

/*
@brief				This function to find remaining clusters of the file
@param[in] 			currentCluster: Current cluster of the file
@param[out] 		nextCluster: Next cluster of the file
@return function	uint16_t
*/
uint16_t HAL_getRemainCluster(uint16_t currentCluster);
#endif	/*HAL.h*/
