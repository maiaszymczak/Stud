/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mszymcza <mszymcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 20:43:57 by mszymcza          #+#    #+#             */
/*   Updated: 2025/11/05 20:43:57 by mszymcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"

Cat::Cat()
{
	this->type = "Cat";
	this->brain = new Brain();
	std::cout << YELLOW << "🐱 Cat default constructor called" << RESET << std::endl;
}

Cat::Cat(const Cat& other) : Animal(other)
{
	std::cout << YELLOW << "🐱 Cat copy constructor called" << RESET << std::endl;
	this->brain = new Brain(*other.brain);
}

Cat& Cat::operator=(const Cat& other)
{
	std::cout << YELLOW << "🐱 Cat assignment operator called" << RESET << std::endl;
	if (this != &other)
	{
		Animal::operator=(other);
		delete this->brain;
		this->brain = other.brain ? new Brain(*other.brain) : NULL;
	}
	return *this;
}

Cat::~Cat()
{
	delete this->brain;
	std::cout << YELLOW << "🐱 Cat destructor called" << RESET << std::endl;
}

void Cat::makeSound() const
{
	std::cout << YELLOW << "🐱 Meow! Meow!" << RESET << std::endl;
}

Brain* Cat::getBrain() const
{
	return this->brain;
}
