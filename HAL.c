#include "HAL.h"

/*Define static Global variable*/
static FILE *filePtr = NULL;

/*Content of the funtion to open file*/
void HAL_openFile(const uint8_t *fileName)
{
    filePtr = fopen(fileName,"rb");
	if (filePtr != NULL)
    {
        printf("Open File Successfully!");
    }
    else
    {
    	printf("Open File Fail!");
    	exit(1);
	}
}

/*Content of the funtion to close file*/
void HAL_closeFile()
{
	fclose(filePtr);
}

/*Content of the function to read Boot sector FAT12 area*/
int32_t HAL_readBoot(bootsectorFAT12 *Boot)
{
	int32_t sizeBoot = sizeof(bootsectorFAT12);
	rewind(filePtr);
	fread(Boot, sizeof(bootsectorFAT12), 1, filePtr);
	return sizeBoot;
}

/*Content of the funtion to get a entry*/
entryType HAL_getAEntry(uint32_t Offset)
{
    entryType Entry;
    fseek(filePtr, Offset, SEEK_SET);
    fread(&Entry, sizeof(entryType), 1, filePtr);
    return Entry;
}

/*Content of the funtion to read the content of multiple sectors from the file*/
int32_t HAL_readMultiSector(uint32_t sectorStart, uint32_t totalSector, uint8_t *saveArr, bootsectorFAT12 *Boot)
{
	uint32_t i = 0;
	int32_t numberOfByte = 0;
    uint32_t offsetReadData = sectorStart * Boot->bytespPerSector;
    fseek(filePtr, offsetReadData, SEEK_SET);
    for (i = 0; i < totalSector; i++)
    {
        fread((saveArr + (Boot->bytespPerSector) * i), Boot->bytespPerSector, 1, filePtr);
    }
    numberOfByte = (Boot->bytespPerSector) * i;
    return numberOfByte;
}

/*Content of the funtion to find remaining clusters of file*/
uint16_t HAL_getRemainCluster(uint16_t currentCluster)
{
	uint16_t fatElement[1];
	uint16_t nextCluster = currentCluster;
	fseek(filePtr, sizeof(bootsectorFAT12), SEEK_SET);
	if (currentCluster % 2 == 0)
	{
		fseek(filePtr, (3 * currentCluster)/2, SEEK_CUR);
		fread(fatElement, 1, 2, filePtr);
		nextCluster = GET_EVEN_CLUSTER(fatElement[0]);
	}
	else
	{
		fseek(filePtr, (3 * currentCluster)/2, SEEK_CUR);
		fread(fatElement, 1, 2, filePtr);
		nextCluster = GET_ODD_CLUSTER(fatElement[0]);
	}
	return nextCluster;
}
