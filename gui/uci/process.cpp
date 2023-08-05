#include "process.h"

#include <chess/chess_string.h>

#include <engine/iface/iface_string.h>

#include <iostream>
#include <regex>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include "windows_process.h"
    using impl_process = windows_process;
#else
    #error "Process Communication currently only implemented for windows!"
#endif

uci_process::uci_process()
    :
      m_os_process(new impl_process()),
      m_shutdown(false),
      m_writer(&uci_process::writer, this),
      m_reader(&uci_process::reader, this),
      m_isreadyok(false),
      m_uciok(false),
      m_print_stdout(false)
{

}

uci_process::~uci_process()
{
    quit();
    m_shutdown.store(true);
    m_running_condition.notify_all();

    m_writer.join();
    m_reader.join();

    m_os_process.reset();
}

const std::string& uci_process::name() const
{
    return m_name;
}

const std::string& uci_process::author() const
{
    return m_author;
}

bool uci_process::execute(const std::filesystem::path& path)
{   
    if(running())
    {
        close();
    }

    /* early check if this makes sense */
    if(!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
    {
        return false;
    }

    m_uciok.store(false);
    m_isreadyok.store(false);

    try
    {
        m_os_process->execute(path);
    } catch (const std::runtime_error& exp)
    {
        std::cerr << exp.what() << std::endl;
        return false;
    }

    m_to_send.push("uci");

    std::lock_guard lock(m_isrunning_mutex);
    m_running_condition.notify_all();

    return true;
}

bool uci_process::close()
{
    /*
     * TODO: this is pretty ugly, but i can't seem to do it cleanly
     * -> issues with synchronized pipes in windows
    */
    quit();
    m_shutdown.store(true);
    m_running_condition.notify_all();

    m_writer.join();
    m_reader.join();

    m_os_process.reset(new impl_process());

    m_writer = std::thread(&uci_process::writer, this);
    m_reader = std::thread(&uci_process::reader, this);
    m_shutdown.store(false);
    m_isreadyok.store(false);
    m_uciok.store(false);

    return true;
}

bool uci_process::running()
{
    return m_os_process->alive();
}

void uci_process::print_stdout(bool value)
{
    m_print_stdout = value;
}

void uci_process::newgame()
{
    m_to_send.push("ucinewgame");
}

void uci_process::quit()
{
    m_to_send.push("quit");
}

void uci_process::position(const chess::game_board& board, const chess::game_record& moves)
{
    m_board = board;

    std::stringstream ss;
    ss << "position startpos";
    if(!moves.empty()) { ss << " moves "; }
    for(auto& mv : moves)
    {
        ss << chess::square_names[mv.m_move.m_from];
        ss << chess::square_names[mv.m_move.m_to];
        if(mv.m_move.m_promote != chess::not_a_piece)
        {
            ss << chess::piece_names[mv.m_move.m_promote];
        }
        ss << " ";
    }

    m_to_send.push(ss.str());
}

void uci_process::go()
{
    m_to_send.push("go depth 6");
}

bool uci_process::try_bestmove(chess::move& mv)
{
    mv = m_best_move.exchange(chess::illegal_move);
    return mv != chess::illegal_move;
}

bool uci_process::try_isready(unsigned int ms)
{
    m_isreadyok.store(false);
    m_to_send.push("isready");

    std::unique_lock<std::mutex> lock(m_isready_mutex);
    m_isready_condition.wait_for(lock, std::chrono::milliseconds(ms),[&]()
    { return m_isreadyok.load(); });

    return m_isreadyok.load();
}

bool uci_process::try_uciok(unsigned int ms)
{
    std::unique_lock<std::mutex> lock(m_uciok_mutex);
    m_uciok_condition.wait_for(lock, std::chrono::milliseconds(ms),[&]()
    { return m_uciok.load(); });

    return m_uciok.load();
}

bool uci_process::uciok()
{
    return m_uciok.load();
}

void uci_process::writer()
{
    do
    {
        {
            std::unique_lock lock(m_isrunning_mutex);
            m_running_condition.wait(lock, [&]() { return m_shutdown.load() || m_os_process->alive(); });
        }

        auto msg = m_to_send.pop();
        m_os_process->send(msg);
        if(m_print_stdout) { std::cout << msg << std::endl; }

    } while(!m_shutdown.load());
}

void uci_process::reader()
{
    do
    {

        {
            std::unique_lock lock(m_isrunning_mutex);
            m_running_condition.wait(lock, [&]() { return m_shutdown || m_os_process->alive(); });
        }

        std::string msg;
        if(m_os_process->recv(msg))
        {
            if(m_print_stdout) { std::cout << msg << std::endl; }
            parse_response(msg);
        }

    } while(!m_shutdown.load());
}

namespace detail
{

std::vector<std::string> tokenize(const std::string &t_input)
{
    const std::regex pattern("\\S+");
    std::vector<std::string> tokens(std::sregex_token_iterator(t_input.begin(), t_input.end(), pattern),
                                    std::sregex_token_iterator());

    return tokens;
}

chess::square to_square(char file, char rank)
{
    auto file_ = file - 'a';
    auto rank_ = rank - '1';

    if(0 > file_ || file_ > 7 || 0 > rank_ || rank_ > 7) {  return chess::square::not_a_square; }

    return static_cast<chess::square>( (7-rank_)*8 + file_);
}

}

void uci_process::parse_response(const std::string& msg)
{
    std::istringstream iss(msg);

    std::string input;
    while (std::getline(iss, input))
    {
        auto tokens = detail::tokenize(input);
        if(tokens.empty()) continue;

        if(tokens[0] == "bestmove" && tokens.size() >= 2)
        {
            auto best_move = ckly::iface::parse_move(tokens[1], m_board);
            m_best_move.store(best_move);
        }

        if(tokens[0] == "id" && tokens.size() >= 3)
        {
            std::string name = tokens[2];
            for(unsigned int i = 3; i < tokens.size(); i++)
            {
                name += " " + tokens[i];
            }

            if(tokens[1] == "name")
            {
                m_name = name;
            }
            else if(tokens[1] == "author")
            {
                m_author = name;
            }
        }

        if(tokens[0] == "uciok")
        {
            m_uciok.store(true);
            m_uciok_condition.notify_all();
        }

        if(tokens[0] == "readyok")
        {
            m_isreadyok.store(true);
            m_isready_condition.notify_all();
        }
    }
}
