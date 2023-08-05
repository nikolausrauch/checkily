#pragma once

#include "process.h"

#include <filesystem>

#include <windows.h>

class windows_process : public os_process
{
public:
    ~windows_process();

    void execute(const std::filesystem::path& path) override;
    void close(unsigned int ms) override;
    void kill() override;

    bool send(const std::string& msg) override;
    bool recv(std::string& msg) override;

    bool alive() override;

private:
    HANDLE m_std_out_read{nullptr};
    HANDLE m_std_in_write{nullptr};
    PROCESS_INFORMATION m_proc_info;
};
