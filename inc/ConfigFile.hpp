/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:40:38 by jyap              #+#    #+#             */
/*   Updated: 2024/12/16 20:26:45 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "Webserv.hpp"

class ConfigFile
{
	private:
		std::string		_path;
		size_t			_size;

	public:
		ConfigFile();
		ConfigFile(std::string const path);
		~ConfigFile();

		static int getTypePath(std::string const path);
		static int checkFile(std::string const path, int mode);
		std::string	readFile(std::string path);
		static int fileExistReadable(std::string const path, std::string const index);

		std::string getPath();
};

#endif
