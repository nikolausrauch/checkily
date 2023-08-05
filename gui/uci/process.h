#pragma once

#include "safe_queue.h"

#include <chess/board.h>
#include <chess/move.h>

#include <filesystem>
#include <string>
#include <thread>

class os_process
{
public:
    virtual ~os_process() {}

    virtual void execute(const std::filesystem::path& path) = 0;
    virtual void close(unsigned int ms = 0) = 0;
    virtual void kill() = 0;

    virtual bool send(const std::string& cmd) = 0;
    virtual bool recv(std::string& cmd) = 0;

    virtual bool alive() = 0;
};

class uci_process
{
public:
    uci_process();
    ~uci_process();

    const std::string& name() const;
    const std::string& author() const;

    bool execute(const std::filesystem::path& path);
    bool close();
    bool running();
    void print_stdout(bool value = true);

    void newgame();
    void quit();
    void position(const chess::game_board& board, const chess::game_record& moves);
    void go();
    void isready();

    bool try_bestmove(chess::move& mv);
    bool try_isready(unsigned int ms);
    bool try_uciok(unsigned int ms);

    bool uciok();

private:
    void writer();
    void reader();

    void parse_response(const std::string& msg);

private:
    std::unique_ptr<os_process> m_os_process;

    std::mutex m_isrunning_mutex;
    std::condition_variable m_running_condition;
    std::atomic_bool m_shutdown;
    std::thread m_writer;
    std::thread m_reader;

    safe_queue<std::string> m_to_send;
    std::atomic<chess::move> m_best_move;

    std::mutex m_isready_mutex;
    std::condition_variable m_isready_condition;
    std::atomic_bool m_isreadyok;

    std::mutex m_uciok_mutex;
    std::condition_variable m_uciok_condition;
    std::atomic_bool m_uciok;

    chess::game_board m_board;

    std::string m_name;
    std::string m_author;

    bool m_print_stdout;
};
