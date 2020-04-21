#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include "ziplib/Source/ZipLib/ZipFile.h"

#define APR_NAME "AlfPasswordRemover"
#define APR_VER "1.0"

//Function prototypes
void PrintInfo(void);
void CopySubstring(char* dest, char* src, int begin, int end, int destOffset = 0);
int FindString(char* str1, size_t size1, char* str2, size_t size2, int offset);
bool FileExists(char* path);
void FindAndModify(char* charText, size_t size);
char* ReadFile(size_t* size, char* path = "tmp/test.alf");
void RemovePasswords(int fileCount, char** files);

//
// main
// Calls the RemovePasswords function which actually does the whole thing
//
int main(int argc, char** argv)
{
	PrintInfo();
	char* file[2] = { "1324.alf", "mytest.alf" };
	
	RemovePasswords(2, file);
	return 0;
}

//
// PrintInfo
// Prints program's name and version at the start of execution
//
void PrintInfo(void)
{
	std::cout << APR_NAME << " [Version " << APR_VER << "]" << std::endl;
	std::cout << std::endl;
}

//
// CopySubstring
// Copies defined part of given char array into another one
//
void CopySubstring(char* dest, char* src, int begin, int end, int destOffset = 0)
{
	int destIndex = destOffset;

	for (int i = begin; i < end; i++)
	{
		dest[destIndex] = src[i];
		destIndex++;
	}
}

//
// FindString
// Finds position of a defined text in a defined char array
//
int FindString(char* str1, size_t size1, char* str2, size_t size2, int offset)
{
	for (int i = offset; i < size1; i++)
	{
		int startPos = i;
		bool found = true;

		for (int n = 0; n < size2; n++)
		{
			if (str1[i + n] != str2[n])
			{
				found = false;
				break;
			}
		}

		if (found)
			return startPos;
	}

	return -1;
}

//
// FileExists
// Checks if the defined file exists
//
bool FileExists(char* path)
{
	FILE* file;

	if (file = fopen(path, "r"))
	{
		fclose(file);
		return true;
	}

	return false;
}

//
// FindAndModify
// Does the whole thing. It finds and modify the password saved in the [sometest].alf/test.alf file
//
void FindAndModify(char* charText, size_t size)
{
	char* settingsFlag = "aqz_nastavenia=";
	size_t settingsFlagSize = strlen(settingsFlag);

	size_t settingsStartPos = -1;
	size_t settingsEndPos = -1;

	settingsStartPos = FindString(charText, size, settingsFlag, settingsFlagSize, 0);
	settingsStartPos += settingsFlagSize;
	settingsEndPos = FindString(charText, size, "»", strlen("»"), settingsStartPos + 1) - 1;

	size_t firstPartSize = settingsStartPos;
	char* firstPart = new char[firstPartSize];
	CopySubstring(firstPart, charText, 0, settingsStartPos);

	size_t secondPartSize = size - settingsEndPos;
	char* secondPart = new char[secondPartSize];
	CopySubstring(secondPart, charText, settingsEndPos, size);

	size_t newTextSize = firstPartSize + strlen("NaN,NaN") + secondPartSize;
	char* newText = new char[newTextSize];

	CopySubstring(newText, firstPart, 0, firstPartSize);
	CopySubstring(newText, "NaN,NaN", 0, strlen("NaN,NaN"), firstPartSize);
	CopySubstring(newText, secondPart, 0, strlen(secondPart), firstPartSize + strlen("NaN,NaN"));

	FILE* newFile;
	newFile = fopen("tmp/test.alf", "wb");

	fwrite(newText, newTextSize, 1, newFile);
	fclose(newFile);

	delete charText;
	delete firstPart;
	delete secondPart;
	delete newText;
}

//
// ReadFile
// Reads the extracted file 
//
char* ReadFile(size_t* size, char* path = "tmp/test.alf")
{
	FILE* testFile;
	testFile = fopen(path, "rb");

	fseek(testFile, 0, SEEK_END);
	*size = ftell(testFile);
	fseek(testFile, 0, SEEK_SET);

	char* returnValue = new char[*size];
	fread(returnValue, *size, 1, testFile);

	fclose(testFile);
	return returnValue;
}

//
// RemovePasswords
// Loops through all of the defined files and removes their password
//
void RemovePasswords(int fileCount, char** files)
{
	for (unsigned int i = 0; i < fileCount; i++)
	{
		if (!FileExists(files[i]))
		{
			std::cout << "File " << files[i] << " does not exist." << std::endl;
			continue;
		}

		std::cout << "Removing password from " << files[i] << std::endl;
		ZipFile::ExtractFile(files[i], "test.alf", "tmp/test.alf");

		size_t size = 0;
		FindAndModify(ReadFile(&size), size);

		ZipFile::AddFile(files[i], "tmp/test.alf");
		std::cout << "Password removed from " << files[i] << std::endl;
	}
}