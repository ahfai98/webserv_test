/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:19:09 by jyap              #+#    #+#             */
/*   Updated: 2024/12/16 11:31:15 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_HPP
#define MIME_HPP

#include "Webserv.hpp"

class Mime
{
	public:
		Mime();
		static std::string toLower(const std::string &str);
		std::string getMimeType(std::string extension);
	private:
		std::map<std::string, std::string> _mime_types;
};

#endif
