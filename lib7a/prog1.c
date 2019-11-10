#include <windows.h>
#include <psapi.h>
#include <stdio.h>

int main() {
	int a, b;
	HINSTANCE hMyDll;

	if ((hMyDll = LoadLibrary("lib")) == NULL) { /* не удалось загрузить DLL */
		printf("Bye");
	}
	else { /* приложение имеет право пользоваться функциями DLL через hMyDll */
		char q[2];
		a = 1; b = 2;
		while (q[0] != 'q') {
			printf("Hello, i'm = %d\n", GetCurrentProcessId());
			typedef int (WINAPI *PFN_MyFunction)(int a, int b);
			PFN_MyFunction my_foo;
			my_foo = (PFN_MyFunction)GetProcAddress(hMyDll, "foo");
			int tmp = (*my_foo)(a, b);
			printf("%d\n", tmp);
			fgets(q, 2, stdin);
			b++;
			a++;
		}
		
	}
	return 0;
}
