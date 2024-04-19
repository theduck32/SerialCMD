#include <iostream>
#include <fstream>
#include <windows.h>

HANDLE hSerial;
std::ofstream logFile;

void SerialInit() {
	hSerial = CreateFile("COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSerial == INVALID_HANDLE_VALUE) {
		DWORD errorCode = GetLastError();
		std::cerr << "Error opening serial port (COM1). Error code: " << errorCode << std::endl;
		logFile << "Error opening serial port (COM1). Error code: " << errorCode << std::endl;
		exit(1);
	}
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams)) {
		DWORD errorCode = GetLastError();
		std::cerr << "Error getting serial port info. Error code: " << errorCode << std::endl;
		logFile << "Error getting serial port info. Error code: " << errorCode << std::endl;
		CloseHandle(hSerial);
		exit(1);
	}
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	if (!SetCommState(hSerial, &dcbSerialParams)) {
		DWORD errorCode = GetLastError();
		std::cerr << "Error setting serial port state (maybe change baudrate). Error code: " << errorCode << std::endl;
		logFile << "Error setting serial port state (maybe change baudrate). Error code: " << errorCode << std::endl;
		CloseHandle(hSerial);
		exit(1);
	}
}

void SerialKill() {
	CloseHandle(hSerial);
}

void SerialWrite(const std::string& data) {
	DWORD bytesWritten;
	WriteFile(hSerial, data.c_str(), data.length(), &bytesWritten, NULL);
}

std::string SerialReceive() {
	char incomingData;
	DWORD bytesRead;
	std::string receivedData;
	do {
		ReadFile(hSerial, &incomingData, sizeof(incomingData), &bytesRead, NULL);
		if (bytesRead > 0 && isprint(incomingData)) {
			receivedData += incomingData;
			SerialWrite(&incomingData);
		}
	} while (incomingData != '\r');

	return receivedData;
}

std::string SerialExec(const std::string& command) {
	char buffer[4096];
	std::string result;
	FILE* pipe = _popen(("cmd.exe /c " + command).c_str(), "r");
	if (!pipe) {
		DWORD errorCode = GetLastError();
		std::cerr << "Error executing command. Error code: " << errorCode << std::endl;
		logFile << "Error executing command. Error code: " << errorCode << std::endl;
		return "Error executing command";
	}
	while (!feof(pipe)) {
		if (fgets(buffer, 4096, pipe) != NULL) {
			result += buffer;
		}
	}
	_pclose(pipe);
	return result;
}

int main() {
	logFile.open("SerialCMD.log", std::ios::out | std::ios::app);
	if (!logFile.is_open()) {
		std::cerr << "Error opening log file" << std::endl;
		return 1;
	}

	SerialInit();
	SerialWrite("\nSerialCMD - duck32 (2024)\r\n");
	SerialWrite("\nGet the source code : https://github.com/theduck32/SerialCMD  \r\n");

	while (true) {
		SerialWrite("\n> ");
		std::string input = SerialReceive();
		if (input.length() > 0) {
			std::string output = SerialExec(input);
			SerialWrite("\r\n" + output);
		}
		if (input == "exit\r") {
			break;
		}
		PurgeComm(hSerial, PURGE_RXCLEAR);
	}
	SerialKill();
	logFile.close();
	return 0;
}
