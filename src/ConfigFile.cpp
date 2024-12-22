/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:44:39 by jyap              #+#    #+#             */
/*   Updated: 2024/12/16 22:34:17 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ConfigFile.hpp"

ConfigFile::ConfigFile(): _size(0){}

ConfigFile::ConfigFile(std::string const path): _path(path), _size(0){}

ConfigFile::~ConfigFile(){}

// Check path is file, directory or other
int ConfigFile::getTypePath(std::string const path)
{
	struct stat	file_stat;

	if (stat(path.c_str(), &file_stat) == 0)
	{
		if (file_stat.st_mode & S_IFREG) // File
			return (1);
		else if (file_stat.st_mode & S_IFDIR) // Directory
			return (2);
		else // Other types (symlink, socket, etc.)
			return (3);
	}
	else
	{
		return (-1); // stat failed
	}
}

// Check file is accessible with the mode (F_OK, R_OK, W_OK, X_OK)
//Mode: File exist, read, write, execute permissions
//Returns 0 if accessible, else -1
int	ConfigFile::checkFile(std::string const path, int mode)
{
	return (access(path.c_str(), mode));
}

//Check file exist and readable for two locations(index or path+index)
int ConfigFile::fileExistReadable(std::string const path, std::string const index)
{
	if (getTypePath(index) == 1 && checkFile(index, R_OK) == 0)
		return (0);
	if (getTypePath(path + index) == 1 && checkFile(path + index, R_OK) == 0)
		return (0);
	return (-1);
}

//Read file contents into string
std::string ConfigFile::readFile(std::string path)
{
	if (path.empty())
		return (NULL);
	// Open the file in read mode
	std::ifstream config_file(path.c_str());
	if (!config_file.is_open()) 
		return (NULL);
	// Read the file into a string
	std::stringstream ss;
	ss << config_file.rdbuf();
	return (ss.str());
}

/* Get functions */
std::string ConfigFile::getPath()
{
	return (this->_path);
}
