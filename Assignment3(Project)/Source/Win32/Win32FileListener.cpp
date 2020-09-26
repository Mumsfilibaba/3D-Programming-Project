#include "..\Core\Utilities\Utilities.h"
#include "..\Core\FileManegment\FileListener.h"


#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>

//Thread-loop
void FileListener::Watch(FileListener* listener, std::string path)
{
	//Check so ptr is valid
	if (listener == nullptr)
		return;

	//Create a directory handle (Windows specific that supports "asynchronous" - use)
	HANDLE dirHandle = CreateFileA(path.c_str(), GENERIC_READ | FILE_LIST_DIRECTORY,
				FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE, 0,
				OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, 0);
	
	//Does not exist
	if (dirHandle == INVALID_HANDLE_VALUE)
		return;

	//Variables
	int id = 0;
	int offset = 0;
	bool wait = true;
	char buffer[1024];
	char filename[MAX_PATH];
	DWORD timeOutRes = 0;
	DWORD bytesWritten = 0;
	OVERLAPPED overlap;
	FILE_NOTIFY_INFORMATION* notify = nullptr;

	//Loop for changes
	while (listener->IsRunning())
	{
		//Make sure thread will wait for changes
		wait = true;
		offset = 0;
		bytesWritten = 0;
		notify = nullptr;

		//Create event
		ZeroBlock(&overlap, sizeof(OVERLAPPED));
		overlap.hEvent = CreateEvent(0, TRUE, FALSE, 0);

		//Setup event
		if (!ReadDirectoryChangesW(dirHandle, &buffer, sizeof(buffer), 0,
			FILE_NOTIFY_CHANGE_LAST_WRITE, &bytesWritten, &overlap, 0))
		{
			CloseHandle(overlap.hEvent);
			break;
		}

		//Wait for changes
		while (wait)
		{
			//Break if application is exited before finding any changes
			if (!listener->IsRunning())
				break;

			//Check for changes
			timeOutRes = WaitForSingleObject(overlap.hEvent, 250);
			if (timeOutRes == WAIT_OBJECT_0)
			{
				//Something happened
				CloseHandle(overlap.hEvent);

				do
				{
					notify = (FILE_NOTIFY_INFORMATION*)(buffer + offset);
				
					//Find out the filename
					ZeroBlock(filename, sizeof(filename));
					wcstombs(filename, notify->FileName, sizeof(filename));
					filename[notify->FileNameLength / sizeof(WCHAR)] = 0;
					
					//Check if the file is on the watchlist
					id = listener->GetFileID(filename);
					if (id >= 0)
						listener->PostChange(id);

					offset += notify->NextEntryOffset;
				} while (notify->NextEntryOffset != 0);

				//Stop wait
				wait = false;
			}
			else if (timeOutRes == WAIT_FAILED)
			{
				//Error Exit thread
				CloseHandle(dirHandle);
				CloseHandle(overlap.hEvent);
				return;
			}
		}
	}

	CloseHandle(dirHandle);
	CloseHandle(overlap.hEvent);
	return;
}
#endif