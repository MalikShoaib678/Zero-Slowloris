# ZeroSlowloris

## Introduction

ZeroSlowloris is a powerful Denial-of-Service (DoS) tool written in C++, inspired by the popular Python Slowloris tool. It introduces multithreading and customization options, making it a more efficient and versatile tool for conducting DoS attacks.

## Features

* **Multithreading:** ZeroSlowloris utilizes multiple threads to establish connections, significantly improving the speed and efficiency of the attack.
* **Customization:** With ZeroSlowloris, you have control over various parameters to tailor the attack according to your needs:
    * **Target:** The IP address or hostname of the target.
    * **Port:** The target port number.
    * **Max_socks:** The maximum number of sockets to open.
    * **Threads:** The number of threads to use.
    * **Min_delay:** [Optional] The minimum delay between each character (default:5ms).
    * **Max_delay:** [Optional] The maximum delay between each character (default:10ms).
    * **Delay_keep_alive:** [Optional] The delay between sending Keep-Alive headers (default:1 second).
    * **Delay_print_details:** [Optional] The delay for printing attack details (default:10 seconds).
* **Random User-Agent:** Each request sent by ZeroSlowloris includes a randomly generated User-Agent header, making it more challenging to detect and mitigate the attack.
* **Proxies (Coming Soon):** In future updates, ZeroSlowloris will include a proxy feature, allowing you to use multiple proxies simultaneously, further enhancing anonymity and making detection even harder.

## Usage

To use ZeroSlowloris, follow the instructions below:

```bash
Usage: Slowloris target port max_socks threads [options]

Options:
  -h, --help            Show help message and exit

Input Information:
  target        : The target IP address or hostname
  port          : The target port number
  max_socks     : The maximum number of sockets to open
  threads       : The number of threads to use
  min_delay     : [Optional] The minimum delay between each character (default:5ms)
  max_delay     : [Optional] The maximum delay between each character (default:10ms)
  delay_keep_alive : [Optional] The delay between sending Keep-Alive headers (default:1 second)
  delay_print_details : [Optional] The delay for printing attack details (default:10 seconds)

Author: MalikShoaib
```

## Performance Comparison

We have conducted performance tests comparing ZeroSlowloris with the original Python Slowloris tool. The results were impressive:

* **Python Slowloris Tool:** It took approximately 5 minutes to establish 800 connections to the target.
* **ZeroSlowloris:** With its multithreading capabilities, ZeroSlowloris achieved the same result in just 16 seconds.

## Disclaimer

Please note that the use of ZeroSlowloris or any other DoS tool for malicious purposes is strictly prohibited. This tool is intended for educational and testing purposes only. The author and contributors are not responsible for any misuse or damage caused by this tool.

## Get Involved

If you are interested in contributing to ZeroSlowloris or have any suggestions, feel free to reach out. Together, let's make it an even more powerful tool!

🔗 [GitHub Repository](https://github.com/MalikShoaib678/Zero-Slowloris)

✉️ Contact: [shoaib688malik@gmail.com](mailto:shoaib688malik@gmail.com)

🌟 Star the project and show your support!
