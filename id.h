#ifndef ID_H
#define ID_H

#include <stack>

class Id
{
public:
	Id();
	~Id();
	
	unsigned int generate_id();
	void release_id(unsigned int id);

private:
	unsigned int id_;
    std::stack<unsigned int> avail_id_;
};

#endif
