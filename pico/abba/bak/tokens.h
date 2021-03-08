#pragma once

enum tokens { PRINT = 257, ID };

#ifdef __cplusplus
extern "C" {
#endif

int yylex ();


#ifdef __cplusplus
}
#endif
