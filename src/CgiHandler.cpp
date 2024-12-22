/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 21:57:14 by jyap              #+#    #+#             */
/*   Updated: 2024/12/22 23:38:51 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/CgiHandler.hpp"

/* Constructor */
CgiHandler::CgiHandler()
{
	this->_cgi_pid = -1;
	this->_exit_status = 0;
	this->_cgi_path = "";
	this->_ch_env = NULL;
	this->_argv = NULL;
}

CgiHandler::CgiHandler(std::string path)
{
	this->_cgi_pid = -1;
	this->_exit_status = 0;
	this->_cgi_path = path;
	this->_ch_env = NULL;
	this->_argv = NULL;
}

CgiHandler::~CgiHandler()
{
	if (this->_ch_env)
		delete[] this->_ch_env;
	if (this->_argv)
		delete[] _argv;
	this->_env.clear();
}

CgiHandler::CgiHandler(const CgiHandler &other)
{
	this->_env = other._env;
	this->_ch_env = other._ch_env;
	this->_argv = other._argv;
	this->_cgi_path = other._cgi_path;
	this->_cgi_pid = other._cgi_pid;
	this->_exit_status = other._exit_status;
}

CgiHandler &CgiHandler::operator=(const CgiHandler &rhs)
{
	if (this != &rhs)
	{
		this->_env = rhs._env;
		this->_ch_env = rhs._ch_env;
		this->_argv = rhs._argv;
		this->_cgi_path = rhs._cgi_path;
		this->_cgi_pid = rhs._cgi_pid;
		this->_exit_status = rhs._exit_status;
	}
	return (*this);
}

/*Set functions */

void CgiHandler::setCgiPath(const std::string &cgi_path)
{
	this->_cgi_path = cgi_path;
}

const pid_t &CgiHandler::getCgiPid() const
{
	return (this->_cgi_pid);
}

const std::string &CgiHandler::getCgiPath() const
{
	return (this->_cgi_path);
}

//prepares environment variables to be passed to the child CGI process for execution
void CgiHandler::initEnvCgi(HttpRequest& req, const std::vector<Location>::iterator it_loc)
{
	std::string	cgi_exec = "cgi-bin/" + it_loc->getCgiPath()[0]; //get path to the CGI executable
	char		*cwd = getcwd(NULL, 0);
	if(_cgi_path[0] != '/') //Ensure cgi_path is an absolute path
	{
		std::string tmp(cwd);
		tmp.append("/");
		if(_cgi_path.length() > 0)
			_cgi_path.insert(0, tmp);
	}
	if(req.getMethod() == POST) //Handle POST request
	{
		std::stringstream out;
		out << req.getBody().length();
		this->_env["CONTENT_LENGTH"] = out.str();
		this->_env["CONTENT_TYPE"] = req.getHeader("content-type");
	}
	this->_env["GATEWAY_INTERFACE"] = std::string("CGI/1.1");
	this->_env["SCRIPT_NAME"] = cgi_exec;//virtual path of the CGI script relative to the document root
	this->_env["SCRIPT_FILENAME"] = this->_cgi_path; //absolute path to CGI executable
	this->_env["PATH_INFO"] = this->_cgi_path;//Extra info after the script name
	this->_env["PATH_TRANSLATED"] = this->_cgi_path;//absolute path for PATH_INFO
	this->_env["REQUEST_URI"] = this->_cgi_path;//full URI of the request, includes path and any query parameters
	this->_env["SERVER_NAME"] = req.getHeader("host");
	this->_env["SERVER_PORT"] ="8002";
	this->_env["REQUEST_METHOD"] = req.getMethodStr();
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["REDIRECT_STATUS"] = "200";
	this->_env["SERVER_SOFTWARE"] = "AMANIX";

	std::map<std::string, std::string> request_headers = req.getHeaders();
	for(std::map<std::string, std::string>::iterator it = request_headers.begin();
		it != request_headers.end(); ++it)
	{
		//Convert the header names to uppercase and prepends HTTP_
		std::string name = it->first;
		//transform(begin_it, end_it, destination_it, transformation)
		std::transform(name.begin(), name.end(), name.begin(), ::toupper);
		std::string key = "HTTP_" + name;
		_env[key] = it->second;
	}
	//convert environment map to array
	this->_ch_env = new char*[this->_env.size() + 1];
	for (size_t i = 0; i < this->_env.size() + 1; ++i)
		this->_ch_env[i] = NULL;
	std::map<std::string, std::string>::const_iterator it = this->_env.begin();
	for (int i = 0; it != this->_env.end(); it++, i++)
	{
		std::string tmp = it->first + "=" + it->second;
		this->_ch_env[i] = strdup(tmp.c_str());
	}
	//argv for execve
	this->_argv = new char*[3]; //cgi_exec, _cgi_path and NULL terminator
	this->_argv[0] = strdup(cgi_exec.c_str());
	this->_argv[1] = strdup(this->_cgi_path.c_str());
	this->_argv[2] = NULL;
}


