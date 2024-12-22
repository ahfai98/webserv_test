/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 22:51:47 by jyap              #+#    #+#             */
/*   Updated: 2024/12/21 17:49:05 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "Webserv.hpp"

class Location;

class ServerConfig
{
	private:
		uint16_t						_port; //port number the server listens to
		in_addr_t						_host; //IP address of server's host in IPv4
		std::string						_server_name; //domain name
		std::string						_root; //Root directory of the server
		unsigned long					_client_max_body_size; // max size for http request body
		std::string						_index; //index page
		bool							_autoindex; //if true, will display directory listing, else will display error page
		std::map<short, std::string>	_error_pages; //map status codes to custom error pages
		std::vector<Location> 			_locations; //List of Location objects
		struct sockaddr_in 				_server_address; //struct that contains serer's IP address and port
		//Includes: sin_family(AF_INET), sin_port, sin_addr
		int								_listen_fd; //fd for server's listening socket

	public:
		ServerConfig();
		~ServerConfig();
		ServerConfig(const ServerConfig &other);
		ServerConfig &operator=(const ServerConfig & rhs);

		void initErrorPages(void);
		void setServerName(std::string server_name);
		void setHost(std::string parameter);
		void setRoot(std::string root);
		void setFd(int);
		void setPort(std::string parameter);
		void setClientMaxBodySize(std::string parameter);
		void setErrorPages(std::vector<std::string> &parameter);
		void setIndex(std::string index);
		void setLocation(std::string nameLocation, std::vector<std::string> parameter);
		void setAutoindex(std::string autoindex);
		bool isValidHost(std::string host) const;
		bool isValidErrorPages();
		int isValidLocation(Location &location) const;
		const std::string &getServerName();
		const uint16_t &getPort();
		const in_addr_t &getHost();
		const size_t &getClientMaxBodySize();
		const std::vector<Location> &getLocations();
		const std::string &getRoot();
		const std::map<short, std::string> &getErrorPages();
		const std::string &getIndex();
		const bool &getAutoindex();
		const std::string &getPathErrorPage(short key);
		const std::vector<Location>::iterator getLocationKey(std::string key);
		static void checkToken(std::string &parameter);
		bool checkLocationsDup() const;

		class ErrorException : public std::exception
		{
			private:
				std::string _message;
			public:
				ErrorException(std::string message) throw()
				{
					_message = "SERVER CONFIG ERROR: " + message;
				}
				virtual const char* what() const throw()
				{
					return (_message.c_str());
				}
				virtual ~ErrorException() throw() {}
		};

		void	setupServer();
		int		getFd();
};

#endif
