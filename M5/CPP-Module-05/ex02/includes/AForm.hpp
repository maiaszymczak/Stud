/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AForm.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mszymcza <mszymcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 10:21:32 by mszymcza          #+#    #+#             */
/*   Updated: 2025/11/18 11:41:41 by mszymcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AFORM_HPP
#define AFORM_HPP

#include <iostream>
#include <stdexcept>
#include <exception>
#include <string>
#include "Bureaucrat.hpp"

// Colors
#define RESET "\033[0m"
#define NGREEN "\033[1;32m"
#define NRED "\033[1;31m"
#define NPURPLE "\033[1;35m"

class AForm
{
    protected:
		AForm();
        const std::string name;
        bool isSigned;
        const int gradeToSign;
        const int gradeToExecute;

    public:
        AForm(const std::string& name, int gradeToSign, int gradeToExecute);
        AForm(const Form& other);
        AForm& operator=(const Form& other);
       virtual ~AForm();
        const std::string& getName() const;
        bool getIsSigned() const;
        int getGradeToSign() const;
        int getGradeToExecute() const;
       virtual void beSigned(const Bureaucrat& bureaucrat);
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
};

std::ostream& operator<<(std::ostream& os, const AForm& form);

#endif