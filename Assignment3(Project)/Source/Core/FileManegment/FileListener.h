#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "..\Utilities\Clock.h"
#include "IFileCallback.h"

//FileListener
class FileListener
{
private:
	struct File
	{
		std::string name;
		IFileCallback* callbackObject = nullptr;
		void(*callback)() = nullptr;
		Clock clock;
	};

public:
	FileListener();
	FileListener(const std::string& path);
	~FileListener();

	bool Create(const std::string& Path);
	void HandleChanges();
	int GetFileID(const std::string& filename);

	bool AddFile(const std::string& filename, IFileCallback* const onModified);
	bool AddFile(const std::string& filename, void(*onModified)());

	inline const std::string& GetPath() const
	{
		return m_Path;
	}

private:
	bool IsRunning();
	void Start();
	void Quit();
	void PostChange(int fileID);

private:
	//Variables
	bool m_IsRunning;
	std::string m_Path;
	std::vector<int> m_ChangedFiles;
	std::vector<File> m_Files;
	//Locks
	std::mutex __m_IsRunning;
	std::mutex __m_Files;
	std::mutex __m_ChangedFiles;
	//Thread
	std::thread m_Thread;

private:
	static void Watch(FileListener* listener, std::string path);
};