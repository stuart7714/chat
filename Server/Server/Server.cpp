#include <cxxopts.hpp>
#include <iostream>

#include "Service.h"

int main(int argc, char* argv[])
{
	try
	{
		cxxopts::Options options("Chat", "A simple Chat server");
		options.add_options()
			("h,help", "Print usage")
			("url", "The URL of the chat server", cxxopts::value<std::string>());
		cxxopts::ParseResult parsed = options.parse(argc, argv);
		if (parsed.count("help"))
		{
			std::cout << options.help() << std::endl;
			return 0;
		}

		std::string url = parsed["url"].as<std::string>();
		Service{ url };
	}

	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
