/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mszymcza <mszymcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:30:00 by mszymcza          #+#    #+#             */
/*   Updated: 2025/11/22 12:30:00 by mszymcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bureaucrat.hpp"
#include "../includes/Intern.hpp"
#include <iostream>

int main()
{
    Intern someRandomIntern;
    Bureaucrat boss("Boss", 1);
    AForm* rrf;
    AForm* scf;
    AForm* ppf;

    std::cout << NPURPLE << "=== Creating forms with Intern ===" << RESET << std::endl;

    // Test 1: robotomy request
    rrf = someRandomIntern.makeForm("robotomy request", "Bender");
    boss.signForm(*rrf);
    boss.executeForm(*rrf);
    delete rrf;

    std::cout << std::endl;

    // Test 2: shrubbery creation
    scf = someRandomIntern.makeForm("shrubbery creation", "home");
    boss.signForm(*scf);
    boss.executeForm(*scf);
    delete scf;

    std::cout << std::endl;

    // Test 3: presidential pardon
    ppf = someRandomIntern.makeForm("presidential pardon", "Arthur Dent");
    boss.signForm(*ppf);
    boss.executeForm(*ppf);
    delete ppf;

    std::cout << std::endl;
    std::cout << NRED << "=== Testing invalid form ===" << RESET << std::endl;

    // Test 4: invalid form
    try {
        AForm* invalid = someRandomIntern.makeForm("invalid form", "target");
        delete invalid;
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