/* initialization environment variable */
void CgiHandler::initEnv(HttpRequest& req, const std::vector<Location>::iterator it_loc)
{
	int			poz;
	std::string extension;
	std::string ext_path;

	extension = this->_cgi_path.substr(this->_cgi_path.find(".")); //get file extension
	std::map<std::string, std::string>::iterator it_path = it_loc->_ext_path.find(extension);
	if (it_path == it_loc->_ext_path.end())
		return ;
	ext_path = it_loc->_ext_path[extension]; //get path to extension
	this->_env["AUTH_TYPE"] = "Basic";
	this->_env["CONTENT_LENGTH"] = req.getHeader("content-length");
	this->_env["CONTENT_TYPE"] = req.getHeader("content-type");
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	poz = findStart(this->_cgi_path, "cgi-bin/");
	this->_env["SCRIPT_NAME"] = this->_cgi_path;
	this->_env["SCRIPT_FILENAME"] = ((poz < 0 || (size_t)(poz + 8) > this->_cgi_path.size()) ? "" : this->_cgi_path.substr(poz + 8, this->_cgi_path.size()));
	// check there are characters after cgi-bin/ for substr function
	this->_env["PATH_INFO"] = getPathInfo(req.getPath(), it_loc->getCgiExtension());
	this->_env["PATH_TRANSLATED"] = it_loc->getRootLocation() + (this->_env["PATH_INFO"] == "" ? "/" : this->_env["PATH_INFO"]);
	this->_env["QUERY_STRING"] = decode(req.getQuery());
	this->_env["REMOTE_ADDR"] = req.getHeader("host");
	poz = findStart(req.getHeader("host"), ":");
	this->_env["SERVER_NAME"] = (poz > 0 ? req.getHeader("host").substr(0, poz) : "");
	this->_env["SERVER_PORT"] = (poz > 0 ? req.getHeader("host").substr(poz + 1, req.getHeader("host").size()) : "");
	this->_env["REQUEST_METHOD"] = req.getMethodStr();
	this->_env["HTTP_COOKIE"] = req.getHeader("cookie");
	this->_env["DOCUMENT_ROOT"] = it_loc->getRootLocation();
	this->_env["REQUEST_URI"] = req.getPath() + req.getQuery();
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["REDIRECT_STATUS"] = "200";
	this->_env["SERVER_SOFTWARE"] = "AMANIX";

	this->_ch_env = new char*[this->_env.size() + 1];
	std::map<std::string, std::string>::const_iterator it = this->_env.begin();
	for (int i = 0; it != this->_env.end(); it++, i++)
	{
		std::string tmp = it->first + "=" + it->second;
		this->_ch_env[i] = strdup(tmp.c_str());
	}
	this->_argv = new char*[3];
	this->_argv[0] = strdup(ext_path.c_str());
	this->_argv[1] = strdup(this->_cgi_path.c_str());
	this->_argv[2] = NULL;
}

/* Pipe and execute CGI */
void CgiHandler::execute(short &error_code)
{
	//checks executable path and CGI script path
	if (this->_argv[0] == NULL || this->_argv[1] == NULL)
	{
		error_code = 500;
		return ;
	}
	if (pipe(pipe_in) < 0)
	{
		Logger::logMsg(RED, "pipe() failed");
		error_code = 500;
		return ;
	}
	if (pipe(pipe_out) < 0)
	{
		Logger::logMsg(RED, "pipe() failed");
		close(pipe_in[0]);
		close(pipe_in[1]);
		error_code = 500;
		return ;
	}
	this->_cgi_pid = fork();
	if (this->_cgi_pid == 0) //child process
	{
		dup2(pipe_in[0], STDIN_FILENO);
		dup2(pipe_out[1], STDOUT_FILENO);
		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);
		this->_exit_status = execve(this->_argv[0], this->_argv, this->_ch_env);
		exit(this->_exit_status);
	}
	else if (this->_cgi_pid > 0){}
	else
	{
		std::cout << "Fork failed" << std::endl;
		error_code = 500;
	}
}

int CgiHandler::findStart(const std::string path, const std::string delim)
{
	if (path.empty())
		return (-1);
	size_t poz = path.find(delim);
	if (poz != std::string::npos)
		return (poz);
	else
		return (-1);
}

/* Translation of parameters for QUERY_STRING environment variable */
std::string CgiHandler::decode(std::string &path)
{
	size_t token = path.find("%");
	//finds % symbol and convert the next two characters from HextoDec
	while (token != std::string::npos)
	{
		if (path.length() < token + 2)
			break ;
		char decimal = fromHexToDec(path.substr(token + 1, 2));
		path.replace(token, 3, toString(decimal));
		token = path.find("%");
	}
	return (path);
}

/* Isolation PATH_INFO environment variable */
//path = "/cgi-bin/script.py/foo/bar?key=value";
//checks for extension like.py and also / symbol after .py
//if found, it will return the info but before the ? symbol, so returns /foo/bar
std::string CgiHandler::getPathInfo(std::string& path, std::vector<std::string> extensions)
{
	std::string tmp;
	size_t start, end;

	for (std::vector<std::string>::iterator it_ext = extensions.begin(); it_ext != extensions.end(); it_ext++)
	{
		start = path.find(*it_ext);
		if (start != std::string::npos)
			break ;
	}
	if (start == std::string::npos)
		return "";
	if (start + 3 >= path.size())
		return "";
	tmp = path.substr(start + 3, path.size());
	if (!tmp[0] || tmp[0] != '/')
		return "";
	end = tmp.find("?");
	return (end == std::string::npos ? tmp : tmp.substr(0, end));
}

void	CgiHandler::clear()
{
	this->_cgi_pid = -1;
	this->_exit_status = 0;
	this->_cgi_path = "";
	this->_ch_env = NULL;
	this->_argv = NULL;
	this->_env.clear();
}
