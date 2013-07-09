#include <fstream>
#include <iostream>
#include <sstream>

#include <QString>

#include <settings.h>

Settings::Settings()
{
	tab_width_ = 4;
	line_wrap_ = false;
	read_settings("teste");
}

Settings::~Settings()
{
}

bool Settings::read_settings(const QString &file_path)
{
	//std::ifstream conf_file(file_path.toStdString().c_str());

	std::ifstream conf_file("/home/lgm/json");
	std::stringstream buff;

	if (conf_file.is_open()) {
		buff << conf_file.rdbuf();
		//std::cout << buff.str() << std::endl;
		conf_file.close();
	} else {
		return false;
	}
	
	return true;
}

bool Settings::parse_json(std::string &buff)
{
	/*
	char *errorPos = 0;
	char *errorDesc = 0;
	int errorLine = 0;
	block_allocator allocator(1 << 10); // 1 KB per block
			
	json_value *root = json_parse(const_cast<char *>(buff.c_str()), &errorPos, &errorDesc, &errorLine, &allocator);
	
	for (json_value *it = root->first_child; it; it = it->next_sibling) {
        if (value->name) {
                //printf("%s = ", value->name);
                std::cout << value->name << std::endl;
        }

        switch(value->type)
        {
        case JSON_NULL:
                //printf("null\n");
                break;
        case JSON_STRING:
                //printf("\"%s\"\n", value->string_value);
                std::cout << value->string_value << std::endl;
                break;
        case JSON_INT:
                //printf("%d\n", value->int_value);
                std::cout << value->int_value << std::endl;
                break;
        case JSON_FLOAT:
                //printf("%f\n", value->float_value);
                std::cout << value->float_value << std::endl;
                break;
        case JSON_BOOL:
                //printf(value->int_value ? "true\n" : "false\n");
                std::cout << value->int_value ? "true" : "false" << std::endl;
                break;
        }
	}*/

	return true;
}

tab_settings& Settings::get_tab_settings()
{
	return tab_settings_;
}

bool Settings::set_tab_width(int size)
{
	if (size <= 0)
		return false;
	
	tab_width_ = size;
	return true;
}


int Settings::get_tab_width()
{
	return tab_settings_.get_tab_width();
}
	
void Settings::set_line_wrap(bool wrap)
{
	line_wrap_ = wrap;
}

bool Settings::get_line_wrap()
{
	return line_wrap_;
}










