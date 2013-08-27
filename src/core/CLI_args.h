#ifndef CLI_ARGS_H
#define CLI_ARGS_H

#include <list>
#include <map>
#include <QString>


typedef void (*f_ptr)(void *);


class CLI_args
{
private:
	CLI_args(const CLI_args&);
	CLI_args& operator=(const CLI_args&);
	
public:
	CLI_args(int argc, char *argv[]);
	~CLI_args();

	bool empty() const;
	bool exec_pre_init();

	std::list<QString>* get_args();

private:
	std::list<QString> args_;
	std::map<QString, f_ptr> terminate_opts_;
};


#endif
