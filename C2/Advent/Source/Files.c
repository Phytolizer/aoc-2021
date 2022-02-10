#include "Advent/Files.h"
#include "Advent/Core.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#define ERRBUF_SIZE 64

#ifdef _WIN32
static void printLastError(void)
{
  LPSTR outBuffer;

  if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0,
                    (LPSTR)&outBuffer, 16, NULL) == 0)
  {
    return;
  }

  fprintf(stderr, "%s\n", outBuffer);
  LocalFree(outBuffer);
}
#endif

AdventString adventSlurpFile(const char* filePath)
{
#ifdef _WIN32
  HANDLE f = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (f == INVALID_HANDLE_VALUE)
  {
    fprintf(stderr, "ERROR: could not open file %s: ", filePath);
    printLastError();
    return (AdventString){0};
  }

  LARGE_INTEGER fileSize;

  if (!GetFileSizeEx(f, &fileSize))
  {
    fprintf(stderr, "ERROR: could not get file size for %s: ", filePath);
    printLastError();
    CloseHandle(f);
    return (AdventString){0};
  }

  struct string_span readResult;

  readResult.data = calloc(fileSize.LowPart, 1);

  readResult.length = fileSize.LowPart;

  if (readResult.data == NULL)
  {
    CloseHandle(f);
    fprintf(stderr, "ERROR: could not allocate memory to read file %s\n", filePath);
    return (AdventString){0};
  }

  DWORD nread;

  if (!ReadFile(f, readResult.data, fileSize.LowPart, &nread, NULL))
  {
    fprintf(stderr, "ERROR: could not read file %s: ", filePath);
    printLastError();
    CloseHandle(f);
    free(readResult.data);
    return (AdventString){0};
  }

  CloseHandle(f);
  return readResult;
#else
  int fd = open(filePath, O_RDONLY | O_NOFOLLOW | O_CLOEXEC);

  if (fd == -1)
  {
    char errbuf[ERRBUF_SIZE];
    strerror_r(errno, errbuf, sizeof errbuf);
    fprintf(stderr, "ERROR: Could not open %s: %s\n", filePath, errbuf);
    return (AdventString){0};
  }

  struct stat statbuf;

  if (fstat(fd, &statbuf) == -1)
  {
    char errbuf[ERRBUF_SIZE];
    strerror_r(errno, errbuf, sizeof errbuf);
    fprintf(stderr, "ERROR: Could not obtain file size for %s: %s\n", filePath, errbuf);
    close(fd);
    return (AdventString){0};
  }

  off_t fileSize = statbuf.st_size;
  AdventString result = {calloc(fileSize, 1), fileSize};

  if (result.data == NULL)
  {
    close(fd);
    fprintf(stderr, "ERROR: could not allocate memory to read file %s\n", filePath);
    return result;
  }

  if (read(fd, result.data, fileSize) == -1)
  {
    char errbuf[ERRBUF_SIZE];
    strerror_r(errno, errbuf, sizeof errbuf);
    fprintf(stderr, "ERROR: could not read file %s: %s\n", filePath, errbuf);
    close(fd);
    free(result.data);
    return (AdventString){0};
  }

  close(fd);
  return result;
#endif
}
