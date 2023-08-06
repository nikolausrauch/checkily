#include "windows_process.h"

#include <stdexcept>
#include <iostream>
#include <cstdio>

windows_process::~windows_process()
{
    if(alive()) { kill(); }
}

void windows_process::execute(const std::filesystem::path& path)
{
    if(alive()) { kill(); }

    HANDLE std_in_read{nullptr};
    HANDLE std_out_write{nullptr};

    SECURITY_ATTRIBUTES sec_attrib =
    {
        .nLength = sizeof(SECURITY_ATTRIBUTES),
        .lpSecurityDescriptor = nullptr,
        .bInheritHandle = true
    };

    if (    !CreatePipe(&m_std_out_read, &std_out_write, &sec_attrib, 0) ||
            !CreatePipe(&std_in_read, &m_std_in_write, &sec_attrib, 0))
    {
        throw std::runtime_error("Failed to create in / out pipes.");
    }

    STARTUPINFO start_info;
    ZeroMemory(&start_info, sizeof(STARTUPINFO));
    start_info.cb = sizeof(start_info);
    start_info.dwFlags = STARTF_USESTDHANDLES;
    start_info.hStdOutput = std_out_write;
    start_info.hStdInput = std_in_read;

    ZeroMemory(&m_proc_info, sizeof(PROCESS_INFORMATION));

    if(!CreateProcess( const_cast<LPSTR>(path.string().c_str()), const_cast<LPSTR>(""),
                       nullptr, nullptr, true, 0, nullptr, nullptr,
                       &start_info, &m_proc_info))
    {
        throw std::runtime_error("Failed to create process: " + path.string());
    }

    /* close not needed pipes */
    CloseHandle(std_out_write);
    CloseHandle(std_in_read);
}

void windows_process::close(unsigned int ms)
{
    if(!alive()) { return; }

    CloseHandle(m_std_out_read);
    CloseHandle(m_std_in_write);
    m_std_out_read = nullptr;
    m_std_in_write = nullptr;

    DWORD result = WaitForSingleObject(m_proc_info.hProcess, ms);

    if(result == WAIT_OBJECT_0)
    {
        CloseHandle(m_proc_info.hProcess);
        CloseHandle(m_proc_info.hThread);
    }
    else if(result == WAIT_TIMEOUT)
    {
        kill();
    }
    else if(result == WAIT_FAILED)
    {
        throw std::runtime_error("Unexpected wait result on quit");
    }
}

void windows_process::kill()
{
    if(m_std_out_read) { CloseHandle(m_std_out_read); }
    if(m_std_in_write) { CloseHandle(m_std_in_write); }

    if(!TerminateProcess(m_proc_info.hProcess, 0))
    {
        std::cerr << "Couldn't terminate process " << m_proc_info.dwProcessId << std::endl;
    }
    WaitForSingleObject(m_proc_info.hProcess, INFINITE);
}

bool windows_process::send(const std::string& msg)
{
    DWORD bytes_written{0};
    auto cmd_enter = msg + '\n';
    return WriteFile(m_std_in_write, cmd_enter.c_str(), cmd_enter.size(), &bytes_written, nullptr);
}

bool windows_process::recv(std::string& msg)
{
    if(!alive()) { return false; }

    msg.clear();
    msg.resize(256);
    DWORD bytes_read{0};
    return ReadFile(m_std_out_read, msg.data(), msg.size(), &bytes_read, nullptr) && bytes_read > 0;
}

bool windows_process::alive()
{
    DWORD code;
    return m_proc_info.hProcess != nullptr && GetExitCodeProcess(m_proc_info.hProcess, &code) && code == STILL_ACTIVE;
}
