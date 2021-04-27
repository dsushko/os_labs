#pragma warning(disable:4996)

#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "tlhelp32.h"
#include <string>
#include <vector>
#include <sstream>
#include <TlHelp32.h>
#include <iomanip>



int main(int argc, char* argv[])
{

	std::string proc_to_kill;

	char const* temp = getenv("PROC_TO_KILL");
	if (temp != NULL)
	{
		proc_to_kill = std::string(temp);
	}

	

	std::string procname;
	if (argc > 1) {
		procname = std::string(argv[1]);
		procname += ".exe";
	}
	std::wstring wstr(procname.begin(), procname.end());

	const DWORD buffSize = 65535;
	char buffer[buffSize];
	std::stringstream ss(proc_to_kill);

	//проверка процессов из переменной
	{
		std::stringstream ss1(proc_to_kill);
		while (ss1.getline(reinterpret_cast<char*>(&buffer), buffSize, ',')) {
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			PROCESSENTRY32 pe = { 0 };
			pe.dwSize = sizeof(PROCESSENTRY32);

			HANDLE process;
			Process32First(hSnap, &pe);
			bool exists = false;
			do
			{
				std::string cur_proc = std::string(buffer);
				cur_proc += ".exe";
				std::wstring wcur_proc(cur_proc.begin(), cur_proc.end());
				if (wcur_proc.compare(pe.szExeFile) == 0) {
					std::cout << "Process with name \"" << cur_proc << "\" exists\n";
					exists = true;
				}
			} while (Process32Next(hSnap, &pe));
			if (!exists) {
				std::cout << "Process with name \"" << buffer << "\" doesn't exist\n";
			}
			CloseHandle(hSnap);
		}
	}
	//убивство того что забито в переменной
	while (ss.getline(reinterpret_cast<char*>(&buffer), buffSize, ',')) {
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 pe = { 0 };
		pe.dwSize = sizeof(PROCESSENTRY32);

		HANDLE process;
		Process32First(hSnap, &pe);
		do
		{
			std::string cur_proc = std::string(buffer);
			cur_proc += ".exe";
			std::wstring wcur_proc(cur_proc.begin(), cur_proc.end());
			if (wcur_proc.compare(pe.szExeFile) == 0) {
				process = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pe.th32ProcessID);
				TerminateProcess(process, 0);
				WaitForSingleObject(process, INFINITE);
				CloseHandle(process);
			}
		} while (Process32Next(hSnap, &pe));
		CloseHandle(hSnap);
	}
	//проверка процессов из переменной
	{
		std::stringstream ss1(proc_to_kill);
		while (ss1.getline(reinterpret_cast<char*>(&buffer), buffSize, ',')) {
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			PROCESSENTRY32 pe = { 0 };
			pe.dwSize = sizeof(PROCESSENTRY32);

			HANDLE process;
			Process32First(hSnap, &pe);
			bool exists = false;
			do
			{
				std::string cur_proc = std::string(buffer);
				cur_proc += ".exe";
				std::wstring wcur_proc(cur_proc.begin(), cur_proc.end());
				if (wcur_proc.compare(pe.szExeFile) == 0) {
					std::cout << "Process with name \"" << cur_proc << "\" exists\n";
					exists = true;
				}
			} while (Process32Next(hSnap, &pe));
			if (!exists) {
				std::cout << "Process with name \"" << buffer << "\" doesn't exist\n";
			}
			CloseHandle(hSnap);
		}
	}



	//проверка того что передано параметрами
	{

		PROCESSENTRY32 me32;

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		me32.dwSize = sizeof(PROCESSENTRY32);
		Process32First(hSnap, &me32);
		HANDLE process = { 0 };

		if (argc > 2) {
			std::string param_to_kill(argv[2]);
			std::wstring wparam_to_kill(param_to_kill.begin(), param_to_kill.end());
			bool exists = false;
			while (Process32Next(hSnap, &me32))
			{
				if (strstr(argv[1], "--id")) {
					if (me32.th32ProcessID == atoi(argv[2])) {
						std::cout << "Process with id " << me32.th32ProcessID << " exists\n";
						exists = true;
					}
				}
				if (strstr(argv[1], "--name")) {

					std::wstring wcur_proc_name = me32.szExeFile;
					wcur_proc_name = wcur_proc_name.substr(0, wcur_proc_name.size() - 4);
					std::wstring wparam_formatted = wparam_to_kill.substr(0, wcur_proc_name.size());

					if (wcur_proc_name == wparam_formatted) {
						std::wcout << L"Process with name \"" << wcur_proc_name << L"\" exists\n";
						exists = true;
					}
				}
			}
			if (!exists) {
				if (strstr(argv[1], "--name")) {
					std::cout << "Process with name \"" << argv[2] << "\" doesn't exist\n";
				}
				else if (strstr(argv[1], "--id")) {
					std::cout << "Process with id " << atoi(argv[2]) << " doesn't exist\n";
				}
			}
		};
		CloseHandle(hSnap);

	}
	//убивство того что передали параметрами
	if (argc > 2) {
		PROCESSENTRY32 me32;

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		me32.dwSize = sizeof(PROCESSENTRY32);
		Process32First(hSnap, &me32);
		HANDLE process = { 0 };
		std::string param_to_kill(argv[2]);
		std::wstring wparam_to_kill(param_to_kill.begin(), param_to_kill.end());
		while (Process32Next(hSnap, &me32))
		{
			if (strstr(argv[1], "--id")) {
				if (me32.th32ProcessID == atoi(argv[2])) {
					process = OpenProcess(1, false, me32.th32ProcessID);
					if (process)
						TerminateProcess(process, -9);
				}
			}
			if (strstr(argv[1], "--name")) {

				std::wstring wcur_proc_name = me32.szExeFile;
				wcur_proc_name = wcur_proc_name.substr(0, wcur_proc_name.size() - 4);
				std::wstring wparam_formatted = wparam_to_kill.substr(0, wcur_proc_name.size());

				if (wcur_proc_name == wparam_formatted) {
					process = OpenProcess(1, false, me32.th32ProcessID);
					if (process)
						TerminateProcess(process, -9);
				}
			}
		}
		CloseHandle(hSnap);

	};
	Sleep(1000);
	//проверка того что передано параметрами
	{
		PROCESSENTRY32 me32;

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		me32.dwSize = sizeof(PROCESSENTRY32);
		Process32First(hSnap, &me32);
		HANDLE process = { 0 };
		if (argc > 2) {
			std::string param_to_kill(argv[2]);
			std::wstring wparam_to_kill(param_to_kill.begin(), param_to_kill.end());
			bool exists = false;
			while (Process32Next(hSnap, &me32))
			{
				if (strstr(argv[1], "--id")) {
					if (me32.th32ProcessID == atoi(argv[2])) {
						std::cout << "Process with id " << me32.th32ProcessID << " exists\n";
						exists = true;
					}
				}
				if (strstr(argv[1], "--name")) {

					std::wstring wcur_proc_name = me32.szExeFile;
					wcur_proc_name = wcur_proc_name.substr(0, wcur_proc_name.size() - 4);
					std::wstring wparam_formatted = wparam_to_kill.substr(0, wcur_proc_name.size());

					if (wcur_proc_name == wparam_formatted) {
						std::wcout << L"Process with name \"" << wcur_proc_name << L"\" exists\n";
						exists = true;
					}
				}
			}
			if (!exists) {
				if (strstr(argv[1], "--name")) {
					std::cout << "Process with name \"" << argv[2] << "\" doesn't exist\n";
				}
				else if (strstr(argv[1], "--id")) {
					std::cout << "Process with id " << atoi(argv[2]) << " doesn't exist\n";
				}
			}
		};
		CloseHandle(hSnap);

	}


	std::cin.get();

	return 0;
}
