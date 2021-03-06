/* date = July 18th 2021 4:11 pm */

#ifndef UTILITY_H
#define UTILITY_H

static char *
ReadEntireFileToString(char *Filename)
{
    HANDLE FileHandle = CreateFileA(Filename,
                                    GENERIC_READ,
                                    0,
                                    0,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL, // TODO: Read only?
                                    0);
    
    if(FileHandle == INVALID_HANDLE_VALUE)
    {
        // TODO: Logging
        fprintf(stderr, "WARNING: Windows could not open the file %s.\n", Filename);
        return 0;
    }
    
    unsigned int FileSizeInBytes = GetFileSize(FileHandle, 0);
    char *FileBuffer = 0;
    if(FileSizeInBytes)
    {
        FileBuffer = (char *)malloc(FileSizeInBytes + 1);
        memset(FileBuffer, 0, FileSizeInBytes + 1);
        
        unsigned long int BytesRead;
        bool FileRead = ReadFile(FileHandle, FileBuffer, FileSizeInBytes, &BytesRead, 0);
        if(!FileRead)
        {
            // TODO: Logging
            fprintf(stderr, "WARNING: Windows encountered an error while reading from \"%s\". File read aborted.", Filename);
            free(FileBuffer);
            FileBuffer = 0;
        }
    }
    
    CloseHandle(FileHandle);
    
    return FileBuffer;
}


static char *
ReadEntireFile(char *Filename, unsigned long int *TotalBytesRead)
{
    HANDLE FileHandle = CreateFileA(Filename,
                                    GENERIC_READ,
                                    0,
                                    0,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL, // TODO: Read only?
                                    0);
    
    if(FileHandle == INVALID_HANDLE_VALUE)
    {
        // TODO: Logging
        fprintf(stderr, "WARNING: Windows could not open the file %s.\n", Filename);
        return 0;
    }
    
    unsigned int FileSizeInBytes = GetFileSize(FileHandle, 0);
    char *FileBuffer = 0;
    if(FileSizeInBytes)
    {
        FileBuffer = (char *)malloc(FileSizeInBytes);
        memset(FileBuffer, 0, FileSizeInBytes);
        
        unsigned long int BytesRead;
        bool FileRead = ReadFile(FileHandle, FileBuffer, FileSizeInBytes, &BytesRead, 0);
        if(!FileRead)
        {
            // TODO: Logging
            fprintf(stderr, "WARNING: Windows encountered an error while reading from \"%s\". File read aborted.", Filename);
            free(FileBuffer);
            FileBuffer = 0;
        }
        else
        {
            *TotalBytesRead = BytesRead;
        }
    }
    
    return FileBuffer;
}

internal char *
Extension(char *FilePath)
{
    char *Result = FilePath;
    while(*Result && *Result != '.')
    {
        Result++;
    }
    
    return Result;
}

#endif //UTILITY_H
