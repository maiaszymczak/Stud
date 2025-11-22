/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PresidentialPardonForm.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mszymcza <mszymcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 10:22:26 by mszymcza          #+#    #+#             */
/*   Updated: 2025/11/22 12:05:56 by mszymcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRESIDENTIALPARDONFORM_HPP
#define PRESIDENTIALPARDONFORM_HPP

#include "AForm.hpp"
#include <iostream>

// Colors
#define RESET "\033[0m"
#define NGREEN "\033[1;32m"
#define NRED "\033[1;31m"
#define NPURPLE "\033[1;35m"

class PresidentialPardonForm : public AForm
{
	private:
		PresidentialPardonForm();
		std::string _target;
	public:
		PresidentialPardonForm(const std::string& targets);
		PresidentialPardonForm(const PresidentialPardonForm& src);
		~PresidentialPardonForm();
		PresidentialPardonForm& operator=(const PresidentialPardonForm& src);
		
		class GradeTooHighException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class GradeTooLowException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

	void	execute(Bureaucrat const& executor) const;
	const std::string& getTarget() const;
};std::ostream &operator<<(std::ostream &out, PresidentialPardonForm const &src);

#endif