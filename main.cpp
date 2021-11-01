
#include <iostream>
#include <vector>
#include <string>

#include "config.h"

int main(){

	std::vector<std::string> vec;

	config::CConfig cfg;
	cfg.readto("./test.txt", vec);

	for(auto it : vec){
		std::cout << it << std::endl;
	}

	return 0;
}

