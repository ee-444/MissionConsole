
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "golab.h"

// suppress warnings in the output window
#pragma warning( disable : 4244 4996)

int GetTime(wchar_t* time_buf)
{
	time_t rawtime;
	struct tm* timeinfo;
	char* tmp_buf = (char*)calloc(256, sizeof(char));
	char* base_address = tmp_buf;
	if (tmp_buf == NULL){
		// calloc failed
		return 0;
	}
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(tmp_buf, "%s", asctime(timeinfo));
	while (*tmp_buf != NULL){
		*time_buf = *tmp_buf;
		time_buf++;
		tmp_buf++;
	}
	free(base_address);
	return 1;
}


