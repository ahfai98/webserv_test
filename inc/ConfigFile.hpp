/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:40:38 by jyap              #+#    #+#             */
/*   Updated: 2024/12/25 20:29:24 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "Webserv.hpp"

enum PathType
{
	IS_FILE = 1,
	IS_DIRECTORY = 2,
	IS_OTHER = 3,
	STATFAIL = -1
};

class ConfigFile
{
	private:
		std::string		_path;

	public:
		ConfigFile();
		ConfigFile(const std::string &path);
		~ConfigFile();

		static PathType getPathType(const std::string &path);
		static int checkFile(const std::string &path, int mode);
		std::string	readFile(const std::string &path);
		static int fileExistReadable(const std::string &path, const std::string &index);

		std::string getPath() const;
};

#endif
