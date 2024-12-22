/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:42:54 by jyap              #+#    #+#             */
/*   Updated: 2024/12/16 12:10:04 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream> //std::cin, cout, cerr
# include <fcntl.h> //open, fcntl, O_RDONLY, O_WRONLY
# include <cstring> //strcpy, strcmp, memset
# include <string>  //std::string
# include <unistd.h> //close, read, write, fork
# include <dirent.h> //DIR structure, opendir, readdir, closedir
# include <sstream> //std::stringstream, toString
# include <cstdlib> //malloc, free, atoi
# include <fstream> //std::ifstream, std::ofstream
# include <cctype> //tolower, isdigit
# include <ctime> //time, gmtime, strftime
# include <cstdarg> //va_start, va_end

/* STL Containers */
# include <map>
# include <vector>
# include <algorithm> //std::transform
# include <iterator>

/* System */
# include <sys/types.h> //pid_t, size_t
# include <sys/wait.h> //wait, waitpid
# include <sys/stat.h> //stat
# include <sys/time.h> //timeval struct
# include <signal.h> //signal, SIGPIPE, SIGKILL

/* Network */
# include <sys/socket.h> //socket, bind, listen, accept
# include <netinet/in.h> //sockaddr_in structure
# include <sys/select.h> //select
# include <arpa/inet.h> //inet_ntoa, inet_pton

# include "ConfigParser.hpp"
# include "ConfigFile.hpp"
# include "ServerConfig.hpp"
# include "Location.hpp"
# include "HttpRequest.hpp"
# include "CgiHandler.hpp"
# include "Mime.hpp"
# include "Logger.hpp"


#define CONNECTION_TIMEOUT 60 // Time in seconds for a client to disconnect due to inactivity
#define MESSAGE_BUFFER 40000 //Size of the buffer for reading messages

#define MAX_URI_LENGTH 4096
#define MAX_CONTENT_LENGTH 30000000 //Maximum size of HTTP request content

template <typename T>
std::string toString(const T val)
{
	std::stringstream stream;
	stream << val;
	return (stream.str());
}

/* Utils.c */
std::string statusCodeString(short);
std::string getErrorPage(short);
int buildHtmlIndex(std::string &, std::vector<uint8_t> &, size_t &);
int ft_stoi(std::string str);
unsigned int fromHexToDec(const std::string& nb);

#endif
