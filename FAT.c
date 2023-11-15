#include "FAT.h"

/*Content of the function to open img file*/
void FAT_openFileImg(const uint8_t *imgFileName)
{
	HAL_openFile(imgFileName);
}

/*Content of the function to close img file*/
void FAT_closeFileImg()
{
	HAL_closeFile();
}

/*Content of the function to read Boot sector area*/
int32_t FAT_readBoot(bootsectorFAT12 *Boot)
{
	int32_t sizeBoot;
	sizeBoot = HAL_readBoot(Boot);
	return sizeBoot;
}

/*Content of the funtion to get a entry*/
entryType FAT_getAEntry(uint32_t Offset)
{
	entryType Entry;
	Entry = HAL_getAEntry(Offset);
	return Entry;
}

/*Content of the funtion to read the content of multiple sectors from the file*/
int32_t FAT_readMultiSector(uint32_t sectorStart, uint32_t totalSector, uint8_t *saveArr, bootsectorFAT12 *Boot)
{
	int32_t numberOfByte = 0;
	numberOfByte = HAL_readMultiSector(sectorStart, totalSector, saveArr, Boot);
	return numberOfByte;
}

/*Content of the funtion to find remaining clusters of file*/
uint16_t FAT_getRemainCluster(uint16_t currentCluster)
{
	uint16_t nextCluster;
	nextCluster = HAL_getRemainCluster(currentCluster);
	return nextCluster;
}