/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 21:15:59 by jyap              #+#    #+#             */
/*   Updated: 2024/12/15 22:02:23 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Webserv.hpp"
#include "../inc/ServerManager.hpp"

// Handles SIGPIPE signals to prevent program termination.
// If a client disconnects while the server is writing data, SIGPIPE would
// terminate the server unexpectedly, leading to downtime.
void sigpipeHandle(int sig) { if(sig) {}}

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		Logger::logMsg(RED, "Error: Wrong number of arguments.");
		return (1);
	}
	try 
	{
		signal(SIGPIPE, sigpipeHandle);
		// configuration file as argument or default path
		std::string config = (argc == 1 ? "configs/default.conf" : argv[1]);
		//Parse config file for servers
		ConfigParser	cluster;
		cluster.createCluster(config);
		// cluster.print(); // for checking
		// Setup and run servers based on config info
		ServerManager 	master;
		master.setupServers(cluster.getServers());
		master.runServers();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
