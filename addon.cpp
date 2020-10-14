#include <napi.h>
#include <windows.h>

#include <psapi.h>
#include <iostream>
#include <vector>
#include <regex>
#include <algorithm>

using namespace Napi;

const char* stringScan(const char* windowTitle, const char* cstr_regexString);

Value scan(const CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::string windowTitle = info[0].ToString().Utf8Value();
    std::string regexString = info[1].ToString().Utf8Value();
    return String::New(env,
                       stringScan(
                           windowTitle.c_str(),
                           regexString.c_str()));
}

Napi::Object Init(Napi::Env env, Object exports) {
    exports.Set("scan", Function::New(env, scan));
    return exports;
}

const char* stringScan(const char* windowTitle, const char* cstr_regexString) {
    std::string regexString(cstr_regexString);
    std::regex regex(regexString);
    std::vector<char> buffer;
    DWORD pID;
    HWND hwnd = FindWindowA(NULL, windowTitle);
    if (!hwnd) {
        return "FindWindowA ERROR";
    }
    if (!GetWindowThreadProcessId(hwnd, &pID)) {
        return "GetWindowThreadProcessId ERROR";
    }
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (!pHandle) {
        return "OpenProcess ERROR";
    }
    unsigned char* p = NULL;
    MEMORY_BASIC_INFORMATION info;
    int idx = 0;
    for (p = NULL; VirtualQueryEx(pHandle, p, &info, sizeof(info)) == sizeof(info); p += info.RegionSize) {
        if (info.State == MEM_COMMIT && (info.Type == MEM_MAPPED || info.Type == MEM_PRIVATE)) {
            ++idx;
            SIZE_T bytes_read;
            buffer.resize(info.RegionSize);
            ReadProcessMemory(pHandle, p, &buffer[0], info.RegionSize, &bytes_read);
            buffer.resize(bytes_read);
            std::string string = std::string(buffer.data(), buffer.size());
            string.erase(
                remove_if(
                    string.begin(), string.end(), [](char c) {
                        return !(c >= 32 && c < 126);
                    }),
                string.end());
            auto ids_begin = std::sregex_iterator(string.begin(), string.end(), regex);
            auto ids_end = std::sregex_iterator();
            for (auto it = ids_begin; it != ids_end; ++it) {
                std::smatch match = *it;
                std::string MATCH = match[1].str();
                return strdup(MATCH.c_str());
            }
        }
    }
    return "MATCH NOT FOUND";
}

NODE_API_MODULE(foo, Init) // no idea what this is for