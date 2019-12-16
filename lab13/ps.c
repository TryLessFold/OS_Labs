#include <windows.h>
#include <stdio.h>
#include <conio.h>

int main()
{
    // Флаг успешного создания канала
    BOOL fConnected;

    // Идентификатор канала Pipe
    HANDLE hNamedPipe;

    // Имя создаваемого канала Pipe
    LPSTR lpszPipeName = "\\\\.\\pipe\\$MyPipe$";

    // Буфер для передачи данных через канал
    char szBuf[512];
    char szBuf2[256];

    // Количество байт данных, принятых через канал
    DWORD cbRead;

    // Количество байт данных, переданных через канал
    DWORD cbWritten;

    printf("Named pipe server demo\n");

    // Создаем канал Pipe, имеющий имя lpszPipeName
    hNamedPipe = CreateNamedPipe(
        lpszPipeName,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        512, 512, 5000, NULL);

    // Если возникла ошибка, выводим ее код и зваершаем
    // работу приложения
    if (hNamedPipe == INVALID_HANDLE_VALUE) {
        fprintf(stdout, "CreateNamedPipe: Error %ld\n", GetLastError());
        getch();
        return 0;
    }

    // Выводим сообщение о начале процесса создания канала
    fprintf(stdout, "Waiting for connect...\n");

    // Ожидаем соединения со стороны клиента
    fConnected = ConnectNamedPipe(hNamedPipe, NULL);

    // При возникновении ошибки выводим ее код
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

    // Выводим сообщение об успешном создании канала
    fprintf(stdout, "\nConnected. Waiting for command...\n");

    // Цикл получения команд через канал
    while (1) {
        // Получаем очередную команду через канал Pipe
        if (ReadFile(hNamedPipe, szBuf, 512, &cbRead, NULL)) {
            // Посылаем эту команду обратно клиентскому
            // приложению
	    printf(">");
            gets(szBuf2);
            if (!WriteFile(hNamedPipe, szBuf2, strlen(szBuf) + 1,&cbWritten, NULL)) break;

            //if (!WriteFile(hNamedPipe, szBuf, strlen(szBuf) + 1,
            //       &cbWritten, NULL))
            //    break;

            // Выводим принятую команду на консоль
            printf("Received: %s\n", szBuf);

            // Если пришла команда "exit",
            // завершаем работу приложения
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