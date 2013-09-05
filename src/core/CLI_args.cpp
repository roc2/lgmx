#include <CLI_args.h>
#include <iostream>


static void print_version(void *)
{
	std::cout << "lgmx - version 0.5 Copyright Leonardo Mariano" << std::endl;
}

static void print_help(void *)
{
	std::cout << "Arguments:\n"
	<< "  -h or --help  Print Help (this message) and exit\n"
	<< "  --version     Print version information and exit" << std::endl;
}

/**
 * Constructor.
 * @param argc - number of arguments.
 * @param argv - list of arguments.
 */

CLI_args::CLI_args(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
		args_.push_back(argv[i]);
	
	terminate_opts_["version"] = print_version;
	terminate_opts_["help"] = print_help;
}

/**
 * Destructor.
 */

CLI_args::~CLI_args()
{
}

/**
 * 
 */

bool CLI_args::empty() const
{
	return args_.empty();
}

/**
 * Searches and executes termination arguments. Termination arguments 
 * (--version, --help...) must execute and terminate the application, so they 
 * are parsed before the application is created.
 * @return false, if the application must be terminated, true otherwise.
 */

bool CLI_args::exec_pre_init()
{
	std::map<QString, f_ptr>::iterator m_it;
	std::list<QString>::iterator it;

	for (it = args_.begin(); it != args_.end(); it++) {
		if (it->startsWith("--")) {
			m_it = terminate_opts_.find((it->right(it->size() - 2)));

			if (m_it != terminate_opts_.end()) {
				m_it->second(NULL);
				return false;
			}
		}
	}
	
	return true;
}

/**
 * Return arguments list.
 */

std::list<QString>* CLI_args::get_args()
{
	return &args_;
}














