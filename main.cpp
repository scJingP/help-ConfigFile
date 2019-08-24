#include <stdio.h>

#include <string>
#include <list>
#include <iostream>
using namespace std;

#include "ConfigFile.h"

int main(){
	/*
	CConfigFile config;
	printf("config : %p",&config);
	char resStr[100];	//使用char数组
	char *resTest = new char[100];	//使用动态创建char数组
	
	printf("------------- resStr size : %d\n", sizeof(resTest));
	//"config.txt"
	if(config.Open("config.txt") == 0){
		printf("open success\n");
	}
	short i = config.GetValue("admin", "ipAddress", resTest);
	if(i == 0){
		printf("ipAddress : %s\n",resTest);
	}
	
	delete resTest;
	*/
	
	list<string> lst;
	CConfigFile config("config.txt");
	short i = config.GetValueForSession("date", lst);
	if(i == 0){
		list<string>::iterator itor;
		for (itor = lst.begin(); itor != lst.end(); itor++) {
			string tmp = string(*itor);
			printf("%s\n", tmp.c_str());
		}
	}else{
		printf("not success\n");
	}
	
	return 0;
}