#include "FileListener.h"
#include "..\Utilities\Utilities.h"
#include <iostream>

FileListener::FileListener()
	: m_IsRunning(false),
	m_Path(),
	m_Files(0),
	m_ChangedFiles(0),
	m_Thread(),
	__m_IsRunning(),
	__m_Files(),
	__m_ChangedFiles()
{
}

FileListener::FileListener(const std::string& path)
	: m_IsRunning(false),
	m_Path(),
	m_Files(0),
	m_ChangedFiles(),
	m_Thread(),
	__m_IsRunning(),
	__m_Files(),
	__m_ChangedFiles()
{
	Create(path);
}

FileListener::~FileListener()
{
	Quit();

	if (m_Thread.joinable())
		m_Thread.join();
}


bool FileListener::Create(const std::string& path)
{
	if (!system_directory_exists(path))
		return false;

	//Set startup-values
	Start();
	m_Path = path;
	
	//Startup thread
	m_Thread = std::thread(FileListener::Watch, this, m_Path);
	return true;
}

void FileListener::HandleChanges()
{
	std::lock_guard<std::mutex> lock(__m_ChangedFiles);
	std::lock_guard<std::mutex> lock2(__m_Files);
	
	if (m_ChangedFiles.size() < 1)
		return;

	for (int i = 0; i < m_ChangedFiles.size(); i++)
	{
		//reference to make code easier to read
		File& file = m_Files[m_ChangedFiles[i]];
		
		//Tick the files clock to make sure not to handle to many writes
		file.clock.Tick();
		if (file.clock.DeltaTime().AsMilliSeconds() < 150)
			continue;

		//Handle change
		file.clock.Reset();

		if (file.callbackObject != nullptr)
			file.callbackObject->OnFileModified();
		else if (file.callback != nullptr)
			file.callback();
	}

	m_ChangedFiles.clear();
}

int FileListener::GetFileID(const std::string& filename)
{
	std::lock_guard<std::mutex> lock(__m_Files);

	for (int i = 0; i < m_Files.size(); i++)
	{
		if (m_Files[i].name == filename)
			return i;
	}

	return -1;
}

//Handle threading
bool FileListener::IsRunning()
{
	std::lock_guard<std::mutex> lock(__m_IsRunning);
	return m_IsRunning;
}

void FileListener::Start()
{
	std::lock_guard<std::mutex> lock(__m_IsRunning);
	m_IsRunning = true;
}

void FileListener::Quit()
{
	std::lock_guard<std::mutex> lock(__m_IsRunning);
	m_IsRunning = false;
}

void FileListener::PostChange(int fileID)
{
	std::lock_guard<std::mutex> lock(__m_ChangedFiles);
	m_ChangedFiles.emplace_back(fileID);
}

bool FileListener::AddFile(const std::string& filename, IFileCallback* const onModified)
{
	if (GetFileID(filename) >= 0)
		return true;

	if (!system_file_exists(filename, m_Path))
		return false;


	File file;
	file.name = filename;
	file.callbackObject = onModified;
	file.callback = nullptr;

	std::lock_guard<std::mutex> lock(__m_Files);

	m_Files.emplace_back(file);
	return true;
}

bool FileListener::AddFile(const std::string& filename, void(*onModified)())
{
	if (GetFileID(filename) >= 0)
		return true;

	if (!system_file_exists(filename, m_Path))
		return false;

	std::lock_guard<std::mutex> lock(__m_Files);
	
	File file;
	file.name = filename;
	file.callbackObject = nullptr;
	file.callback = onModified;

	m_Files.emplace_back(file);
	return true;
}