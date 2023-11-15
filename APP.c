/*Include libraries*/
#include "APP.h"
#include "FAT.h"

/*Declare static global variables*/
static nodeType *HEAD = NULL;
static bootsectorFAT12 bootSector;
static uint32_t countMainEntry = 0;
static statusType statusEntry = {None, 0, 0};

/*Content of the funtion to add one node to linked list*/
void APP_addNode(uint32_t Option, uint32_t curAddress)
{
	uint32_t startAddressRDET = (bootSector.reservedSectors + (bootSector.numberOfFats * bootSector.fatSizeSectors)) * bootSector.bytespPerSector;
	uint32_t Offset;
	Offset = startAddressRDET + curAddress;
	entryType Entry;
	uint32_t i;
	uint32_t countOption = 0;
	for (i = 0; countOption < Option; i++)
    {
        Entry = FAT_getAEntry(Offset + (sizeof(entryType) * i));
        if (((Entry.Attribute == 0x00) || (Entry.Attribute == 0x10)) && (Entry.Name[0] != 0x00) && (Entry.Name[0] != 0xE5) && (Entry.Name[0] != 0x2E))
        {
            curAddress = Offset + (sizeof(entryType) * i);
            countOption++;        
        }
    }
    nodeType *newNode =(nodeType*)malloc(sizeof(nodeType));
	if (newNode == NULL)
    {
        exit(1);
    }
    newNode->Data = curAddress;
    newNode->Next = HEAD;
    HEAD = newNode;
}

/*Content of the funtion to delete one node of linked list*/
void APP_deleteNode()
{
	nodeType *currentNode = HEAD;
	if (HEAD != NULL)
	{
		HEAD = HEAD->Next;
		free(currentNode);
	}
}

/*Content of the funtion to show list of file and folder*/
void APP_printList(uint32_t startAddress)
{
	uint8_t Hours, Minutes, Second, Day, Month;
	uint16_t Year;
	uint8_t numMainEntry = 1;
	entryType Entry;
	uint32_t i = 0, j;
	printf("\nName\t\t\t\tModified Time&Date\tType\t\tSize\n");
	do
	{
		Entry = FAT_getAEntry(startAddress + (sizeof(entryType) * i));
		if (Entry.Name[0] != 0xE5 && Entry.Attribute != 0x0F && Entry.Name[0] != 0x2E && Entry.Name[0] != 0x00)
		{
			Hours  = (uint8_t)(((Entry.Time) >> 11) & 0x1F);
			Minutes = (uint8_t)(((Entry.Time) >> 5) & 0x3F);
			Second = (uint8_t)((Entry.Time) & 0x1F);
			Day  = (uint8_t)((Entry.Date) & 0x1F);
			Month = (uint8_t)(((Entry.Date) >> 5) & 0x0F);
			Year  = (((Entry.Date) >> 9) & 0x7F) + 1980;
			printf("%d.", numMainEntry);
			for (j = 0; j < sizeof(Entry.Name) - 3; j++)
			{
				if (Entry.Name[j] == ' ')
				{
					break;
				}
				printf("%c", Entry.Name[j]);
			}
			if (Entry.Attribute == 0x00)
			{
				printf(".");
			}
			for (j = 8; j < sizeof(Entry.Name); j++)
			{
				printf("%c", Entry.Name[j]);
			}
			printf("\t\t\t%02d/%02d/%04d %02d:%02d:%02d", Day, Month, Year, Hours, Minutes, Second);
			if (Entry.Attribute == 0x10)
			{
				printf("\tFolder");
			}
			else
			{
				printf("\tFile");
				printf("\t\t%d", (Entry.Size));
			}
			printf("\n");
			numMainEntry++;
		}
		i++;
	} while (Entry.Name[0] != 0x00);
	countMainEntry = numMainEntry - 1;
}

