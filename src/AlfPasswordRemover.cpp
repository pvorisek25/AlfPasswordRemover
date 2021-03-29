/*
===============================================================================
MIT License

Copyright (c) 2020 Petr Veris

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
===============================================================================
*/

#include <iostream>
#include <fstream>
#include <string.h>

#include "ziplib/Source/ZipLib/ZipFile.h"

#if defined(unix) || defined(__unix__) || defined(__unix)
#include <sys/stat.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

#define APR_NAME "AlfPasswordRemover"
#define APR_VER "1.0"

//Function prototypes
void PrintInfo(void);
void CopySubstring(char* dest, char* src, int begin, int end, int destOffset = 0);
int FindString(char* str1, size_t size1, char* str2, size_t size2, int offset);
bool FileExists(char* path);
void FindAndModify(char* charText, size_t size);
char* ReadFile(size_t* size, char* path = (char*)"tmp/test.alf");
void RemovePasswords(int fileCount, char** files);
void CreateDir(char* path);
void RemoveDir(char* path);

//
// main
// Calls the RemovePasswords function which actually does the whole thing
//
int main(int argc, char** argv)
{
    PrintInfo();
    RemovePasswords(argc, argv);
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
void CopySubstring(char* dest, char* src, int begin, int end, int destOffset)
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
	char* settingsFlag = (char*)"aqz_nastavenia=";
	size_t settingsFlagSize = strlen(settingsFlag);

	size_t settingsStartPos = -1;
	size_t settingsEndPos = -1;

	char settingsEndChar = (char)49851;
	
	settingsStartPos = FindString(charText, size, settingsFlag, settingsFlagSize, 0);
	settingsStartPos += settingsFlagSize;
	settingsEndPos = (size_t)FindString(charText, size, &settingsEndChar, 1, (int)settingsStartPos + 1) - 1;

	size_t firstPartSize = settingsStartPos;
	char* firstPart = new char[firstPartSize];
	CopySubstring(firstPart, charText, 0, (int)settingsStartPos);

	size_t secondPartSize = size - settingsEndPos;
	char* secondPart = new char[secondPartSize];
	CopySubstring(secondPart, charText, (int)settingsEndPos, (int)size);

	size_t newTextSize = firstPartSize + strlen("NaN,NaN") + secondPartSize;
	char* newText = new char[newTextSize];

	CopySubstring(newText, firstPart, 0, (int)firstPartSize);
	CopySubstring(newText, (char*)"NaN,NaN", 0, (int)strlen("NaN,NaN"), (int)firstPartSize);
	CopySubstring(newText, secondPart, 0, (int)strlen(secondPart), (int)(firstPartSize + strlen("NaN,NaN")));

	FILE* newFile;
	newFile = fopen("tmp/test.alf", "wb");

	if (newFile != NULL)
	{
		fwrite(newText, newTextSize, 1, newFile);
		fclose(newFile);
	}
	else
		std::cout << "Failed to open new file" << std::endl;

	delete charText;
	delete firstPart;
	delete secondPart;
	delete newText;
}

//
// ReadFile
// Reads the extracted file 
//
char* ReadFile(size_t* size, char* path)
{
	FILE* testFile;
	testFile = fopen(path, "rb");

	if (testFile == NULL)
		return NULL;
	
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
	for (int i = 1; i < fileCount; i++)
	{
		if (!FileExists(files[i]))
		{
			std::cout << "File " << files[i] << " does not exist." << std::endl;
			continue;
		}

		CreateDir((char*)"tmp");
		
		std::cout << "Removing password from " << files[i] << std::endl;
		ZipFile::ExtractFile(files[i], "test.alf", "tmp/test.alf");

		size_t size = 0;
		char* testText = ReadFile(&size);
		
		if (testText == NULL)
		{
			std::cout << "Failed to open " << files[i] << "'s test.alf" << std::endl;
			continue;
		}
		
		FindAndModify(testText, size);

		ZipFile::RemoveEntry(files[i], "test.alf");
		
		ZipFile::AddFile(files[i], "tmp/test.alf");
		std::cout << "Password removed from " << files[i] << std::endl;

		remove("tmp/test.alf");
		RemoveDir((char*)"tmp");
	}
}

//
// CreateDir
// Uses platform-specific functions to create defined directory
//
void CreateDir(char* path)
{
#if defined(unix) || defined(__unix__) || defined(__unix)
	mkdir(path, 0777);
#elif defined(_WIN32)
	CreateDirectory(path, NULL);
#endif
}

//
// RemoveDir
// Uses platform-specific functions to remove defined directory
//
void RemoveDir(char* path)
{
#if defined(unix) || defined(__unix__) || defined(__unix)
	rmdir(path);
#elif defined(_WIN32)
	RemoveDirectory(path);
#endif
}
