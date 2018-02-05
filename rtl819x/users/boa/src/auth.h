#ifndef _AUTH_H
#define _AUTH_H

#include <stdio.h>

#include "apmib.h"
#include "boa.h"

#ifdef SUPER_NAME_SUPPORT

#define USERS_NUMS 2

enum {
	NO_LOGIN,
	SUPER_FLAG,
	SECOND_FLAG
};

typedef struct auth_html_s 
{
	char *name;
	char read[USERS_NUMS];
	
} auth_html_t;

typedef struct auth_form_s
{
	char *name;
	char write[USERS_NUMS];
	
} auth_form_t;


int aspUserFlag(request * wp);
//enum


int getHtmlAuth(char *htmlName, int flag);

int getFormAuth(char *formName, int flag);

#endif
#endif
	
