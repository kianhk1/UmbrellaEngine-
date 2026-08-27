#include "ReadFile.h"
#include <Windows.h>


bool Engine::CORE::Reader::Compile(const std::string& cppFile, const std::string& outputDll)
{
	HANDLE hRead = nullptr;
	HANDLE hWrite = nullptr;

	SECURITY_ATTRIBUTES sa{};
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
		return false;
	}

	SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

	STARTUPINFOA si{};
	PROCESS_INFORMATION pi{};

	si.cb = sizeof(si);

	si.dwFlags |= STARTF_USESTDHANDLES;

	si.hStdOutput = hWrite;
	si.hStdError = hWrite;
	
	std::string command =
		"cmd.exe /c "
		"\"call \""
		"C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\VC\\Auxiliary\\Build\\vcvarsall.bat"
		"\" x64 && "
		"cl.exe /LD \"" +
		cppFile +
		"\" /Fe:\"" +
		outputDll +
		"\"\"";

	std::vector<char> commandBuffer(
		command.begin(),
		command.end()
	);

	commandBuffer.push_back('\0');

	BOOL result = CreateProcessA( 
		nullptr,
		commandBuffer.data(),
		nullptr,
		nullptr,
		TRUE,
		0,
		nullptr,
		nullptr,
		&si,
		&pi
	);

	if (!result) {
		CloseHandle(hRead);
		CloseHandle(hWrite);

		return false;
	}

	// Parent doesn't need the write handle anymore
	CloseHandle(hWrite);

	char buffer[4096];
	DWORD bytesRead;

	std::string output;

	std::string pending;

	while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, nullptr)) {
		if (bytesRead == 0)
			break;

		buffer[bytesRead] = '\0';

		pending += buffer;

		size_t pos;

		while ((pos = pending.find('\n')) != std::string::npos)
		{
			std::string line = pending.substr(0, pos);

			if (!line.empty() && line.back() == '\r')
				line.pop_back();

			Info(CORE::LogCategory::Resource, line);

			pending.erase(0, pos + 1);
		}
	}

	WaitForSingleObject(
		pi.hProcess,
		INFINITE
	);

	DWORD exitCode = 1;

	GetExitCodeProcess(
		pi.hProcess,
		&exitCode
	);

	CloseHandle(hRead);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return exitCode == 0;
}
