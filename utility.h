/* date = July 18th 2021 4:11 pm */

#ifndef UTILITY_H
#define UTILITY_H

struct image
{
    unsigned char *Data;
    char *Filename;
    int Width;
    int Height;
    int ColorChannels;
};

// NOTE: Requires STB_IMAGE_IMPLEMENTATION definition and stb_image.h
internal image
LoadImage(char *Filename)
{
    image Result = {};
    Result.Filename = Filename;
    Result.Data = stbi_load(Filename,
                            &Result.Width,
                            &Result.Height,
                            &Result.ColorChannels,
                            0);
    
    fprintf(stderr, "%s\n", stbi_failure_reason());
    
    return Result;
}

internal char *
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


internal unsigned char *
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
    unsigned char *FileBuffer = 0;
    if(FileSizeInBytes)
    {
        FileBuffer = (unsigned char *)malloc(FileSizeInBytes);
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

#endif //UTILITY_H
