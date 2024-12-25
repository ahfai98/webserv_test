/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:44:39 by jyap              #+#    #+#             */
/*   Updated: 2024/12/25 20:29:24 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ConfigFile.hpp"

ConfigFile::ConfigFile(){}

ConfigFile::ConfigFile(const std::string &path): _path(path) {}

ConfigFile::~ConfigFile(){}

// Check path is file, directory or other
PathType ConfigFile::getPathType(const std::string &path)
{
	struct stat	file_stat;

	if (stat(path.c_str(), &file_stat) == 0)
	{
		if (file_stat.st_mode & S_IFREG) // File
			return (IS_FILE);
		else if (file_stat.st_mode & S_IFDIR) // Directory
			return (IS_DIRECTORY);
		else // Other types (symlink, socket, etc.)
			return (IS_OTHER);
	}
	return (STATFAIL); // stat failed
}

// Check file is accessible with the mode (F_OK, R_OK, W_OK, X_OK)
//Mode: File exist, read, write, execute permissions
//Returns 0 if accessible, else -1
int	ConfigFile::checkFile(const std::string &path, int mode)
{
	return (access(path.c_str(), mode));
}

//Check file exists and is readable for two locations(index or path+index)
int ConfigFile::fileExistReadable(const std::string &path, const std::string &index)
{
	std::string fullPath1 = index;
	std::string fullPath2 = path + index;
	if ((getPathType(fullPath1) == IS_FILE && checkFile(fullPath1, R_OK) == 0) ||
		(getPathType(fullPath2) == IS_FILE && checkFile(fullPath2, R_OK) == 0))
		return (0);
	return (-1);
}

//Read file contents into string
std::string ConfigFile::readFile(const std::string &path)
{
	if (path.empty())
		return (NULL);
	// Open the file in read mode
	std::ifstream config_file(path.c_str());
	if (!config_file) 
		return (NULL);
	// Read the file into a string
	std::stringstream ss;
	ss << config_file.rdbuf();
	return (ss.str());
}

/* Get functions */
std::string ConfigFile::getPath() const
{
	return (this->_path);
}
