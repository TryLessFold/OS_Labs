#include <windows.h>
#include <stdio.h>
#include <conio.h>

DWORD main(int argc, char* argv[])
{
    // ������������� ������ Pipe
    HANDLE hNamedPipe;

    // ���������� ����, ���������� ����� �����
    DWORD cbWritten;

    // ���������� ����, �������� ����� �����
    DWORD cbRead;

    // ����� ��� �������� ������
    char szBuf[256];

    // ����� ��� ����� ������ Pipe
    char szPipeName[256];

    printf("Named pipe client demo\n");

    printf("Syntax: pipec [servername]\n");

    // ���� ��� ������� PIPEC ���� ������� ��� ������,
    // ��������� ��� � ����� ������ Pipe
    if (argc > 1)
        sprintf(szPipeName, "\\\\%s\\pipe\\$MyPipe$",
            argv[1]);

    // ���� ��� ������� ������ �� ����, ������� �����
    // � ��������� ���������
    else
        strcpy(szPipeName, "\\\\.\\pipe\\$MyPipe$");

    // ������� ����� � ��������� PIPES
    hNamedPipe = CreateFile(
        szPipeName, GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL);

    // ���� �������� ������, ������� �� ��� �
    // ��������� ������ ����������
    if (hNamedPipe == INVALID_HANDLE_VALUE) {
        fprintf(stdout, "CreateFile: Error %ld\n",
            GetLastError());
        getch();
        return 0;
    }

    // ������� ��������� � �������� ������
    fprintf(stdout, "\nConnected. Type 'exit' to terminate\n");

    // ���� ������ ������� � ��������� ���������
    while (1) {
        // ������� ����������� ��� ����� �������
        printf(">");

        // ������ ��������� ������
        gets(szBuf);

        // �������� ��������� ������ ���������� ��������
        // � �������� �������
        if (!WriteFile(hNamedPipe, szBuf, strlen(szBuf) + 1,
                &cbWritten, NULL))
            break;

        // �������� ��� �� ������� ������� �� �������
        if (ReadFile(hNamedPipe, szBuf, 512, &cbRead, NULL))
            printf("Received: %s\n", szBuf);

        // ���� ��������� ������, ������� �� ��� �
        // ��������� ������ ����������
        else {
            fprintf(stdout, "ReadFile: Error %ld\n",
                GetLastError());
            getch();
            break;
        }

        // � ����� �� ������� "exit" ��������� ����
        // ������ ������� � ��������� ���������
        if (!strcmp(szBuf, "exit"))
            break;
    }

    // ��������� ������������� ������
    CloseHandle(hNamedPipe);
    return 0;
}