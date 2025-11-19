#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"
#include "Brain.hpp"

int main()
{
	std::cout << "--- Test: Animal is now abstract ---\n" << std::endl;
	
	std::cout << "\n--- Test: Dog and Cat ---\n" << std::endl;
	const Animal* j = new Dog();
	const Animal* i = new Cat();
	
	j->makeSound();
	i->makeSound();
	
	delete j; //should not create a leak
	delete i;

	std::cout << "\n--- Array of Animals ---\n" << std::endl;

	Animal* animals[4];
	animals[0] = new Dog();
	animals[1] = new Dog();
	animals[2] = new Cat();
	animals[3] = new Cat();
	
	std::cout << "\nAll animals making sounds:" << std::endl;
	for(int i = 0; i < 4; i++)
	{
		std::cout << "Animal " << i << ": ";
		animals[i]->makeSound();
	}
	
	std::cout << "\nDeleting all animals:" << std::endl;
	for(int i = 0; i < 4; i++)
		delete animals[i];

	std::cout << "\n--- Deep Copy Test ---\n" << std::endl;
	
	Dog basic;
	std::cout << "Empty idea: " << basic.getBrain()->getIdea(0) << std::endl;
	{
		Dog tmp;
		tmp.getBrain()->setIdea(0, "Woof!");
		std::cout << "Original idea: " << tmp.getBrain()->getIdea(0) << std::endl;
		basic = tmp;
	}
	std::cout << "Copied idea: " << basic.getBrain()->getIdea(0) << std::endl;
	return 0;
}
