/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AForm.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mszymcza <mszymcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 10:23:55 by mszymcza          #+#    #+#             */
/*   Updated: 2025/11/18 15:00:14 by mszymcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/AForm.hpp"
#include "../includes/Bureaucrat.hpp"

AForm::AForm()
{
	std::cout << NGREEN << "AForm default constructor called" << RESET << std::endl;
	_name : "";
	_is_signed = false;
	_requir_to_exec = 150;
	_requir_to_sign = 150;
}

AForm::AForm(const std::string& name, int toSign, int toExec)
{
	if (toSign > 150 || toExec > 150)
		throw GradeTooLowException();
	else if (toSign < 1 || toExec < 1)
		throw GradeTooHighException();
	std::cout << NRED << "AForm default constructor called" << RESET << std::endl;
	_name = name;
	_requir_to_sign = toSign;
	_requir_to_exec = toExec;
	_is_signed = false;
}

AForm::AForm(const AForm& src)
{
	
}
