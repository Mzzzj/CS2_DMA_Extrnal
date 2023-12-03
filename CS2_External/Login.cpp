#include "Login.hpp"

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}
std::string GetGuid() {
    std::string sub_key = "SOFTWARE\\Microsoft\\Cryptography";
    std::string name = "MachineGuid";
    HKEY hKey;
    DWORD dwType = REG_SZ;
    DWORD dwLen = MAX_PATH;
    if (RegOpenKeyA(HKEY_LOCAL_MACHINE, sub_key.c_str(), &hKey) != ERROR_SUCCESS) {
        std::cout << "error\n";
        return "";
    }
    char buf[100];
    PLONG data = 0;
    if (RegQueryValueExA(hKey, name.c_str(), 0, &dwType, (LPBYTE)buf, &dwLen) != ERROR_SUCCESS) {
        std::cout << "error\n";
        return "";
    }
    return buf;
}
char* Ansi2Utf8(std::string content)
{
    int nLen = MultiByteToWideChar(CP_ACP, NULL, content.c_str(), content.length(), NULL, 0);
    wchar_t* wszText = new wchar_t[nLen + 1];
    nLen = MultiByteToWideChar(CP_ACP, NULL, content.c_str(), content.length(), wszText, nLen);
    wszText[nLen] = '\0';
    nLen = WideCharToMultiByte(CP_UTF8, NULL, wszText, -1, NULL, NULL, NULL, NULL);
    char* contextBuf = new char[nLen];
    nLen = WideCharToMultiByte(CP_UTF8, NULL, wszText, -1, contextBuf, nLen, NULL, NULL);
    contextBuf[nLen] = '\0';

    return contextBuf;
}

char* Utf82Ansi(const char* res)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, res, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_UTF8, 0, res, -1, wstr, len);
    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);

    return str;
}
std::string SeendHttpRequest(std::string key, std::string uuid) {
    HttpRequest httpReq("8.137.19.4", 9999);
    //HttpRequest httpReq("127.0.0.1", 9999);
    std::string res = httpReq.HttpGet("/checkKeyFull?key=" + key + "&uuid=" + uuid);
    res = Utf82Ansi(res.c_str());
    return res.substr(res.find_first_of("{"), res.find_last_of("}"));
}
bool Login::CheckKey()
{
    std::string uuid =GetGuid();
    std::string key;
    LPCTSTR lpFileName = ".\\config.ini";
    LPSTR Lkey = new char[MAX_PATH];
    GetPrivateProfileString("config", "key", "", Lkey, MAX_PATH, lpFileName);
    if (Lkey!=NULL&& Lkey!="") {
        key = Lkey;
    }
GetKey:
    //F501366883DD4F51
    if (key == "") {
        std::cout << "ÇëÊäÈëÃÜ³×(Please input your Key): " << std::endl;
        std::getline(std::cin, key);
    }

    std::string res = SeendHttpRequest(key, uuid).c_str();
    if (res=="") {
        goto GetKey;
    }
    Json::Reader reader;
    Json::Value value;
  
    if (reader.parse(res, value, false))
    {
        bool keyIsOk = value["keyIsOk"].asBool();
        std::string msg = value["msg"].asString();
        std::cout << msg << std::endl;
        std::string data = value["data"].asString();
        Json::Value valueData;
        if (keyIsOk) {
            if (reader.parse(data, valueData, false))
            {
                Offset::EntityList = stoi(valueData["EntityList"].asString());
                Offset::Matrix = stoi(valueData["Matrix"].asString());
                Offset::ViewAngle = stoi(valueData["ViewAngle"].asString());
                Offset::LocalPlayerController = stoi(valueData["LocalPlayerController"].asString());
                Offset::LocalPlayerPawn = stoi(valueData["LocalPlayerPawn"].asString());
                Offset::ForceJump = stoi(valueData["ForceJump"].asString());
                Offset::GlobalVars = stoi(valueData["GlobalVars"].asString());

                remove(lpFileName);
                WritePrivateProfileString("config", "key", key.c_str(), lpFileName);
                return true;
            }
            else {
                key = "";
                goto GetKey;
            }
        }else {
            key = "";
            goto GetKey;
        }
    }
    else {
        return false;
    }
}





