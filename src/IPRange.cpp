/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPRange.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 22:39:03 by jyap              #+#    #+#             */
/*   Updated: 2024/12/26 19:16:41 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IPRange.hpp"
#include <stdexcept> 

// Constructor
IPRange::IPRange(const std::string& ip_addr)
{
	// Validate IP address format
	if (!isValidIP(ip_addr))
		throw std::runtime_error("Invalid IP address format");

	size_t slash_pos = ip_addr.find('/');
	if (slash_pos != std::string::npos)
	{
		// If there's a '/', separate the IP address and prefix
		std::string ip_part = ip_addr.substr(0, slash_pos);
		std::string prefix_part = ip_addr.substr(slash_pos + 1);
		ip = ip_part;
		prefix = ft_stoi(prefix_part.c_str());
	}
	else
	{
		ip = ip_addr;
		prefix = 32;
	}
	cidrToRange();
}

// Getters
std::string IPRange::getIP() const
{ 
	return (ip);
}

int IPRange::getPrefix() const
{
	return (prefix);
}

// Setters
void IPRange::setIP(const std::string& ip_addr)
{
	if (!isValidIP(ip_addr))
		throw std::invalid_argument("Invalid IP address format");
	ip = ip_addr;
}

void IPRange::setPrefix(int prefix_length)
{
	if (prefix_length < 0 || prefix_length > 32)
		throw std::invalid_argument("Invalid prefix length (must be between 0 and 32)");
	prefix = prefix_length;
}

// Check if a given IP is within this range
bool IPRange::contains(const std::string& target_ip) const
{
	if (!isValidIP(target_ip))
	{
		throw std::invalid_argument("Invalid IP address format");
		return (false);
	}
	uint32_t target_ip_int = ipToInt(target_ip);
	return (target_ip_int >= ip_start && target_ip_int <= ip_end);
}

uint32_t IPRange::ipToInt(const std::string &ip) const
{
	uint32_t result = 0;
	size_t pos = 0;
	size_t last_pos = 0;
	int shift = 24;
	
	while ((pos = ip.find('.', last_pos)) != std::string::npos)
	{
		std::string segment = ip.substr(last_pos, pos - last_pos);
		result |= (ft_stoi(segment.c_str()) << shift);
		last_pos = pos + 1;
		shift -= 8;
	}
	result |= ft_stoi(ip.substr(last_pos).c_str());
	return (result);
}

void IPRange::cidrToRange()
{
	uint32_t ip_binary = ipToInt(this->ip);
	uint32_t mask = (0xFFFFFFFF << (32 - this->prefix)) & 0xFFFFFFFF;
	ip_start = ip_binary & mask;
	ip_end = ip_start | (~mask);
}
