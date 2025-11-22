/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mszymcza <mszymcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 11:41:12 by mszymcza          #+#    #+#             */
/*   Updated: 2025/11/22 12:00:00 by mszymcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bureaucrat.hpp"
#include "../includes/AForm.hpp"
#include "../includes/ShrubberyCreationForm.hpp"
#include "../includes/RobotomyRequestForm.hpp"
#include "../includes/PresidentialPardonForm.hpp"
#include <iostream>

int main()
{
    int grade;

    std::cout << NPURPLE << "=============================================================="
              << RESET << std::endl;
    std::cout << NGREEN << "          TEST 1: ShrubberyCreationForm (145/137)           "
              << RESET << std::endl;
    std::cout << NPURPLE << "=============================================================="
              << RESET << std::endl;

    std::cout << NPURPLE << "Creating bureaucrat Alice" << RESET << std::endl;
    std::cout << "Enter grade for Alice (1-150): ";
    std::cin >> grade;
    try {
        Bureaucrat alice("Alice", grade);
        ShrubberyCreationForm shrubbery("Home");
        
        std::cout << alice << std::endl;
        std::cout << NGREEN << "Signing ShrubberyCreationForm..." << RESET << std::endl;
        alice.signForm(shrubbery);
        
        std::cout << NGREEN << "Executing ShrubberyCreationForm..." << RESET << std::endl;
        alice.executeForm(shrubbery);
    } catch (std::exception &e) {
        std::cout << NRED << "Exception: " << e.what() << RESET << std::endl;
    }

    std::cout << "\n" << NPURPLE << "=============================================================="
              << RESET << std::endl;
    std::cout << NGREEN << "          TEST 2: RobotomyRequestForm (72/45)              "
              << RESET << std::endl;
    std::cout << NPURPLE << "=============================================================="
              << RESET << std::endl;

    std::cout << NPURPLE << "Creating bureaucrat Bob" << RESET << std::endl;
    std::cout << "Enter grade for Bob (1-150): ";
    std::cin >> grade;
    try {
        Bureaucrat bob("Bob", grade);
        RobotomyRequestForm robotomy("Bender");
        
        std::cout << bob << std::endl;
        std::cout << NGREEN << "Signing RobotomyRequestForm..." << RESET << std::endl;
        bob.signForm(robotomy);
        
        std::cout << NGREEN << "Executing RobotomyRequestForm..." << RESET << std::endl;
        bob.executeForm(robotomy);
        
        std::cout << NGREEN << "Executing again..." << RESET << std::endl;
        bob.executeForm(robotomy);
    } catch (std::exception &e) {
        std::cout << NRED << "Exception: " << e.what() << RESET << std::endl;
    }

    std::cout << "\n" << NPURPLE << "=============================================================="
              << RESET << std::endl;
    std::cout << NGREEN << "          TEST 3: PresidentialPardonForm (25/5)            "
              << RESET << std::endl;
    std::cout << NPURPLE << "=============================================================="
              << RESET << std::endl;

    std::cout << NPURPLE << "Creating bureaucrat Charlie" << RESET << std::endl;
    std::cout << "Enter grade for Charlie (1-150): ";
    std::cin >> grade;
    try {
        Bureaucrat charlie("Charlie", grade);
        PresidentialPardonForm pardon("Arthur Dent");
        
        std::cout << charlie << std::endl;
        std::cout << NGREEN << "Signing PresidentialPardonForm..." << RESET << std::endl;
        charlie.signForm(pardon);
        
        std::cout << NGREEN << "Executing PresidentialPardonForm..." << RESET << std::endl;
        charlie.executeForm(pardon);
    } catch (std::exception &e) {
        std::cout << NRED << "Exception: " << e.what() << RESET << std::endl;
    }

    std::cout << "\n" << NPURPLE << "=============================================================="
              << RESET << std::endl;
    std::cout << NRED << "          TEST 4: Execution without signing                "
              << RESET << std::endl;
    std::cout << NPURPLE << "=============================================================="
              << RESET << std::endl;

    try {
        Bureaucrat diana("Diana", 1);
        ShrubberyCreationForm unsignedForm("Garden");
        
        std::cout << diana << std::endl;
        std::cout << NRED << "Trying to execute unsigned form..." << RESET << std::endl;
        diana.executeForm(unsignedForm);
    } catch (std::exception &e) {
        std::cout << NRED << "Exception: " << e.what() << RESET << std::endl;
    }

    std::cout << "\n" << NPURPLE << "=============================================================="
              << RESET << std::endl;
    std::cout << NRED << "          TEST 5: Insufficient grade to execute            "
              << RESET << std::endl;
    std::cout << NPURPLE << "=============================================================="
              << RESET << std::endl;

    try {
        Bureaucrat edward("Edward", 50);
        Bureaucrat frank("Frank", 1);
        PresidentialPardonForm pardon2("Zaphod");
        
        std::cout << frank << std::endl;
        std::cout << NGREEN << "Frank signs the form..." << RESET << std::endl;
        frank.signForm(pardon2);
        
        std::cout << edward << std::endl;
        std::cout << NRED << "Edward tries to execute (grade too low)..." << RESET << std::endl;
        edward.executeForm(pardon2);
    } catch (std::exception &e) {
        std::cout << NRED << "Exception: " << e.what() << RESET << std::endl;
    }

    return 0;
}


