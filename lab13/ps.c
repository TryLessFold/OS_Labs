#include <windows.h>
#include <stdio.h>
#include <conio.h>

int main()
{
    // ���� ��������� �������� ������
    BOOL fConnected;

    // ������������� ������ Pipe
    HANDLE hNamedPipe;

    // ��� ������������ ������ Pipe
    LPSTR lpszPipeName = "\\\\.\\pipe\\$MyPipe$";

    // ����� ��� �������� ������ ����� �����
    char szBuf[512];
    char szBuf2[256];

    // ���������� ���� ������, �������� ����� �����
    DWORD cbRead;

    // ���������� ���� ������, ���������� ����� �����
    DWORD cbWritten;

    printf("Named pipe server demo\n");

    // ������� ����� Pipe, ������� ��� lpszPipeName
    hNamedPipe = CreateNamedPipe(
        lpszPipeName,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        512, 512, 5000, NULL);

    // ���� �������� ������, ������� �� ��� � ���������
    // ������ ����������
    if (hNamedPipe == INVALID_HANDLE_VALUE) {
        fprintf(stdout, "CreateNamedPipe: Error %ld\n", GetLastError());
        getch();
        return 0;
    }

    // ������� ��������� � ������ �������� �������� ������
    fprintf(stdout, "Waiting for connect...\n");

    // ������� ���������� �� ������� �������
    fConnected = ConnectNamedPipe(hNamedPipe, NULL);

    // ��� ������������� ������ ������� �� ���
    if (!fConnected) {
        switch (GetLastError()) {
        case ERROR_NO_DATA:
            fprintf(stdout, "ConnectNamedPipe: ERROR_NO_DATA");
            getch();
            CloseHandle(hNamedPipe);
            return 0;
            break;

        case ERROR_PIPE_CONNECTED:
            fprintf(stdout, "ConnectNamedPipe: ERROR_PIPE_CONNECTED");
            getch();
            CloseHandle(hNamedPipe);
            return 0;
            break;

        case ERROR_PIPE_LISTENING:
            fprintf(stdout, "ConnectNamedPipe: ERROR_PIPE_LISTENING");
            getch();
            CloseHandle(hNamedPipe);
            return 0;
            break;

        case ERROR_CALL_NOT_IMPLEMENTED:
            fprintf(stdout, "ConnectNamedPipe: ERROR_CALL_NOT_IMPLEMENTED");
            getch();
            CloseHandle(hNamedPipe);
            return 0;
            break;

        default:
            fprintf(stdout, "ConnectNamedPipe: Error %ld\n", GetLastError());
            getch();
            CloseHandle(hNamedPipe);
            return 0;
            break;
        }

        CloseHandle(hNamedPipe);
        getch();
        return 0;
    }

    // ������� ��������� �� �������� �������� ������
    fprintf(stdout, "\nConnected. Waiting for command...\n");

    // ���� ��������� ������ ����� �����
    while (1) {
        // �������� ��������� ������� ����� ����� Pipe
        if (ReadFile(hNamedPipe, szBuf, 512, &cbRead, NULL)) {
            // �������� ��� ������� ������� �����������
            // ����������
	    printf(">");
            gets(szBuf2);
            if (!WriteFile(hNamedPipe, szBuf2, strlen(szBuf) + 1,&cbWritten, NULL)) break;

            //if (!WriteFile(hNamedPipe, szBuf, strlen(szBuf) + 1,
            //       &cbWritten, NULL))
            //    break;

            // ������� �������� ������� �� �������
            printf("Received: %s\n", szBuf);

            // ���� ������ ������� "exit",
            // ��������� ������ ����������
            if (!strcmp(szBuf, "exit"))
                break;
        }
        else {
            fprintf(stdout, "ReadFile: Error %ld\n", GetLastError());
            getch();
            break;
        }
    }

    CloseHandle(hNamedPipe);
    return 0;
}