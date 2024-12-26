/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPRange.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 22:33:02 by jyap              #+#    #+#             */
/*   Updated: 2024/12/26 14:43:40 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPRANGE_HPP
#define IPRANGE_HPP

#include "Webserv.hpp"

class IPRange
{
	private:
		std::string ip; // The IP address or network address
		int prefix; // The subnet prefix (e.g., /24)
		uint32_t ip_start;
		uint32_t ip_end;

	public:
		// Constructor
		IPRange(const std::string& ip_addr);
		// Helper function to validate IP format

		// Getters
		std::string getIP() const;
		int getPrefix() const;

		// Setters
		void setIP(const std::string& ip_addr);
		void setPrefix(int prefix_length);

		// Check if a given IP is within this range (placeholder for actual implementation)
		bool contains(const std::string& target_ip) const;

		uint32_t ipToInt(const std::string &ip) const;
		void cidrToRange();
};

#endif