/*Content of the funtion to print content of File*/
void APP_printFile(uint32_t Address)
{
	uint32_t dataSectorStart = 0;
	entryType Entry;
	uint32_t dataAddressStart = bootSector.reservedSectors + (bootSector.fatSizeSectors * bootSector.numberOfFats) + (bootSector.numberEntryRoot/(bootSector.bytespPerSector/sizeof(entryType)));
	Entry = FAT_getAEntry(Address);
	uint16_t remainCluster = Entry.firstCluster;
	uint8_t dataArray[bootSector.bytespPerSector*bootSector.sectorspPerCluster];
	uint16_t Total_ClusterData = bootSector.totalSectorsShort - ((bootSector.fatSizeSectors * bootSector.numberOfFats) + (bootSector.numberEntryRoot/(bootSector.bytespPerSector/sizeof(entryType)))) / bootSector.sectorspPerCluster;
	system("cls");
	printf("File Name: %s\n", Entry.Name);
	if (Address < (dataAddressStart * bootSector.bytespPerSector))
	{
		dataSectorStart = dataAddressStart - 2 + remainCluster;
		FAT_readMultiSector(dataSectorStart, bootSector.sectorspPerCluster, dataArray, &bootSector);
		printf("%s", dataArray);
	}
	else
	{
		while (remainCluster != 0xFFF &&  remainCluster <= Total_ClusterData)
		{
			dataSectorStart = dataAddressStart - 2 + remainCluster;
			FAT_readMultiSector(dataSectorStart, bootSector.sectorspPerCluster, dataArray, &bootSector);
			printf("%s", dataArray);
			remainCluster = FAT_getRemainCluster(remainCluster);
		}
	}
	printf("\n\t\t*********************************END DATA****************************************\n");
}

/*Content of the funtion to print content of File or Folder*/
void APP_printData()
{
	uint32_t dataAddressStart = (bootSector.bytespPerSector * (bootSector.reservedSectors + (bootSector.fatSizeSectors * bootSector.numberOfFats))) + (bootSector.numberEntryRoot*sizeof(entryType));
    uint32_t startAddress;
    uint32_t i = 0;
    entryType Entry;
    if (HEAD != NULL)
	{
		Entry = FAT_getAEntry(HEAD->Data);
		if (Entry.Attribute == 0x10)
		{	
			startAddress = dataAddressStart + (Entry.firstCluster - 2) * bootSector.bytespPerSector;
			if (statusEntry.Flag != File)
			{
				system("cls");
			}
			printf("Folder Name: %s", Entry.Name);
			APP_printList(startAddress);
			statusEntry.Flag = Folder;
			statusEntry.Address = bootSector.numberEntryRoot * sizeof(entryType) + (Entry.firstCluster - 2) * bootSector.bytespPerSector;
			statusEntry.numMainEntry = countMainEntry;
		}
		else if (Entry.Attribute == 0x00)
        {
            APP_printFile(HEAD->Data);
            statusEntry.Flag = File;
            statusEntry.Address = 0;
            statusEntry.numMainEntry = 0;
        }
	}
	else
	{
		if (statusEntry.Flag != File)
		{
			system("cls");
		}
		statusEntry.Flag = None;
        statusEntry.Address = 0;
        statusEntry.numMainEntry = 0;
	}
}

/*Content of the funtion to take the user's choice*/
uint8_t APP_getOptionUser()
{
	uint8_t Result;
	uint8_t userSelection[2];
	scanf("%s", userSelection);
	uint8_t lengthString = strlen(userSelection);
	if ((lengthString == 1) && (userSelection[0] >= '0') && (userSelection[0] <= '9'))
	{
		Result = userSelection[0] - 48;
	}
	else if (lengthString == 2 && (userSelection[0] >= '0') && (userSelection[0] <= '9') && (userSelection[1] >= '0') && (userSelection[1] <= '9'))
    {
    	Result = (userSelection[0] - 48) * 10 + userSelection[1] - 48;
	}
    else
    {
        return 0xFF;
    }
    return  Result;
}

/*Content of the funtion to run user interface */
void APP_runProgram()
{
	FAT_readBoot(&bootSector);
	uint16_t lengthRDET = bootSector.numberEntryRoot;
	uint32_t startAddressRDET = (bootSector.reservedSectors + (bootSector.numberOfFats * bootSector.fatSizeSectors)) * bootSector.bytespPerSector;
	uint8_t Selection;
	while(1)
	{
		if (HEAD == NULL)
		{
			APP_printList(startAddressRDET);
			statusEntry.numMainEntry = countMainEntry;
		}
		if (statusEntry.Flag == File)
		{
            Selection = 0;
		}
		else
		{
			do
            {
            	if (statusEntry.Flag == Folder)
            	{
            		printf("\nEnter 0 to back.\n");
				}
				else
				{
					printf("\nEnter 0 to exit.\n");
				}
                printf("Enter your choice (0->%d): ", statusEntry.numMainEntry);
                Selection = APP_getOptionUser();
                if(statusEntry.Flag == None && Selection == 0)
                {
                    return;
                }
            } while((Selection > statusEntry.numMainEntry) || (Selection < 0) || (Selection == 0xFF));
		}
		if (Selection == 0)
		{
			APP_deleteNode();
		}
		else
		{
			APP_addNode(Selection, statusEntry.Address);
		}
		APP_printData();
	}
}

/*main function*/
int main() {
	FAT_openFileImg("floppy.img");
	APP_runProgram();
	FAT_closeFileImg();
	return 0;
}
