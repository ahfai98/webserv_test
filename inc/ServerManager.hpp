/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 22:06:37 by jyap              #+#    #+#             */
/*   Updated: 2024/12/27 14:50:22 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include "Webserv.hpp"
#include "Client.hpp"
#include "Response.hpp"

 //ServerManager
 // setup and run servers using config file
 // establis new connections with clients and receive/send requests/responses.

class ServerManager
{
	public:
		ServerManager();
		~ServerManager();
		void	setupServers(std::vector<ServerConfig>);
		void	runServers();
		
	private:
		std::vector<ServerConfig> _servers;
		std::map<int, ServerConfig> _servers_map;
		std::map<int, Client> _clients_map;
		fd_set	_recv_fd_pool;
		fd_set	_write_fd_pool;
		int		_biggest_fd;

		void acceptNewConnection(ServerConfig &);
		void checkTimeout();
		void initializeSets();
		void readRequest(const int &, Client &);
		void handleReqBody(Client &);
		void sendResponse(const int &, Client &);
		void sendCgiBody(Client &, CgiHandler &);
		void readCgiResponse(Client &, CgiHandler &);
		void closeConnection(const int);
		void assignServer(Client &);
		void addToSet(const int , fd_set &);
		void removeFromSet(const int , fd_set &);
};

#endif // SERVERMANAGER_HPP
