#pragma once

#include <time.h>
#include <stdlib.h>
#include <string.h>

void RevStr(char s[]) {
  int i, j;

  for(i = 0, j = strlen(s)-1; i <= j; ++i, --j) {
    char temp = s[i];
    s[i] = s[j];
    s[j] = temp;
  }
}

int GetRandNumber(int limit) {
  return GetRandomValue(10, limit);
}

void IntToAsci(int value, char s[]) {
  int result = value / 10;
  int rem = value % 10, counter = 0;
  s[counter++] = '0' + rem;

  while(result > 0) {
    int temp = result / 10;
    rem = result % 10;
    result = temp;
    s[counter++] = '0' + rem;
  }

  s[counter] = '\0';
  RevStr(s);
}
