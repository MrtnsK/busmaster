/*
-----------------------------------------------------------
 CANPCEX.H  Copyright (c) 2003 NSI (Annecy France)
-----------------------------------------------------------
Author :   CRO
Product :  CANEX Interface
File    :  Main include file for user applications
Target  :  Windows 32 DLL
History :
  19/02/2003 : First release candidate
Comments :
-----------------------------------------------------------
*/

#ifndef _CANEXINTFCDLL
/*
    The symbol _CANAPI defines the calling convention for
    the DLL functions. It is used in CANPROEX.H. Make sure
    it is compatible with the currently used compiler.
    The following syntax is Microsoft specific.

    Le symbole _CANAPI definit la convention d'appel des
    fonctions de la DLL. Il est utilise dans CANPROEX.H. Il
    faut s'assurer que la syntaxe suivante qui est
    sp�cifique aux outils Microsoft est correctement
    interpretee par le compilateur utilis�.
*/
#define _CANAPI __declspec(dllimport) __stdcall
/*
    BORLAND :
    #define _CANAPI _import _stdcall
*/
#else
#define _CANAPI __declspec(dllexport) __stdcall
#endif

/*
   Structures and data definitions
   D�finition des structures et des donn�es
*/
#include "candefex.h"

/*
   Functions prototypes declarations
   D�claration des prototypes des fonctions
*/
#include "canproex.h"

