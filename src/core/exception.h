#ifndef _LGMX_EXCEPTION_
#define _LGMX_EXCEPTION_


namespace lgmx {

class exception : public std::exception
{
	public:
	exception() {};
	exception(const std::string &msg) : msg_(msg) {};
	~exception() throw() {};
	
	void set_message(const std::string &msg) {
		msg_ = msg;
	}
	void append_message(const std::string &msg) {
		msg_ += "\n" + msg;
	}
	std::string get_message() {
		return msg_;
	}

	private:
	std::string msg_;
};

}

#endif
